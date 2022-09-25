#include "Header.h"

Gem::Gem(int pos_x, int pos_y, int m_x, int m_y, GemType gemtype )
{
	type = gemtype;
	score = 10 * type;
	display_x = (double)pos_x;
	display_y = (double)pos_y;
	mat_x = m_x;
	mat_y = m_y;

	changeImage();
};

void Gem::DrawGem(RenderWindow *window)
{
	if (this != NULL)
	{
		GemTex.setPosition( (float)(display_y - size / 2), (float)(display_x - size / 2) );

		if (display_x - size / 2 >= 0)
		{
			window->draw(this->GemTex);
		}
	}
		
	return;
}

int Gem::GetScore()
{
	return score;
}

GemType Gem::getType()
{
	return type;
}

void Gem::setType(GemType newtype)
{
	type = newtype;
}

void Gem::changeImage()
{
	String names[8] = {"coal", "iron", "gold", "glowstone", "redstone", "diamond", "magma", "obsidian" };

	img.loadFromFile("resourses/textures/" + names[(int)(type)] + ".png");
	
	if (type != obsidian)
	{
		img.createMaskFromColor(img.getPixel(0, 0));
		tex.loadFromImage(img);
	}
	
	GemTex.setTexture(tex, true);
}

void Gem::Move(int startx, int starty)
{
	double edgex, edgey;
	double speed_sign;
	int around_point = size / 8;

	edgex = (double)(size / 2 + size * (mat_x)+startx);
	edgey = (double)(size / 2 + size * (mat_y)+starty);


	if (abs(display_x - edgex) <= around_point)
	{
		display_x = edgex;
		speedx = 0.0;
	}
	else
	{
		speed_sign = (edgex - display_x) / fabs(display_x - edgex);
		display_x += (speedx / (double)FPS);
		speedx += speed_sign * FallAcceleration / FPS;
	}

	if (abs(display_y - edgey) <= around_point)
	{
		display_y = edgey;
		speedy = 0.0;
	}
	else
	{
		speed_sign = (edgey - display_y) / fabs(display_y - edgey);
		display_y += (speedy / (double)FPS);
		//speedy += speed_sign * FallAcceleration / FPS;
	}

}