//(c) gijs.i voxel engine

#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>

#include "buffer.hpp"
#include "texture.hpp"
#include "block.hpp"

#define BLOCK_AIR 1

class Block {
protected:
	TextureUV *topTexture;
	TextureUV* bottomTexture;
	TextureUV* leftTexture;
	TextureUV* rightTexture;
	TextureUV* frontTexture;
	TextureUV* backTexture;
public:
	Block(std::string blockName, TextureUV *topUV, TextureUV* bottomUV, TextureUV* leftUV, TextureUV* rightUV, TextureUV* frontUV, TextureUV* backUV) {
		topTexture = topUV;
		bottomTexture = bottomUV;
		leftTexture = leftUV;
		rightTexture = rightUV;
		frontTexture = frontUV;
		backTexture = backUV;
	}

	~Block() {
	}

	void generateMesh(Buffer* buffer, float xPos, float yPos, float zPos, BlockSpaceInfo blockSpaceInfo) {
		if (blockSpaceInfo.rightEmpty) {
			AOMap aoMap = { 0.0f, 0.0f, 0.0f, 0.0f };
			if (blockSpaceInfo.lowerRightOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.bottomRight = 1.0f;
			}
			else {
				if (blockSpaceInfo.lowerTopRightOccupied) aoMap.bottomRight = 1.0f;
				if (blockSpaceInfo.lowerDownRightOccupied) aoMap.bottomLeft = 1.0f;
			}
			if (blockSpaceInfo.topRightOccupied) {
				//TODO: also check bottomRight and topRight seperately for ambient occlusion
				aoMap.bottomRight = 1.0f;
				aoMap.topRight = 1.0f;
			}
			if (blockSpaceInfo.bottomRightOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.topLeft = 1.0f;
			}
			buffer->createRightVerticalPlane(xPos, yPos, zPos, rightTexture, aoMap);
		}

		if (blockSpaceInfo.leftEmpty) {
			AOMap aoMap = { 0.0f, 0.0f, 0.0f, 0.0f };
			if (blockSpaceInfo.lowerLeftOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.bottomRight = 1.0f;
			}
			else {
				if (blockSpaceInfo.lowerTopLeftOccupied) aoMap.bottomLeft = 1.0f;
				if (blockSpaceInfo.lowerDownLeftOccupied) aoMap.bottomRight = 1.0f;
			}
			if (blockSpaceInfo.topLeftOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.topLeft = 1.0f;
			}
			if (blockSpaceInfo.bottomLeftOccupied) {
				aoMap.bottomRight = 1.0f;
				aoMap.topRight = 1.0f;
			}
			buffer->createLeftVerticalPlane(xPos, yPos, zPos, leftTexture, aoMap);
		}

		if (blockSpaceInfo.topEmpty) {
			AOMap aoMap = { 0.0f, 0.0f, 0.0f, 0.0f };
			if (blockSpaceInfo.upperTopOccupied) {
				aoMap.topLeft = 1.0f;
				aoMap.topRight = 1.0f;
			}
			else {
				if (blockSpaceInfo.upperTopRightOccupied) aoMap.topRight = 1.0f;
				if (blockSpaceInfo.upperTopLeftOccupied) aoMap.topLeft = 1.0f;
			}

			if (blockSpaceInfo.upperRightOccupied) {
				aoMap.topRight = 1.0f;
				aoMap.bottomRight = 1.0f;
			}
			else {
				if (blockSpaceInfo.upperTopRightOccupied) aoMap.topRight = 1.0f;
				if (blockSpaceInfo.upperDownRightOccupied) aoMap.bottomRight = 1.0f;
			}

			if (blockSpaceInfo.upperDownOccupied) {
				aoMap.bottomRight = 1.0f;
				aoMap.bottomLeft = 1.0f;
			}
			else {
				if (blockSpaceInfo.upperDownRightOccupied) aoMap.bottomRight = 1.0f;
				if (blockSpaceInfo.upperDownLeftOccupied) aoMap.bottomLeft = 1.0f;
			}

			if (blockSpaceInfo.upperLeftOccupied) {
				aoMap.topLeft = 1.0f;
				aoMap.bottomLeft = 1.0f;
			}
			else {
				if (blockSpaceInfo.upperTopLeftOccupied) aoMap.topLeft = 1.0f;
				if (blockSpaceInfo.upperDownLeftOccupied) aoMap.bottomLeft = 1.0f;
			}
			buffer->createTopHorizontalPlane(xPos, yPos, zPos, topTexture, aoMap);
		}

		if (blockSpaceInfo.bottomEmpty) {
			AOMap aoMap = { 0.0f, 0.0f, 0.0f, 0.0f };
			buffer->createBottomHorizontalPlane(xPos, yPos, zPos, bottomTexture, aoMap);
		}

		if (blockSpaceInfo.frontEmpty) {
			AOMap aoMap = { 0.0f, 0.0f, 0.0f, 0.0f };
			if (blockSpaceInfo.lowerDownOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.bottomRight = 1.0f;
			}
			else {
				if (blockSpaceInfo.lowerDownLeftOccupied) aoMap.bottomLeft = 1.0f;
				if (blockSpaceInfo.lowerDownRightOccupied) aoMap.bottomRight = 1.0f;
			}
			if (blockSpaceInfo.bottomRightOccupied) {
				aoMap.bottomRight = 1.0f;
				aoMap.topRight = 1.0f;
			}
			if (blockSpaceInfo.bottomLeftOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.topLeft = 1.0f;
			}
			buffer->createFrontVerticalPlane(xPos, yPos, zPos, frontTexture, aoMap);
		}

		if (blockSpaceInfo.backEmpty) {
			AOMap aoMap = { 0.0f, 0.0f, 0.0f, 0.0f };
			if (blockSpaceInfo.lowerTopOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.bottomRight = 1.0f;
			}
			else {
				if (blockSpaceInfo.lowerTopLeftOccupied) aoMap.bottomRight = 1.0f;
				if (blockSpaceInfo.lowerTopRightOccupied) aoMap.bottomLeft = 1.0f;
			}
			if (blockSpaceInfo.topRightOccupied) {
				aoMap.bottomLeft = 1.0f;
				aoMap.topLeft = 1.0f;
			}
			if (blockSpaceInfo.topLeftOccupied) {
				aoMap.bottomRight = 1.0f;
				aoMap.topRight = 1.0f;
			}
			buffer->createBackVerticalPlane(xPos, yPos, zPos, backTexture, aoMap);
		}
	}
};

class BlockRegistry {
private:
	std::vector<Block*> blocks;
	TextureUV* dirt = new TextureUV(0.0f, 0.0f, 16.0f, 16.0f);
	TextureUV* grassSide = new TextureUV(16.0f, 0.0f, 16.0f, 16.0f);
	TextureUV* grassTop = new TextureUV(32.0f, 0.0f, 16.0f, 16.0f);
	TextureUV* stone = new TextureUV(48.0f, 0.0f, 16.0f, 16.0f);

	//Register block inside vector array
	void registerBlock(Block *block) {
		blocks.push_back(block);
	}

public:
	//Constructor
	BlockRegistry() {
		registerBlock(new Block("Dirt", dirt, dirt, dirt, dirt, dirt, dirt));
		registerBlock(new Block("Grass", grassTop, dirt, grassSide, grassSide, grassSide, grassSide));
		registerBlock(new Block("Stone", stone, stone, stone, stone, stone, stone));
	}

	//Destructor
	~BlockRegistry() {
		delete dirt;
		delete grassSide;
		delete grassTop;
		delete stone;
		for (Block* block : blocks) {
			delete block;
		}
		blocks.clear();
	}


	//Get block from ID
	Block* getBlock(unsigned int id) {
		return blocks[id - 1];
	}
};


enum class BlockEnum {
	AIR = 0,
	DIRT = 1,
	GRASS = 2,
	STONE = 3
};

#endif