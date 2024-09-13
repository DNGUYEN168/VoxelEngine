#include "World.hpp"
#include <iostream>

#include "PerlinNoise.hpp"

World::World(int x, int z): m_WorldSize_x{x}, m_WorldSize_z{z}
{
	//// generate and store noise map
	//m_NoiseMap = new std::vector<double>();
	//CreateNoiseMap();


	for (int x = 0; x < CHUNKSIZE * m_WorldSize_x; x += 16)
	{
		for (int z = 0; z < CHUNKSIZE * m_WorldSize_z; z += 16)
		{
			m_Chunks.push_back(new ChunkIMP(this, glm::vec3(x, 0, z)));
		}

	}


	//m_NoiseMap->clear(); // empty our noise map
}

World::~World()
{
	for (int i = 0; i < m_Chunks.size(); i++)
	{
		delete m_Chunks[i];
	}
}


void World::CreateWorld()
{
	for (int i = 0; i < m_Chunks.size(); i++) // initilize the chunks and fill in initial data
	{
		m_Chunks[i]->m_pRenderer->CreateMesh();
		std::cout << "We are on chunk: " << i << std::endl;
		m_Chunks[i]->LoopThroughBlocks(); 
		m_Chunks[i]->m_pRenderer->FinishMesh();
		std::cout << std::endl;
	}
}


void World::Render(GLuint VAO, int indexCount) {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) { std::cout << "whoopsies: " << errorCode << std::endl; }
	glBindVertexArray(0);

}

void World::drawMesh()
{
	for (int i = 0; i < m_Chunks.size(); i++)
	{
		Render(m_Chunks[i]->m_pRenderer->m_VAO, m_Chunks[i]->m_pRenderer->m_indices.size());

	}




}


