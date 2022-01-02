#include"Header.h"
#include <iostream>
#include <Windows.h>
#include <time.h>

///////////////////BLOCK////////////////////
Block::Block()
{
	SetBlock();
};
void Block::SetBlock()
{
	color = 1 + rand() % 7;
	type = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		X[i] = Game::BlockType[type][i] % 2 + 4;
		Y[i] = Game::BlockType[type][i] / 2;
	}
}
void Block::SetRotate(bool rotate)
{
	this->rotate = rotate;
}
void Block::SetDx(short dx)
{
	this->dx = dx;
}
void Block::MoveDxBlock()
{
	for (int i = 0; i < 4; i++)
		X[i] += dx;
}
void Block::MoveDown()
{
	for (int i = 0; i < 4; i++)
		Y[i] += 1;
}
void Block::Rotation()
{
	if (!rotate) return;

	int	tmpX = X[1];
	int	tmpY = Y[1];

	for (int i = 0; i < 4; i++) //реализаци€ формулы дл€ вращени€ в координтах относительно конкретной точки
	{
		int x = Y[i] - tmpY;
		int y = X[i] - tmpX;
		X[i] = tmpX - x;
		Y[i] = tmpY + y;
	}
	for (int i = 0; i < 4; i++)
	{
		if (X[i] < 0)
		{
			for (int j = 0; j < 4; j++) {
				X[j] += 1;
			}
			break;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (X[i] >= Field::length)
		{
			for (int j = 0; j < 4; j++) {
				X[j] -= 1;
			}
			break;
		}
	}
}
short Block::GetY(short i)
{
	return Y[i];
}
short Block::GetX(short i)
{
	return X[i];
}
short Block::GetType()
{
	return type;
}
short Block::GetColor()
{
	return color;
}
Block& Block::operator=(const Block& tmp)
{
	this->color = tmp.color;
	this->type = tmp.type;
	for (int i = 0; i < 4; i++)
	{
		this->X[i] = tmp.X[i];
		this->Y[i] = tmp.Y[i];
	}
	return *this;
}

///////////////////FIELD////////////////////
short Field::GetField(short i, short j)
{
	return field[i][j];
}
void Field::SetField(Block& block)
{
	for (int i = 0; i < 4; i++)
		field[block.GetY(i) - 1][block.GetX(i)] = block.GetColor();
}
void Field::SetField(short lineNumber, short j, short i)
{
	field[lineNumber][j] = field[i][j];
}

///////////////////TIMER////////////////////
void Timer::AddTime()
{
	this->time += this->getElapsedTime().asSeconds();
	this->sf::Clock::restart();
}
void Timer::ResetTime()
{
	this->time = 0;
}
void Timer::SetDelay()
{
	this->delay = this->cur_delay;
}
void Timer::SetDelay(double time)
{
	this->delay = time;
}
void Timer::CurDelay(double time)
{
	this->cur_delay = time;
}
bool Timer::RequiredTime()
{
	if (this->time > this->delay) return true;
	return false;
}
double Timer::GetDelay()
{
	return delay;
}
double Timer::GetTime()
{
	return this->time;
}

///////////////////GAME////////////////////
short Game::BlockType[7][4] = {
	1,3,5,7, // ѕр€ма€ лини€
	2,4,5,7, // ћолни€ вправо
	3,5,4,6, // ћолни€ влево
	3,5,4,7, // буква “
	2,3,5,7, // Ћини€ с крючком влево
	3,5,7,6, // Ћини€ с крючком вправо
	2,3,4,5, //  вадратик
};
int Game::GetScore()
{
	return Score;
}
void Game::SetField(Block& block)
{
	this->field.SetField(block);
}
void Game::SetDelay()
{
	this->timer.SetDelay();
}
void Game::SetDelay(double time)
{
	this->timer.SetDelay(time);
}
void Game::AddTime()
{
	this->timer.AddTime();
}
void Game::ResetTime()
{
	this->timer.ResetTime();
}
void Game::DeleteLine()
{
	short LineCnt = 0;
	short lineNumber = Field::height - 1;
	for (int i = Field::height - 1; i >= 0; i--)
	{
		short cnt = 0;
		for (int j = 0; j < Field::length; j++)
		{
			if (this->field.GetField(i, j)) cnt++;    //подсчитываетс€ количество блоков в линии
			this->field.SetField(lineNumber, j, i); //в k-ую строку переписываетс€ i-а€, изначально i = k,
		}
		if (cnt < Field::length)
		{
			lineNumber--;  //пока строка не полностью пуста€ k уменьшаетс€ как и i
			 //но, если строка полна€ k не опуститс€ как i, и начнетс€ перезапись верхних строк в нижнюю
		}
		else
		{
			LineCnt++;
			Lines++;
		}
	}
	switch (LineCnt)
	{
	case 0:break;
	case 1:Score += 100; break;
	case 2:Score += 300; break;
	case 3:Score += 700; break;
	case 4:Score += 1500; break;
	}
	if (LineCnt)
		switch (Lines / 10)
		{
		case 1:this->timer.CurDelay(0.37); break;
		case 2:this->timer.CurDelay(0.34); break;
		case 3:this->timer.CurDelay(0.31); break;
		case 4:this->timer.CurDelay(0.28); break;
		case 5:this->timer.CurDelay(0.25); break;
		case 6:this->timer.CurDelay(0.22); break;
		case 7:this->timer.CurDelay(0.18); break;
		case 8:this->timer.CurDelay(0.15); break;
		case 9:this->timer.CurDelay(0.11); break;
		case 10:this->timer.CurDelay(0.08); break;
		default: break;
		}
}
short Game::GetField(short i, short j)
{
	return this->field.GetField(i, j);
}
short Game::GetLines()
{
	return Lines;
}
bool Game::check(Block& tmp)
{
	for (int i = 0; i < 4; i++) //проверка всех 4 кубиков блока (тетрамино)
	{
		if (tmp.GetX(i) < 0 || tmp.GetX(i) >= Field::length || tmp.GetY(i) >= Field::height) return 0;
		else if (this->field.GetField(tmp.GetY(i), tmp.GetX(i))) return 0;
	}
	return  1;//в другом случае движение возможно
}
bool Game::RequiredTime()
{
	return this->timer.RequiredTime();
}
bool Game::EndGame()
{
	for (int i = 0; i < Field::length; i++)
		if (this->field.GetField(3, i)) return true;
	return false;
}
double Game::GetDelay()
{
	return this->timer.GetDelay();
}