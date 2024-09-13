#include "ChunkIMP.hpp"
#include <random>
#include "PerlinNoise.hpp"

ChunkIMP::ChunkIMP(World* worldReference, glm::vec3 worldPosition) : m_worldReference {worldReference }, m_worldPosition{ worldPosition }, m_pRenderer{nullptr}
{
	if (m_pRenderer == nullptr) {
		m_pRenderer = new Renderer(); // create new render class tied to chunk
	}

	m_voxels = new Cube * [maxVoxelCount]; // array pointer allocation
	for (int i = 0; i < maxVoxelCount; i++) // filling the voxel array
	{
		glm::vec3 blockPOS = Convert1Dto3D(i);
		if (blockPOS.y < 3) { m_voxels[i] = new Cube(BlockType_Water); }

		else if (blockPOS.y < 10 ) { m_voxels[i] = new Cube(BlockType_Stone); }

		else { m_voxels[i] = new Cube(); }
	}

	// perlin noise generation
	const siv::PerlinNoise::seed_type seed = 65152415516;

	const siv::PerlinNoise perlin{ seed };

	for (int x = 0; x < CHUNKSIZE; x++)
	{
		for (int z = 0; z < CHUNKSIZE; z++)
		{
			double noise = perlin.octave2D_01(((x + m_worldPosition.x) * 0.01), ((z + m_worldPosition.z) * 0.01), 16);

			for (int y = noise * 20+1; y < CHUNKSIZE; y++)
			{
				m_voxels[Convert3Dto1D(x, y, z)]->SetBlock(BlockType_Air);

			}

		}
	}

	
}

ChunkIMP::~ChunkIMP()
{
	delete m_pRenderer; // delete 
	for (int i = 0; i < maxVoxelCount; i++)
	{
		delete m_voxels[i]; // Delete each Cube
	}
	delete[] m_voxels; // Delete the array of pointers
}

glm::vec4 ChunkIMP::GetBlockColor(BlockType blocktype)
{
	switch (blocktype) {
	case BlockType_Grass: return glm::vec4(0, .65, 0, 1);
	case BlockType_Water: return glm::vec4(0, 0, .65, 1);
	case BlockType_Stone: return glm::vec4(.68f, .68f, 0.46f, 1);
	case BlockType_Air:   return glm::vec4(0, 0, 0, 0);

	default:       return glm::vec4(0, 0, 0,0); // Default case (shouldn't be hit)
	}

}


int ChunkIMP::FindChunkIndex(int x, int z) // given the normailzed position of the nieghborblock, find that check that lives in 
{
	return (x * m_worldReference->m_WorldSize_x) + z ;
}

BlockType ChunkIMP::GetBlockInformation(int index)
{

	return m_voxels[index]->GetBlockType(); // returns the blockType at this index
}

int ChunkIMP::Convert3Dto1D(int x, int y, int z)
{

	return (z * CHUNKSIZE * CHUNKSIZE) + (y * CHUNKSIZE) + x;
}

glm::vec3 ChunkIMP::Convert1Dto3D(int index) const
{
	int z = index / (CHUNKSIZE * CHUNKSIZE);
	int y = (index % (CHUNKSIZE * CHUNKSIZE)) / CHUNKSIZE;
	int x = index % CHUNKSIZE;
	return glm::vec3(x,y,z);
}

bool ChunkIMP::InRangeXZ(int axisCoord) const // for x y z checks
{
	// 4096 is the max number of voxels in our 
	if (axisCoord < 0 || axisCoord > CHUNKSIZE- 1) { return false; } // x y or z doesnt fall within the 16x16x16
	return true;
}


void ChunkIMP::LoopThroughBlocks() 
{

	for (int i = 0; i < maxVoxelCount; i++)
	{
		//int x, int y, int z, ChunkIMP* chunk, BlockType blocktype
		glm::vec3 blockChunkPosition = Convert1Dto3D(i); // blocks x y z
		m_pRenderer->FilterMeshData((int)blockChunkPosition.x, (int)blockChunkPosition.y, (int)blockChunkPosition.z, this, m_voxels[i]->GetBlockType());
		// most likely check blocks and its nieghboors / send mesh data 
	}

}

void ChunkIMP::SetBlockType(BlockType blocktype, int index)
{
	m_voxels[index]->SetBlock(blocktype); // sets the block to be that kind
}

glm::vec3 ChunkIMP::GetBlockInChunkCoordinates(glm::vec3 pos) const
{
	// pass in the blocks WORLD position and calcs the block position relative to the chunk
	int x = pos.x - m_worldPosition.x;
	int y = pos.y - m_worldPosition.y;
	int z = pos.z - m_worldPosition.z;
	return glm::vec3(x, y, z); // now we can use this to find the block in the voxel array 
}
