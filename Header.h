#ifndef HEADER_H
#define HEADER_H

#include <Math.h>
#include <iostream>
#include <string> 

#include "SFML/Graphics.hpp"

#define FPS 120
#define FallAcceleration 1000.0
#define BonusMoveSpeed 150.0
#define FieldSize 10

#define GemSize 64

using namespace sf;

enum GemType
{
	coal,
	iron,
	gold,
	glowstone,
	redstone,
	diamond,
	magma,
	obsidian  
};

class Gem;
class Bonus;

class FieldManager
{
public:
	FieldManager();
	~FieldManager();

	void FixField();
	void GemFall(int x_t, int y_t);

	void ToDispCoords(int x, int y, int* new_x, int* new_y);
	void DeleteGem(Gem* gem);
	void DeleteBonus(int i, Bonus* bonus);
	void DeleteAnimations();
	void DeleteBonuses();

	Gem* getGem(int i, int j);
	void setGem(int i, int j, Gem* gem);
	Bonus* getBonus(int i);
	void setBonus(int i, Bonus* bonus);

	int PlayerScore = 0;

	Gem*** GemsField;
	Bonus** BonusMass;
};

class Gem
{
private:
	Texture tex;
	Image img;
	Sprite GemTex;
	GemType type;
	int score;

public:

	double display_x, display_y;
	int mat_x, mat_y;
	int size = 64;
	double speedx = 0; // with falling acceleration
	double speedy = 0; //constant

	   	 
	Gem() {};
	Gem(int pos_x, int pos_y, int m_x, int m_y, GemType gemtype);
	~Gem() {};

	void Move(int startx, int starty);
	void DrawGem(RenderWindow *window);
	int GetScore();
	GemType getType();
	void setType(GemType newtype);
	void changeImage();
};

enum BonusType
{
	bomb,
	colorchange,
	extrapoints
};

class Bonus
{
public:
	Texture tex;
	Image img;
	Sprite BonusTex;
	int score;

	Bonus() {};
	Bonus(double pos_x, double pos_y, int m_x, int m_y);
	~Bonus() {};

	void bombCreator(int m_x, int m_y);
	void colorchangeCreator(int m_x, int m_y);
	void extrapointsCreator(int m_x, int m_y);

	BonusType getType();
	void ProcessImg();

	int GetScore();
	void DrawBonus(RenderWindow *window);
	void BonusMove(int startx, int starty);
	virtual void MakeAction(FieldManager* FieldManager) = 0;

	double display_x, display_y;
	int mat_x, mat_y;
	int bonussize = 64;
	double speedx = 0.0; 
	double speedy = 0.0; 
};

class BonusBomb : public Bonus {
public:
	BonusBomb(double pos_x, double pos_y, int m_x, int m_y) : Bonus(pos_x, pos_y, m_x, m_y) {
		img.loadFromFile("resourses/textures/fireball.png");
		ProcessImg();
		do {
			mat_x = m_x + 2 * (rand() % 3 + 1) - 3;
			mat_y = m_y + 2 * (rand() % 3 + 1) - 3; //rand gem in radius of 3 cells
		} while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);
		score = 0;
	}

	virtual void MakeAction(FieldManager* FieldManager) {
		if (FieldManager->GemsField[mat_x][mat_y] != NULL)
		{
			FieldManager->DeleteGem(FieldManager->GemsField[mat_x][mat_y]);
		}
	}
};

class BonusColorChange : public Bonus {
public:
	BonusColorChange (double pos_x, double pos_y, int m_x, int m_y) : Bonus(pos_x, pos_y, m_x, m_y) {
		img.loadFromFile("resourses/textures/ender_pearl.png");
		ProcessImg();
		do {
			mat_x = m_x + 2 * (rand() % 3 + 1) - 3;
			mat_y = m_y + 2 * (rand() % 3 + 1) - 3; //rand gem in radius of 3 cells
		} while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);
		score = 0;
	}

	virtual void MakeAction(FieldManager* FieldManager) {
		FieldManager->GemsField[mat_x][mat_y]->setType((GemType)(rand() % 7));
		FieldManager->GemsField[mat_x][mat_y]->changeImage();
	}
};

class BonusExtraPoints : public Bonus {
public:
	BonusExtraPoints(double pos_x, double pos_y, int m_x, int m_y) : Bonus(pos_x, pos_y, m_x, m_y) {
		img.loadFromFile("resourses/textures/extra_points.png");
		ProcessImg();
		mat_x = m_x - 1;
		mat_y = m_y; //one up
		score = 200;
	}

	virtual void MakeAction(FieldManager* FieldManager) {
		FieldManager->PlayerScore += GetScore();
	}
};


struct SelectedGems
{
	int Fgemx = -1, Fgemy = -1;
	int Sgemx = -1, Sgemy = -1;
};



class FieldRender
{
public:
	FieldRender() {};
	~FieldRender() {};
	
	void ToMatCoords(int disp_x, int disp_y, int* mat_x, int* mat_y);
	   
	void FieldUpdate(FieldManager *Matrix); // values
	void PrintGems(FieldManager *Matrix, RenderWindow *window);  // draw
	void PrintBonuses(FieldManager *Matrix, RenderWindow *window); // draw
	bool AnimationPlaying(FieldManager *Matrix);
	bool LineCheck(FieldManager *Matrix);
	bool CheckRepeat( Gem **mass, int num, Gem* gem);

	void Swap(FieldManager *Matrix, SelectedGems *selected);
};

void GameProcess(FieldManager *Matrix, FieldRender *Field, RenderWindow *window);
void Actions(FieldManager *Matrix, FieldRender *Field, RenderWindow *window, SelectedGems *Selected, bool *Gameover, RectangleShape *rect);
void DeathScreen(FieldManager *Matrix, FieldRender *Field, RenderWindow *window);
void MainMenu(FieldManager *Matrix, FieldRender *Field, RenderWindow *window);

#endif HEADER_H