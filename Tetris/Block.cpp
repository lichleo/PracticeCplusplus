#include "Block.h"
#include <stdlib.h>

IMAGE* Block::imgs[7] = { NULL, };
int Block::size = 36;

Block::Block()
{
	int blocks[7][4] = {
		1,3,5,7, // I
		2,4,5,7, // Z 1��
		3,5,4,6, // Z 2��
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // ��
	};

	// ��������ķ���
	blockType = 1 + rand() % 7;
	img = imgs[blockType - 1];

	for (int i = 0; i < 4; i++) 
	{
		int value = blocks[blockType - 1][i];
		smallBlocks[i].row = value / 2;
		smallBlocks[i].col = value % 2;
	}

	// ����ʼ��һ��
	if (imgs[0] == NULL) 
	{
		IMAGE imgTmp;
		loadimage(&imgTmp, _T("res/tiles.png"));
		SetWorkingImage(&imgTmp);
		for (int i = 0; i < 6; i++) 
		{
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}
		imgs[6] = new IMAGE;
		getimage(imgs[6], 5 * size, 0, size, size);

		SetWorkingImage();
		srand(static_cast<unsigned int>(time(nullptr)));
	}

}

Block::Block(const Block& other)
{
	if (this == &other) return;

	this->blockType = other.blockType;
	for (int i = 0; i < 4; i++) 
	{
		this->smallBlocks[i] = other.smallBlocks[i];
	}
}

//Block& Block::operator=(const Block& other)
//{
//	if (this == &other) return *this;
//
//	this->blockType = other.blockType;
//	for (int i = 0; i < 4; i++) 
//	{
//		this->smallBlocks[i] = other.smallBlocks[i];
//	}
//
//	return *this;
//}


void Block::draw(int leftMargin, int topMargin)
{
	for (int i = 0; i < 4; i++) {
		int x = smallBlocks[i].col * size + leftMargin;
		int y = smallBlocks[i].row * size + topMargin;
		putimage(x, y, img);
	}
}

void Block::drop()
{
	for (auto& block : smallBlocks)
	{
		block.row++;
	}
}

void Block::rotate()
{
	Point p = smallBlocks[1];

	for (int i = 0; i < 4; i++) 
	{
		Point tmp = smallBlocks[i];
		smallBlocks[i].col = p.col - tmp.row + p.row;
		smallBlocks[i].row = p.row + tmp.col - p.col;
	}
}

IMAGE** Block::getImages()
{
	return imgs;
}

int Block::getBlockType()
{
	return 	 blockType;
}

void Block::solidify(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++) 
	{
		// ���ñ�ǣ����̻�����Ӧλ��
		map[smallBlocks[i].row][smallBlocks[i].col] = blockType;
	}
}

void Block::moveLeftRight(int offset)
{
	for (int i = 0; i < 4; i++) 
	{
		smallBlocks[i].col += offset;
	}
}


bool Block::blockInMap(const vector<vector<int>>& map)
{
	int rows = map.size();
	int cols = map[0].size();

	for (int i = 0; i < 4; i++)
	{
		if (smallBlocks[i].col<0 || smallBlocks[i].col>=cols
			|| smallBlocks[i].row<0 || smallBlocks[i].row>=rows			
			)
		{
			return false;
		}
		else if (map[smallBlocks[i].row][smallBlocks[i].col])
		{
			return false;
		}
	}

	return true;
}
