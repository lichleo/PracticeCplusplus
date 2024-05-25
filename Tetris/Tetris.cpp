#include "Tetris.h"

const int SPEED_NORMAL = 500; //普通速度 
const int SPEED_QUICK = 50;   //快速降落速度
const int WINDOW_WIDTH = 938;
const int WINDOW_HEIGHT = 896;

Tetris::Tetris(int rows, int cols, int leftMargin, int topMargin, int blockSize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = leftMargin;
	this->topMargin = topMargin;
	this->blockSize = blockSize;

	for (int i = 0; i < rows; i++) 
	{
		vector<int> mapRow;
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}

	this->pause = false;
	this->delay = SPEED_NORMAL;
	this->score = 0;
}

void Tetris::init()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	//loadimage(&imgBg, _T("res/bg.jpg"));

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			map[i][j] = 0;
		}
	}
}


void Tetris::play()
{
	init();

	curBlock = new Block();
	nextBlock = new Block();

	int timer = 0;
	while (1) 
	{
		keyEvent(); 

		if (!pause)
		{
			timer += getDelay();
			if (timer > delay)
			{
				timer = 0;
				drop();
				update = true;
			}

			if (update)
			{
				update = false;
				updateWindow();  //更新画面
				clearLine();     //消行

				if (isGameOver())
				{
					GameOver();
					Sleep(10000);
					break;
				}
			}
		}
	}
}


void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blockInMap(map)) 
	{
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;
	}

	delay = SPEED_NORMAL; //每下将一次，就把降落速度还原成普通速度
}

void Tetris::keyEvent()
{
	int dx = 0;
	bool rotateFlag = false;
	unsigned char ch = 0;
	ExMessage msg = { 0 };
	while (peekmessage(&msg, EX_KEY))
	{
		if (msg.message == WM_KEYDOWN)
		{
			if ((msg.vkcode == VK_UP) || (msg.vkcode == VK_DOWN) || (msg.vkcode == VK_LEFT) || (msg.vkcode == VK_RIGHT) 
				|| (msg.vkcode == VK_SPACE)
				|| (msg.vkcode == 'W') || (msg.vkcode == 'A') || (msg.vkcode == 'S') || (msg.vkcode == 'D')
				)
			{
				switch (msg.vkcode)
				{
				case VK_UP:
				case 'W':
					rotateFlag = true;
					break;
				case VK_DOWN:
				case 'S':
					delay = SPEED_QUICK; //快速降落
					break;
				case VK_LEFT:
				case 'A':
					dx = -1;
					break;
				case VK_RIGHT:
				case 'D':
					dx = 1;
					break;
				case VK_SPACE:
					pause = !pause;
					break;					
				default:
					break;
				}
			}
		}
	}

	if (dx != 0)
	{
		moveLeftRight(dx);
		update = true;
	}

	if (rotateFlag) 
	{
		rotate();
		update = true;
	}
}

void Tetris::moveLeftRight(int offset) 
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);

	if (!curBlock->blockInMap(map)) 
	{
		*curBlock = bakBlock;
	}
}

void Tetris::rotate()
{
	bakBlock = *curBlock;
	curBlock->rotate();

	if (!curBlock->blockInMap(map))
	{
		*curBlock = bakBlock;
	}
}

void Tetris::updateWindow()
{
	//putimage(0, 0, &imgBg);

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();
	cleardevice();

	//画框
	setlinecolor(WHITE);
	rectangle(leftMargin, topMargin, leftMargin + blockSize * cols, topMargin + blockSize * rows);
	rectangle(leftMargin-10, topMargin-10, leftMargin + blockSize * cols+10, topMargin + blockSize * rows+10);

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			if (map[i][j] == 0) continue;
			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}

	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(WINDOW_WIDTH-200, 150); //绘制预告方块
	rectangle(WINDOW_WIDTH - 300, 150, WINDOW_WIDTH - 20, 400);

	drawScore();

	EndBatchDraw();
}

int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (0 == lastTime)
	{
		lastTime = currentTime;
		return 0;

	}
	else
	{
		int ret = currentTime - lastTime;
		lastTime = currentTime;

		return ret;
	}
}

void Tetris::clearLine()
{
	int k = rows - 1;
	int count = 0;
	int delLine = 0;
	for (int i = rows - 1; i > 0; i--)
	{
		count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{
				count++;
			}
			map[k][j] = map[i][j];
		}
		//判断第i行不满
		if (count < cols)
		{
			k--;
		}
		else
		{
			delLine++;
		}
	}

	if (delLine > 0)
	{
		int addScore[4] = { 10, 30, 60, 100 };
		score += addScore[delLine - 1];
		update = true;
	}
}

void Tetris::drawScore()
{
	TCHAR scoreText[32];
	_sntprintf_s(scoreText,sizeof(scoreText), _T("%d"), score);

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(YELLOW);

	outtextxy(WINDOW_WIDTH - 350, 620, _T("Score:"));
	outtextxy(WINDOW_WIDTH - 350, 720, scoreText);
}


bool Tetris::isGameOver()
{
	return false == curBlock->blockInMap(map);
}

void Tetris::GameOver()
{
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 120;
	f.lfWidth = 80;
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(RED);

	BeginBatchDraw();
	outtextxy(leftMargin + 50, topMargin+50, _T("GAME OVER"));
	EndBatchDraw();
}
