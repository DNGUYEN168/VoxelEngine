#ifndef RENDERER_H

#define RENDERER_H

class ChunkIMP;
enum BlockType;

#include "cube.hpp"
#include "ChunkIMP.hpp"

// C++ includes 
#include <vector>
#include <iostream>

// glm inserts 
#include <glm/glm.hpp>

// glew / glfw 
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <vector>


struct Point
{
	glm::vec3 Position;
	glm::vec4 Color;
};

enum Directions { // using these faces we can calculate the sides of each face and wether its nieghboor is of a certain type of block
	forward,  // z + 1  direction 
	right,  // 1 +x direction
	backward,   //1 -z direction
	left,   // 1-x direction
	up,     // 1+y direction
	down,    // 1-y direction
	Count
	// take the cube's pos and add 1 to any x y z to get its nieghbor
};

class Renderer
{
	// mesh data container 
public:
	std::vector<Point> m_vertices;

	std::vector<unsigned int> m_indices;

	unsigned int m_VAO, m_VBO, m_EBO; // VAO ID 


    Renderer();
    ~Renderer();
	void RenderChunks();
	void FinishMesh() const;
	void CreateMesh(); // generate the mesh id stuffs 

    glm::vec3 GetDirectionVector(Directions direction); // use this to calculate the position of niehgboring blocks 

	void AddVertextoMesh(glm::vec4 blockColor, Directions direction, int x, int y, int z);
	// based on direction (face) we add only that face to our vertex vector
	// x y z represent the center of cube we want to add 

	void FilterMeshData(int x, int y, int z, ChunkIMP* chunk, BlockType blocktype);
		// algo to add only the speific faces, will loop through the 6 directions 
		// take the voxel position, the specific chunk it resides in, and the block's type 


	void AddQuadTriangles();

	glm::vec4 simulateLighting(glm::vec4 color, bool isDark); // change the hues depending on the faces 

private:

};

#endif