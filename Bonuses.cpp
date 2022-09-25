#include "Header.h"

Bonus::Bonus(double pos_x, double pos_y, int m_x, int m_y)
{
	display_x = pos_x;
	display_y = pos_y;

	/*switch (type)
	{
	case bomb: { bombCreator(m_x, m_y); break; }
	case colorchange: { colorchangeCreator(m_x, m_y); break; }
	case extrapoints: { extrapointsCreator(m_x, m_y); break; }
	}*/
};

void Bonus::ProcessImg() {
	img.createMaskFromColor(img.getPixel(0, 0));
	tex.loadFromImage(img);
	BonusTex.setTexture(tex, true);
}

void Bonus::bombCreator(int m_x, int m_y)
{
	img.loadFromFile("resourses/textures/fireball.png");
	do{
		mat_x = m_x + 2 * (rand() % 3 + 1) - 3;
		mat_y = m_y + 2 * (rand() % 3 + 1) - 3; //rand gem in radius of 3 cells
	} while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);
	score = 0;

}

void Bonus::colorchangeCreator(int m_x, int m_y)
{
	img.loadFromFile("resourses/textures/ender_pearl.png");
	do{
		mat_x = m_x + 2 * (rand() % 3 + 1) - 3;
		mat_y = m_y + 2 * (rand() % 3 + 1) - 3; //rand gem in radius of 3 cells
	} while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);
	score = 0;
}

void Bonus::extrapointsCreator(int m_x, int m_y)
{
	img.loadFromFile("resourses/textures/extra_points.png");
	mat_x = m_x-1;
	mat_y = m_y; //one up
	score = 200;
}

void Bonus::DrawBonus(RenderWindow *window)
{
	if (this != NULL)
	{
		BonusTex.setPosition((float)(display_y - bonussize / 2), (float)(display_x - bonussize / 2));

		if (display_x - bonussize / 2 >= 0)
		{
			window->draw(this->BonusTex);
		}
	}

	return;
}

void Bonus::BonusMove(int startx, int starty)
{
	double edgex, edgey;
	double speed_sign;
	double multiplyer = 1.0;
	

	edgex = (double)(bonussize / 2 + bonussize * (mat_x)+startx);
	edgey = (double)(bonussize / 2 + bonussize * (mat_y)+starty);
	
	if( (display_y - edgey )!=0 && (display_x - edgex) != 0)
		multiplyer = fabs((display_x - edgex) / (display_y - edgey));

	
	if(multiplyer<1.0 && speedx == 0)
	{
		if (display_x - edgex < 0)
		{
			speedx =  BonusMoveSpeed * 2.0;
		}
		else
		{
			speedx = -BonusMoveSpeed * 2.0;
		}

		speedx *= 1 / multiplyer;
	}
	else if (speedx == 0)
	{
		if (display_x - edgex < 0)
		{
			speedx = BonusMoveSpeed * 2.0;
		}
		else
		{
			speedx = -BonusMoveSpeed * 2.0;
		}
	}
	   

	if (multiplyer > 1.0 &&speedy == 0)
	{

		if (display_y - edgey < 0)
		{
			speedy = BonusMoveSpeed * 2.0;
		}
		else
		{
			speedy = -BonusMoveSpeed * 2.0;
		}
		speedy *= multiplyer;
	}
	else if (speedy == 0)
	{
		if (display_y - edgey < 0)
		{
			speedy = BonusMoveSpeed * 2.0;
		}
		else
		{
			speedy = -BonusMoveSpeed * 2.0;
		}
	}
	
	BonusTex.rotate( (float)3.14 / FPS);
	   	 
	display_y += (speedy * 1.0 / (double)FPS);

	display_x += (speedx * 1.0 / (double)FPS);	

	speed_sign = speedy / fabs(speedy);

	if (fabs(display_y - edgey) < bonussize / 16 || speed_sign * display_y > speed_sign*edgey)
	{
		display_y = edgey;
		speedy = 0.0;
	}

	speed_sign = speedx / fabs(speedx);

	if (fabs(display_x - edgex) < bonussize / 16 || speed_sign * display_x > speed_sign*edgex)
	{
		display_x = edgex;
		speedx = 0.0;
	}
	   
}


int Bonus::GetScore()
{
	return score;
}


