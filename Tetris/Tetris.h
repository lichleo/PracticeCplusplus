#pragma once
#include "Block.h"
#include <vector>
#include <graphics.h>
#include <time.h>

using namespace std;



class Tetris
{
public:
	Tetris(int rows, int cols, int leftMargin, int topMargin, int blockSize);
	void play();

private:
	void init();

	void keyEvent();
	void updateWindow();

	//距离上一次调用，间隔了多少毫秒，第一次返回0
	int getDelay();
	void moveLeftRight(int offset);
	void rotate(); //旋转
	void drop();
	void clearLine();

	void drawScore();
	bool isGameOver();
	void GameOver();

private:
	int delay;
	bool update;
	bool pause;

	vector<vector<int>> map;   //0,空白没方块  5：第5种方块
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	;
	Block bakBlock;
	Block* curBlock;
	Block* nextBlock; //方块预告

	int score;
};