#pragma once
#include <graphics.h>
#include <time.h>
#include <vector>

using namespace std;

struct Point 
{
	int row;
	int col;
};

class Block
{
public:
	Block();
	Block(const Block& other);
	//Block& operator=(const Block& other);

	void drop();
	void moveLeftRight(int offset);
	void rotate(); //旋转
	void draw(int leftMargin, int topMargin);

	int getBlockType();
	void solidify(vector<vector<int>>& map);
	static IMAGE** getImages();

	bool blockInMap(const vector<vector<int>> &map);

private:

	int blockType;
	Point smallBlocks[4];
	IMAGE* img;

	static int size;         //块的大小
	static IMAGE* imgs[7];   //都有哪些图片
};