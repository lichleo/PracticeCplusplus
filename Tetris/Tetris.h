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

	//������һ�ε��ã�����˶��ٺ��룬��һ�η���0
	int getDelay();
	void moveLeftRight(int offset);
	void rotate(); //��ת
	void drop();
	void clearLine();

	void drawScore();
	bool isGameOver();
	void GameOver();

private:
	int delay;
	bool update;
	bool pause;

	vector<vector<int>> map;   //0,�հ�û����  5����5�ַ���
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	;
	Block bakBlock;
	Block* curBlock;
	Block* nextBlock; //����Ԥ��

	int score;
};