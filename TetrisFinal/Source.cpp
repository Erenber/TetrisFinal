#include "Header.h"

int GetFile()
{
	std::ifstream fin;
	int res;
	fin.open("HighScore.bin", std::ios::binary);
	if (!fin.is_open()) return 0;
	else
	{
		fin.read((char*)&res, sizeof(int));
	}
	fin.close();
	return res;
}
void SetFile(int HighScore)
{
	std::ofstream fout;
	fout.open("HighScore.bin", std::ios::binary);
	if (!fout.is_open()) return;
	else fout.write((char*)&HighScore, sizeof(int));
	fout.close();
}

int main()
{
	srand(time(NULL));
	//double timer = 0;

	const int pixSize = 25;

	Block block, prev, next;
	Game game;

	sf::Font* font = new sf::Font;
	if (!font->loadFromFile("arial.ttf")) exit(0);
	sf::Text text_lines, text_score, text_high_score;

	text_lines.setFont(*font);
	text_lines.setCharacterSize(30);
	text_lines.setFillColor(sf::Color::Black);
	text_lines.setPosition(16.7 * pixSize, 18 * pixSize);

	text_score.setFont(*font);
	text_score.setCharacterSize(30);
	text_score.setFillColor(sf::Color::Black);
	text_score.setPosition(13 * pixSize + 10, 10 * pixSize);

	text_high_score.setFont(*font);
	text_high_score.setCharacterSize(30);
	text_high_score.setFillColor(sf::Color::Black);
	text_high_score.setPosition(13 * pixSize + 10, 14.5 * pixSize);

	char strScore[8], strLines[4], strHighScore[8];
	int HighScore = GetFile();
	_itoa_s(HighScore, strHighScore, 10);
	text_high_score.setString(strHighScore);

	sf::RenderWindow window(sf::VideoMode(pixSize * 20, pixSize * 23), "TETRIS!");
	window.setFramerateLimit(60);
	//sf::Clock clock;
	sf::Event event;

	sf::Texture startScreen;
	sf::Texture endScreen;
	sf::Texture Tiles;
	sf::Texture Background;
	startScreen.loadFromFile("startScreen.png");
	endScreen.loadFromFile("endScreen.png");
	Tiles.loadFromFile("TilesFinal.png");//вручную нарисовання картинка, с 7 разноцветными квадратиками
	Background.loadFromFile("BackgroundFinal.png");

	sf::Sprite SprBackground(Background);
	sf::Sprite SprTiles(Tiles);
	sf::Sprite StrScreen(startScreen);
	sf::Sprite EndScreen(endScreen);

	//start

	while (window.isOpen())
	{
		bool startGame = true;

		window.draw(StrScreen);
		window.display();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				return 0;
			}

			if (event.type == sf::Event::KeyPressed) startGame = false;
		}
		if (!startGame) break;
	}

	while (window.isOpen())
	{
		game.AddTime();
		// Блок взаимодействия пользователя с игрой
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return 0;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Space)
				{
					block.SetRotate(true);
				}
				else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
				{
					block.SetDx(-1);
				}
				else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
				{
					block.SetDx(1);
				}
				else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
				{
					game.SetDelay(0.001);
				}
				else if (event.key.code == sf::Keyboard::LAlt || event.key.code == sf::Keyboard::RAlt)
				{
					bool pause = true;
					while (pause)
					{
						while (window.pollEvent(event))
						{
							if (event.type == sf::Event::Closed)
							{
								window.close();
								return 0;
							}
							if (event.type == sf::Event::KeyPressed) pause = false;
						}
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				game.SetDelay(0.001);
			}
		}

		// Блок анализа состояния и логики игры
		prev = block;  //запоминание старых координат, в случае если невозможно сдвинуть
		block.MoveDxBlock(); //сдвиг 
		if (!game.check(block))block = prev;
		if (block.GetType() != 6) block.Rotation();
		if (!game.check(block)) block = prev;

		if (game.RequiredTime())
		{
			block.MoveDown();
			if (!game.check(block))
			{
				game.SetField(block);
				block = next;
				next.SetBlock();
			}
			game.ResetTime();
			if (game.EndGame())
			{
				if (game.GetScore() > HighScore)
				{
					SetFile(game.GetScore());
				}
				while (window.isOpen())
				{
					window.clear();
					window.draw(EndScreen);
					window.display();
					sf::Clock clock;
					while (clock.getElapsedTime().asSeconds() < 0.5);
					while (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed) {
							window.close();
							return 0;
						}

						if (event.type == sf::Event::KeyPressed) {
							window.close();
							return 0;
						}
					}
				}
			}
		}

		game.DeleteLine();
		block.SetDx(0);
		game.SetDelay();
		block.SetRotate(false);
		//Отрисовка
		window.clear();
		window.draw(SprBackground);
		window.draw(text_score);
		window.draw(text_lines);

		for (int i = 0; i < Field::height; i++)
		{
			for (int j = 0; j < Field::length; j++)
			{
				if (game.GetField(i, j) == 0) continue;
				SprTiles.setTextureRect(sf::IntRect(game.GetField(i, j) * pixSize, 0, pixSize, pixSize));
				SprTiles.setPosition((j + 1) * pixSize + 1, (i - 2) * pixSize + 1);
				// смещение на 1 блок из-за фона + увеличение размера поля до 25 клеток дает занчение i-2  
				window.draw(SprTiles);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (block.GetY(i) < 3) continue;
			SprTiles.setTextureRect(sf::IntRect(block.GetColor() * pixSize, 0, pixSize, pixSize));
			SprTiles.setPosition((block.GetX(i) + 1) * pixSize + 1, (block.GetY(i) - 2) * pixSize + 1);
			window.draw(SprTiles);
		}
		for (int i = 0; i < 4; i++)
		{
			SprTiles.setTextureRect(sf::IntRect(next.GetColor() * pixSize, 0, pixSize, pixSize));
			SprTiles.setPosition((next.GetX(i) + 11) * pixSize + 1, (next.GetY(i) + 3) * pixSize + 1);
			window.draw(SprTiles);
		}

		_itoa_s(game.GetLines(), strLines, 10);
		text_lines.setString(strLines);

		_itoa_s(game.GetScore(), strScore, 10);
		text_score.setString(strScore);

		if (game.GetScore() > HighScore)
		{
			//SetFile(game.GetScore());
			text_high_score.setString(strScore);
		}
		window.draw(text_high_score);
		window.display();
	}
}