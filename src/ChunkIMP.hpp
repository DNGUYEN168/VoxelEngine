#ifndef CHUNKIMP_H

#define CHUNKIMP_H

#include "cube.hpp"

class World;
#include "World.hpp"

class Renderer;
#include "Renderer.hpp"

#include <glm/glm.hpp>

class ChunkIMP
{

public:
	ChunkIMP(World* worldReference, glm::vec3 worldPosition);
	~ChunkIMP();

	Cube ** m_voxels; 

	World* m_worldReference; //pointer to main world class

	glm::vec3 m_worldPosition;
	Renderer * m_pRenderer;
	bool NeedsUpdate = false; // check if chunk needs to be changed b/c a block has become unactive 

	int CHUNKSIZE = 16;

	int maxVoxelCount = CHUNKSIZE * CHUNKSIZE * CHUNKSIZE;

	BlockType GetBlockInformation(int index); // return the block information (i.e. its type) 

	int Convert3Dto1D(int x, int y, int z); 
	glm::vec3 Convert1Dto3D(int index) const;

	bool InRangeXZ(int axisCoord) const;

	void LoopThroughBlocks();

	void SetBlockType(BlockType blocktype, int index);

	glm::vec4 GetBlockColor(BlockType blocktype);

	//WORLD STUFF
	glm::vec3 GetBlockInChunkCoordinates(glm::vec3 pos) const; // given world coords, we convert relative to chunk coords 
	int FindChunkIndex(int x, int z); // given the normailzed position of the 


	// RENDERER WILL HANDLE MESH GENERATION AND BE GIVEN THE CHUNK'S INFORMATION
private:
	// // handles mesh generation and checks or watnot 

};


#endif