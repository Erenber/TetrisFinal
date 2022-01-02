#include<fstream>
#include<SFML/Graphics.hpp>

class Field;
class Game;

class Block
{
private:
	short X[4] = { 0 };
	short Y[4] = { 0 };
	short color = 0;
	short type = 0;
	short dx = 0;
	bool rotate = false;
public:
	Block();
	void SetBlock();
	void SetRotate(bool);
	void SetDx(short);
	void MoveDxBlock();
	void MoveDown();
	void Rotation();
	short GetY(short);
	short GetX(short);
	short GetType();
	short GetColor();
	Block& operator=(const Block&);
};

class Field
{
public:
	static const short height = 24; ////высота поля
	static const short length = 10; ////ширина поля
	short GetField(short, short);
	void SetField(Block&);
	void SetField(short, short, short);
private:
	short field[height][length] = { 0 };
};

class Timer : public sf::Clock
{
private:
	double time = 0;
	double delay = 0.4;
	double cur_delay = 0.4;
public:
	void AddTime();
	void ResetTime();
	void SetDelay();
	void SetDelay(double);
	void CurDelay(double);
	bool RequiredTime();
	double GetDelay();
	double GetTime();
};

class Game
{
public:
	static short BlockType[7][4];
	int GetScore();
	void SetField(Block&);
	void SetDelay();
	void SetDelay(double);
	void AddTime();
	void ResetTime();
	void DeleteLine();// так же здесь + Score;
	short GetField(short, short);
	short GetLines();
	bool check(Block&);
	bool RequiredTime();
	bool EndGame();
	double GetDelay();
private:
	Field field;
	Timer timer;
	int Score = 0;
	short Lines = 0;
};