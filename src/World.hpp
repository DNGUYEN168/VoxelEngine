#ifndef WORLD_H

#define WORLD_H

// glew / glfw 
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <vector>


class ChunkIMP;
#include "ChunkIMP.hpp"

class World
{
public:
	// hold all the chunks 
	// keep track of those that need to be updated
	World(int x, int z);
	~World();
	int m_WorldSize_x;
	int m_WorldSize_z;
	int CHUNKSIZE = 16;

	//void GeneratePerlinNoise();

	// want to do chunks 1d y = 0 and x /z = their respive coords 

	std::vector<ChunkIMP*> m_Chunks; // inc x ,z by 16 and y = 0 

	//std::vector<double>* m_NoiseMap;

	//void CreateNoiseMap();
	void CreateWorld();

	void Render(GLuint VAO, int indexCount);


	// draw all triangles 
	void drawMesh();


};

#endif
