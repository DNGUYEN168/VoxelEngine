#include "cube.hpp"


Cube::Cube(): m_active{true}, m_blockType{BlockType_Grass} {}

Cube::Cube(BlockType blocktype): m_active { true }, m_blockType{ blocktype } {}

// we assume all blocks will be active until otherwise stated 

Cube::~Cube() {}

bool Cube::isActive() const 
{
	return m_active;
}

void Cube::SetActive(bool active)
{
	m_active = active;
}

BlockType Cube::GetBlockType()
{
	return m_blockType;
}

void Cube::SetBlock(BlockType blocktype)
{
	m_blockType = blocktype;
}
