#include "Renderer.hpp"
#include "ChunkIMP.hpp"
#include <iostream>
 
Renderer::Renderer() : m_VAO{ 0 }, m_EBO{ 0 }, m_VBO{ 0 }
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderChunks()
{
    // call draw elements 
}

void Renderer::CreateMesh() 
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 98304 * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);

    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) { std::cout << "whoopsies" << std::endl; }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 147456 * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

    errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) { std::cout << "whoopsies" << std::endl; }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, Position));
    glEnableVertexAttribArray(0); // enable this attrib

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, Color));
    glEnableVertexAttribArray(1); // enable this attrib

    errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) { std::cout << "whoopsies" << std::endl; }

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
    glBindVertexArray(0); // Unbind the VA

    errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) { std::cout << "whoopsies" << std::endl; }
    //std::cout << m_MeshID << " " << m_VBO << " " << m_EBO << std::endl;
}

void Renderer::AddQuadTriangles()
{
    m_indices.push_back(m_vertices.size() - 4);
    m_indices.push_back(m_vertices.size() - 3);
    m_indices.push_back(m_vertices.size() - 2);

    m_indices.push_back(m_vertices.size() - 4);
    m_indices.push_back(m_vertices.size() - 2);
    m_indices.push_back(m_vertices.size() - 1);
}

glm::vec4 Renderer::simulateLighting(glm::vec4 color, bool isDark)
{
    if (isDark) 
    {
        if (color.r > .21) { color.r = color.r - .21; }
        if (color.g > .21) { color.g = color.g - .21; }
        if (color.b > .21) { color.b = color.b - .21; }
    }

    else
    {
        if (color.r < .7) { color.r = color.r + .11; }
        if (color.g < .7) { color.g = color.g + .11; }
        if (color.b < .7) { color.b = color.b + .11; }

    }
    glm::vec4 new_color = glm::vec4(color.r, color.g, color.b, color.a);
    return new_color;
}

glm::vec3 Renderer::GetDirectionVector(Directions direction)
{
    switch (direction) {
    case forward:  return glm::vec3(0, 0, 1);
    case backward: return glm::vec3(0, 0, -1);
    case right:    return glm::vec3(1, 0, 0);
    case left:     return glm::vec3(-1, 0, 0);
    case up:       return glm::vec3(0, 1, 0);
    case down:     return glm::vec3(0, -1, 0);
    default:       return glm::vec3(0, 0, 0); // Default case (shouldn't be hit)
    }
}


void Renderer::FilterMeshData(int x, int y, int z, ChunkIMP* chunk, BlockType blocktype)
{
    //loop through each direction at this specific block checking its nieghbors 
    //chunk->m_voxels[1]->GetBlockType(); // using this looping method we can check 

    glm::vec4 mainBlockColor = chunk->GetBlockColor(blocktype); // gets the color of the block
    
    int blockIndex = chunk->Convert3Dto1D(x, y, z);
    if (blocktype == BlockType_Air || !chunk->m_voxels[blockIndex]->isActive()) { return; } // we dont add air to meshData or unactive blocks
    for (int i = 0; i < static_cast<int>(Directions::Count); ++i) // loops through 6 directions 
    {
        Directions dir = static_cast<Directions>(i); // get direction enum
        // main voxel x y z + direction vector = neighboring voxel x y z
        glm::vec3 NeighborBlock = glm::vec3(x, y, z) + GetDirectionVector(dir); // center position of the nieghboring block

        if (NeighborBlock.y < 0) { continue; } // TODO: In future dont generalize like this 

        if (chunk->InRangeXZ(NeighborBlock.x) && chunk->InRangeXZ(NeighborBlock.z) && chunk->InRangeXZ(NeighborBlock.y)) // make sure neighbor is within our chunk
        {
            // if in chunk we know for sure we are in a valid index
            int NeighborIndex = chunk->Convert3Dto1D(NeighborBlock.x, NeighborBlock.y, NeighborBlock.z);

            //now we can run a if else check statements 
            BlockType NeighborBlockType = chunk->GetBlockInformation(NeighborIndex);

            if (NeighborBlockType == BlockType_Air || !chunk->m_voxels[NeighborIndex]->isActive()) // if block is air or is not active
            {
                AddVertextoMesh(mainBlockColor, dir, x + chunk->m_worldPosition.x, y + chunk->m_worldPosition.y, z + chunk->m_worldPosition.z); // direction (face we want to render) and center of the block
                AddQuadTriangles();

            }
            else { /*std::cout << "dont render this face" << std::endl;*/ continue; } 

        }
        else if (dir != up) // not in chunk and max hieght limit render
        {

            int NeighborBlockWorldX = NeighborBlock.x + chunk->m_worldPosition.x;
            int NeighborBlockWorldY = NeighborBlock.y + chunk->m_worldPosition.y;
            int NeighborBlockWorldZ = NeighborBlock.z + chunk->m_worldPosition.z;

            int NieghborChunkIndex = chunk->FindChunkIndex(NeighborBlockWorldX >> 4, NeighborBlockWorldZ >> 4); // index that the nieghblock lives in 

            if (0 <= NieghborChunkIndex && NieghborChunkIndex < chunk->m_worldReference->m_Chunks.size()) // falls within the chunk list
            {
                //std::cout << NieghborChunkIndex << " ";
                ChunkIMP* neighborChunk = chunk->m_worldReference->m_Chunks[NieghborChunkIndex];
                //std::cout << neighborChunk->m_worldPosition.x << " " << neighborChunk->m_worldPosition.y << " " << neighborChunk->m_worldPosition.z << std::endl;
                // find the nieghbor blocks local chunk position 
                int nb_local_posX = NeighborBlockWorldX % 16;
                int nb_local_posY = NeighborBlockWorldY % 16;
                int nb_local_posZ = NeighborBlockWorldZ % 16;

                if (chunk->InRangeXZ(nb_local_posX) && chunk->InRangeXZ(nb_local_posY) && chunk->InRangeXZ(nb_local_posZ))
                {

                    int nblocalIndex = chunk->Convert3Dto1D(nb_local_posX, nb_local_posY, nb_local_posZ);
                    BlockType nbBlockType = neighborChunk->GetBlockInformation(nblocalIndex);

                    if (nbBlockType == BlockType_Air || !neighborChunk->m_voxels[nblocalIndex]->isActive()) // if block is air or is not active
                    {
                        AddVertextoMesh(mainBlockColor, dir, x + chunk->m_worldPosition.x, y + chunk->m_worldPosition.y, z + chunk->m_worldPosition.z); // direction (face we want to render) and center of the block
                        AddQuadTriangles();
                    }
                    else { continue; }
                }
            }
        }
        else 
        {
            //TODO REMOVE THIS PART AND ADD CHUNKS CHECKING
            AddVertextoMesh(mainBlockColor, dir, x + chunk->m_worldPosition.x, y + chunk->m_worldPosition.y, z + chunk->m_worldPosition.z); // direction (face we want to render) and center of the block
            AddQuadTriangles();
            // TODO : make a function that can find the chunk with which that nieghborblock resides in and convert that into a 1d index 
            // use that new index to find the chunk and then find that specific block within that chunk and then check its type 
        }

    }
}



void Renderer::FinishMesh() const
{

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO); //bind 
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Point), &m_vertices.front());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_EBO); //bind 
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_indices.size() * sizeof(unsigned int), &m_indices.front());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Renderer::AddVertextoMesh(glm::vec4 blockColor, Directions direction, int x, int y, int z) // based on direction we only add that face to the vertices 
{
    Point Vpoint{};
    Vpoint.Color = blockColor; // set block color
    //std::cout << blockColor.x << " " << blockColor.y << " " << blockColor.z << " " << blockColor.a << " " << std::endl;
    switch (direction)
    {
    case backward:
        Vpoint.Color = simulateLighting(Vpoint.Color, true);
        Vpoint.Position = glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        break;
    case forward:
        Vpoint.Color = simulateLighting(Vpoint.Color, false);
        Vpoint.Position = glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        break;
    case left:
        Vpoint.Color = simulateLighting(Vpoint.Color, true);
        Vpoint.Position = glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        break;

    case right:
        Vpoint.Color = simulateLighting(Vpoint.Color, false);
        Vpoint.Position = glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        break;
    case down:
        Vpoint.Position = glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        break;
    case up:
        Vpoint.Position = glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        Vpoint.Position = glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f);
        m_vertices.push_back(Vpoint);
        break;
    default:
        break;
    }
}




