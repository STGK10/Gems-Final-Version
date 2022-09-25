#include "Header.h"


void FieldRender::ToMatCoords(int disp_x, int disp_y, int* mat_x, int* mat_y)
{
	*mat_y = (disp_x - GemSize) / GemSize;
	*mat_x = (disp_y - GemSize) / GemSize;
	return;
}

void FieldRender::FieldUpdate(FieldManager *Matrix)
{	   
	//GemsUpdate
	for (int i = 0; i < FieldSize; i++)
	{
		for (int j = 0; j < FieldSize; j++)
		{
			if (Matrix->getGem(i, j) != NULL)
			{
				Matrix->getGem(i, j)->Move(GemSize, GemSize);
			}
		}
	}

	//BonusesUpdate
	for (int i = 0; i < FieldSize*FieldSize; i++)
	{
		if (Matrix->getBonus(i) != NULL)
		{
			Matrix->getBonus(i)->BonusMove(GemSize, GemSize);

			if (Matrix->getBonus(i)->speedx == 0.0 && Matrix->getBonus(i)->speedy == 0.0)
			{
				Matrix->DeleteBonus(i, Matrix->getBonus(i));
				Matrix->setBonus(i, NULL);
			}
		}
	} 	 
	return;
}

void FieldRender::PrintGems(FieldManager *Matrix, RenderWindow *window)
{
	for (int i = FieldSize-1; i >=0; i--)
	{
		for (int j= FieldSize - 1; j >= 0; j--)
		{
			Matrix->getGem(i, j)->DrawGem(window);
		}
	}

	return;
}

void FieldRender::PrintBonuses(FieldManager *Matrix, RenderWindow *window)
{
	for (int i = FieldSize - 1; i >= 0; i--)
	{
		for (int j = FieldSize - 1; j >= 0; j--)
		{
			Matrix->getBonus(i*FieldSize + j)->DrawBonus(window);
		}
	}

	return;
}

void FieldRender::Swap(FieldManager *Matrix, SelectedGems *selected)
{
	Gem *temp, *F, *S;
	
	F = Matrix->getGem(selected->Fgemx, selected->Fgemy);
	S = Matrix->getGem(selected->Sgemx, selected->Sgemy);

	F->mat_x = selected->Sgemx;
	F->mat_y = selected->Sgemy;

	S->mat_x = selected->Fgemx;
	S->mat_y = selected->Fgemy;

	F->speedx = -(F->display_x - S->display_x) / GemSize * FPS;
	F->speedy = -(F->display_y - S->display_y) / GemSize * FPS; //TODO: kalibrowka

	S->speedx = -F->speedx;
	S->speedy = -F->speedy;					             	// Set speed
	   
	temp = Matrix->getGem(selected->Fgemx, selected->Fgemy);
	Matrix->setGem(selected->Fgemx, selected->Fgemy, Matrix->getGem(selected->Sgemx, selected->Sgemy) );
	Matrix->setGem(selected->Sgemx, selected->Sgemy, temp);

	return;
}

bool FieldRender::LineCheck(FieldManager *Matrix)
{
	//For Checking
	Gem **mass = new Gem *[FieldSize*FieldSize];
	int num = 0;

	GemType cur_gem;

	int i=0;
	int count=0;
	bool ans = false;
	for (int x = 0; x < FieldSize; x++)
	{
		for (int y = 0; y < FieldSize; y++)
		{
			cur_gem = Matrix->getGem(x,y)->getType();

			if (!CheckRepeat(mass, num, Matrix->getGem(x,y)))
			{
				continue;
			}

			//vertical
			i = 0;
			count = 0;
			while ((x + i) < FieldSize && Matrix->getGem(x + i, y)->getType() == cur_gem)
			{
				count++;
				i++;
			}

			i = 1;
			while ((x - i) >= 0 && Matrix->getGem(x - i, y)->getType() == cur_gem)
			{
				count++;
				i++;
			}

			if (count >= 3)
			{
				ans = true;
				i = 0;
				while ((x - i) >= 0 && Matrix->getGem(x - i, y)->getType() == cur_gem)
				{
					if (CheckRepeat(mass, num, Matrix->getGem(x - i, y)))
					{
						mass[num] = Matrix->getGem(x - i, y);
						num++;
					}
					i++;
				}

				i = 1;
				while ((x + i) < FieldSize && Matrix->getGem(x + i, y)->getType() == cur_gem)
				{
					if (CheckRepeat(mass, num, Matrix->getGem(x + i, y)))
					{
						mass[num] = Matrix->getGem(x + i, y);
						num++;
					}
					i++;
				}
			}

			//horizontal
			i = 0;
			count = 0;
			while ((y + i) < FieldSize && Matrix->getGem(x , y + i)->getType() == cur_gem)
			{
				count++;
				i++;
			}

			i = 1;
			while ((y - i) >= 0 && Matrix->getGem(x, y- i)->getType() == cur_gem)
			{
				count++;
				i++;
			}

			if (count >= 3)
			{
				ans = true;
				i = 0;
				while ((y - i) >= 0 && Matrix->getGem(x, y - i)->getType() == cur_gem)
				{
					if (CheckRepeat(mass, num, Matrix->getGem(x, y - i)))
					{
						mass[num] = Matrix->getGem(x, y - i);
						num++;
					}
					i++;
				}

				i = 1;
				while ((y + i) < FieldSize && Matrix->getGem(x, y + i)->getType() == cur_gem)
				{
					if (CheckRepeat(mass, num, Matrix->getGem(x, y + i)))
					{
						mass[num] = Matrix->getGem(x, y + i);
						num++;
					}
					i++;
				}
			}
		}

	}

	for (i = 0; i < num; i++)
	{
		//printf("del %i %i %i\n", mass[i]->mat_x, mass[i]->mat_y, mass[i]->getType());
		Matrix->PlayerScore += mass[i]->GetScore();
		Matrix->DeleteGem(mass[i]);
	}	
	   
	delete[] mass;

	return ans;
}

bool FieldRender::CheckRepeat(Gem **mass, int num, Gem* gem)
{
	int j;
	for (j = 0; j < num; j++)
	{
		if (j < num && mass[j] == gem)
		{
			break;
		}
	}

	if (j == num)
	{
		return true;
	}
	return false;
}



bool FieldRender::AnimationPlaying(FieldManager *Matrix)
{
	for (int i = 0; i < FieldSize; i++)
	{
		for (int j = 0; j < FieldSize; j++)
		{
			if (Matrix->getGem(i, j)->speedx != 0.0 || Matrix->getGem(i, j)->speedy != 0.0)
			{
				return true;
			}

			if (Matrix->getBonus(i*FieldSize + j) != NULL && (Matrix->getBonus(i*FieldSize + j)->speedx != 0.0 || Matrix->getBonus(i*FieldSize + j)->speedy != 0.0))
			{
				return true;
			}
		}
	}

	return false;

}