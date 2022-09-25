#include "Header.h"

void GameProcess(FieldManager *Matrix, FieldRender *Field, RenderWindow *window)
{
	//Settings for game
	int MouseDelay=0;
	bool GameOver = false;
	struct SelectedGems *Selected;
	Selected = new SelectedGems;	
	
	//Rectangle selected
	RectangleShape rect;
	rect.setSize(Vector2f(64, 64));
	rect.setOutlineColor(Color::Black);
	rect.setFillColor(Color::Yellow);
	rect.setOutlineThickness(2);
	rect.setPosition(1000, 2000);

	//Sprite Background
	Texture backTexture;
	backTexture.loadFromFile("resourses/textures/Wallpaper2.png");
	Sprite background(backTexture);
	
	//Text player score
	Font font;
	font.loadFromFile("resourses/Minecraft_Font.ttf");
	Text PlayerScore("", font, 25);
	PlayerScore.setFillColor(Color::Yellow);
	PlayerScore.setPosition(960, 100);
	 

	while (Field->LineCheck(Matrix) )
	{
		Matrix->FixField();
		Matrix->DeleteBonuses();
	} 

	//Matrix->DeleteAnimations();

	Matrix->PlayerScore=0;

	//Render
	while (window->isOpen() && !GameOver)
	{
		MouseDelay++;
		MouseDelay = MouseDelay % 60;

		window->clear();
		window->draw(background);
		window->draw(PlayerScore);
		window->draw(rect);
		PlayerScore.setString(" Score:  " + std::to_string(Matrix->PlayerScore));

		Field->FieldUpdate(Matrix);
		Field->PrintGems(Matrix, window);
		Field->PrintBonuses(Matrix, window);

		if (MouseDelay % (FPS/10) == 0 )
		{
			if (!Field->AnimationPlaying(Matrix) )
			{
				if (!Field->LineCheck(Matrix) )
				{
					if (Selected->Sgemx != -1)
					{
	
						Field->Swap(Matrix, Selected);
						Selected->Sgemx = Selected->Fgemx = -1;
						Selected->Sgemy = Selected->Fgemy = -1;

					}
					Actions(Matrix, Field, window, Selected, &GameOver, &rect);
				}
				else
				{
					Selected->Sgemx = Selected->Fgemx = -1;
					Selected->Sgemy = Selected->Fgemy = -1;
				}				
			}
		}
				
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		window->display();
	}

	delete Selected;							
	return;
}

void Actions(FieldManager *Matrix, FieldRender *Field, RenderWindow *window, SelectedGems *Selected, bool *GameOver, RectangleShape *rect)
{
	int disp_x, disp_y;
	int *mat_x, *mat_y;
	mat_x = new int;
	mat_y = new int;

	   	
//	std::cout << "X: " << sf::Mouse::getPosition(*window).x << "\tY: " << sf::Mouse::getPosition(*window).y << "\n";

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		disp_x = sf::Mouse::getPosition(*window).x;
		disp_y = sf::Mouse::getPosition(*window).y;


		if (disp_x >= GemSize && disp_x <= FieldSize * GemSize + GemSize
			&& disp_y >= GemSize && disp_y <= FieldSize * GemSize + GemSize)
		{
			Field->ToMatCoords(disp_x, disp_y, mat_x, mat_y);
			if(Selected->Fgemx!=-1)
			{
				if (*mat_x != Selected->Fgemx || *mat_y != Selected->Fgemy)
				{
					if ((abs(*mat_x - Selected->Fgemx) == 1 && Selected->Fgemy == *mat_y) ||
						(abs(*mat_y - Selected->Fgemy) == 1 && Selected->Fgemx == *mat_x))
					{
						Selected->Sgemx = *mat_x;
						Selected->Sgemy = *mat_y;
						Field->Swap(Matrix, Selected);
						printf("Selected second\n");						
					}
					else
					{
						Selected->Sgemx = Selected->Fgemx = -1;
						Selected->Sgemy = Selected->Fgemy = -1;
					}
					rect->setPosition(1000, 2000);
				}
			}
			else
			{
				Selected->Fgemx = *mat_x;
				Selected->Fgemy = *mat_y;
				rect->setPosition( (float)(disp_x - disp_x%64), (float)(disp_y - disp_y % 64));
				printf("Selected first\n");
			}
		}

		if (disp_x >= 879 && disp_x <= 1091 && disp_y >= 674 && disp_y <= 713)
		{
			*GameOver = true;
		}
	}
		
	/*
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))   //TODO: Delete
	{
		disp_x = sf::Mouse::getPosition(*window).x;
		disp_y = sf::Mouse::getPosition(*window).y;
		
		if (disp_x >= Matrix->startx && disp_x <= (Matrix->FieldSize * Matrix->getSize() + Matrix->startx)
			&& disp_y >= Matrix->starty && disp_y <= (Matrix->FieldSize * Matrix->getSize() + Matrix->starty) )
		{
			Matrix->ToMatCoords(disp_x, disp_y, mat_x, mat_y);
			//printf("%i %i\n", *mat_x, *mat_y);
			Matrix->DeleteGem(Matrix->GemsField[ *mat_x][ *mat_y]);
			Matrix->FixField();
		}
	}
	*/

	return;
}

void DeathScreen(FieldManager *Matrix, FieldRender *Field, RenderWindow *window)
{

	//Background
	Texture backTexture;
	backTexture.loadFromFile("resourses/textures/DeathScreen.png");
	Sprite background;
	background.setTexture(backTexture);
	
	//PlayerScore
	Font font;
	font.loadFromFile("resourses/Minecraft_Font.ttf");
	Text PlayerScore("", font, 25);
	PlayerScore.setFillColor(Color::White);
	PlayerScore.setPosition(550, 150);
	PlayerScore.setString(" Score:  " + std::to_string(Matrix->PlayerScore));
	
	//Restart
	bool Restart = false;
	int disp_x;
	int disp_y;

	while (window->isOpen() && !Restart)
	{
		window->clear();
		window->draw(background);
		window->draw(PlayerScore);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))   
		{
			disp_x = sf::Mouse::getPosition(*window).x;
			disp_y = sf::Mouse::getPosition(*window).y;

			if (disp_x >= 950 && disp_x <= 1250 && disp_y >= 600 && disp_y <= 700)
			{
				Restart = true;
			}

		}
			   	
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		window->display();
	}
}

void MainMenu(FieldManager *Matrix, FieldRender *Field, RenderWindow *window)
{
	//Background
	Texture backTexture;
	backTexture.loadFromFile("resourses/textures/MainMenu.png");
	Sprite background;
	background.setTexture(backTexture);		//443 303 839 343
	   
	int disp_x;
	int disp_y;

	while (window->isOpen())
	{
		window->clear();
		window->draw(background);

		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))   
		{
			disp_x = sf::Mouse::getPosition(*window).x;
			disp_y = sf::Mouse::getPosition(*window).y;

			if (disp_x >= 500 && disp_x <= 800 && disp_y >= 530 && disp_y <= 590)
			{
				break;
			}

		}
		window->display();
	}
	
	return;
}
