#include "Header.h"



FieldManager::FieldManager()
{
	PlayerScore = 0;

	BonusMass = new Bonus*[FieldSize*FieldSize];

	GemsField = new Gem**[FieldSize];

	for (int i = 0; i < FieldSize; i++)
	{
		GemsField[i] = new Gem*[FieldSize];
		for (int j = 0; j < FieldSize; j++)
		{
			GemsField[i][j] = NULL;
			BonusMass[i*FieldSize + j] = NULL;
		}

	}
}

FieldManager::~FieldManager()
{
	for (int i = 0; i < FieldSize; i++)
	{
		delete[] this->GemsField[i];
	}
	delete[] GemsField;

	delete[] BonusMass;
}

void FieldManager::ToDispCoords(int x, int y, int* new_x, int* new_y)
{
	*new_x = GemSize + GemSize * x + GemSize / 2;
	*new_y = GemSize + GemSize * y + GemSize / 2;
	return;
}

void FieldManager::GemFall(int x_t, int y_t)
{
	int i = x_t + 1;

	while (i < FieldSize && GemsField[i][y_t] == NULL)
	{
		i++;
	}

	i -= (x_t + 1);  //Fall while you can

	if (i > 0)
	{
		GemsField[i + x_t][y_t] = GemsField[x_t][y_t];
		GemsField[i + x_t][y_t]->mat_x = i + x_t;
		GemsField[i + x_t][y_t]->mat_y = y_t;
		GemsField[x_t][y_t] = NULL;
	}

	return;
}

void FieldManager::FixField()
{
	int i = 0;
	int new_x, new_y;
	GemType new_type;

	for (i = FieldSize - 2; i >= 0; i--)
	{
		for (int j = 0; j < FieldSize; j++)
		{
			if (GemsField[i][j] != NULL)
				GemFall(i, j);
		}
	}

	for (i = FieldSize - 1; i >= 0; i--)
	{
		for (int j = 0; j < FieldSize; j++)
		{
			if (GemsField[i][j] == NULL)
			{
				//ToDispCoords(-(FieldSize - i)*size / 32 - j * size / 32, j, &new_x, &new_y);        //TODO: ADD RAND
				ToDispCoords(-GemSize/16, j, &new_x, &new_y);
				new_type = (GemType)(rand() % 7);
				GemsField[i][j] = new Gem(new_x, new_y, i, j, new_type);
				GemsField[i][j]->speedx = 3.0*BonusMoveSpeed*(1.0 - (double)(j)/20.0 );
			}
		}
	}

	//printf("\n Fix Complete!\n");

	return;
}

void FieldManager::DeleteGem(Gem* gem)
{
	int x = gem->mat_x;
	int y = gem->mat_y;

	if (rand() % 100 >= 65)	//creating bonus
	{
		int i = 0;
		BonusType new_type = (BonusType)(rand() % 3);
		//printf("%i\n", new_type);

		while (i < FieldSize*FieldSize && BonusMass[i] != NULL)
			i++;


		if (new_type == bomb)
			for (int j = 0; j < 4; j++)														
				BonusMass[i] = new BonusBomb(gem->display_x, gem->display_y, gem->mat_x, gem->mat_y);
		else if(new_type == colorchange)
			BonusMass[i] = new BonusColorChange(gem->display_x, gem->display_y, gem->mat_x, gem->mat_y);
		else
			BonusMass[i] = new BonusExtraPoints(gem->display_x, gem->display_y, gem->mat_x, gem->mat_y);
	}

	Gem* t = GemsField[gem->mat_x][gem->mat_y];
	delete t;
	GemsField[x][y] = NULL;
	FixField();
	return;
}

void FieldManager::DeleteBonus(int i, Bonus* bonus)
{
	bonus->MakeAction(this);
	/*switch (bonus->getType())
	{
	case bomb:
	{
		if (GemsField[bonus->mat_x][bonus->mat_y] != NULL)
		{
			DeleteGem(GemsField[bonus->mat_x][bonus->mat_y]);
		}
		break;
	}
	case colorchange:
	{
		GemsField[bonus->mat_x][bonus->mat_y]->setType((GemType)(rand() % 7));
		GemsField[bonus->mat_x][bonus->mat_y]->changeImage();
		break;
	}
	case extrapoints:
	{
		PlayerScore += bonus->GetScore();
		break;
	}
	}
	*/
	delete bonus;
	BonusMass[i] = NULL;
	return;
}

void FieldManager::DeleteBonuses()
{
	for (int i = 0; i < FieldSize; i++)
	{
		for (int j = 0; j < FieldSize; j++)
		{
			if (BonusMass[i*FieldSize + j] != NULL)
			{
				delete BonusMass[i*FieldSize + j];
				BonusMass[i*FieldSize + j] = NULL;
			}

		}
	}
}

void FieldManager::DeleteAnimations()
{
	int _x, _y;

	for (int i = 0; i < FieldSize; i++)
	{
		for (int j = 0; j < FieldSize; j++)
		{

			if (BonusMass[i*FieldSize + j] != NULL)
			{
				ToDispCoords(BonusMass[i*FieldSize + j]->mat_x, BonusMass[i*FieldSize + j]->mat_y, &_x, &_y);
				BonusMass[i*FieldSize + j]->display_x = _x;
				BonusMass[i*FieldSize + j]->display_y = _y;
			}
		}
	}
}

Gem* FieldManager::getGem(int i, int j)
{
	return GemsField[i][j];
}

Bonus* FieldManager::getBonus(int i)
{
	return BonusMass[i];
}

void FieldManager::setBonus(int i, Bonus *bonus)
{
	BonusMass[i] = bonus;
}

void FieldManager::setGem(int i, int j, Gem* gem)
{
	GemsField[i][j] = gem;
}