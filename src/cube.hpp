#ifndef CUBE_H

#define CUBE_H


enum BlockType {
    BlockType_Grass,
    BlockType_Water,
    BlockType_Stone,
    BlockType_Air

};



class Cube
{
public:
    Cube();
    Cube(BlockType blocktype);
    ~Cube();
	bool isActive() const;
	void SetActive(bool active);
    BlockType GetBlockType();
    void SetBlock(BlockType blocktype);
    float BLOCK_RENDER_SIZE = .5f;

private: 
	bool m_active;
	BlockType m_blockType;
};





#endif