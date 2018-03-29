
#include "level.h"

//‘ункци€ перевода строки в целое число
int perevodN(const char* ch)
{
	int Int = 0;
	int len = strlen(ch);
	int noli = 1;
	for(int i = 1; i < len; i++)
	{ noli *= 10; }
	for(int i = 0; i < len; i++)
	{
		Int += (static_cast<int>(*(ch + i) - '0') * noli);
		noli /= 10;
	}
	return Int;
}

Level::Level()
{
	MAP = 0;
	X = Y = 0;
	Spawn.x = Spawn.y = 0;
}

game::COORDS Level::SpawnCOORDS()
{
	return Spawn;
}

int Level::lenX()
{
	return X;
}

int Level::lenY()
{
	return Y;
}

game::COORDS Level::CurrentFood()
{
	return FoodList.front();
}

bool Level::FoodLeft()
{
	return !FoodList.empty();
}

void Level::Eaten()
{
	FoodList.pop();
}

bool Level::Load(std::string level_file)
{
	//—читывание данных о карте
	std::fstream lfile;
	lfile.open(level_file.c_str());
	if(!lfile)
	{ return false; }

	std::string t;
	lfile >> t;
	X = perevodN(t.c_str());
	lfile >> t;
	Y = perevodN(t.c_str());
	lfile >> t;
	Spawn.x = perevodN(t.c_str());
	lfile >> t;
	Spawn.y = perevodN(t.c_str());
	lfile >> t;
	int i = perevodN(t.c_str());
	game::COORDS tFood;
	for(i; i > 0; i--) //—читывание очерЄдности по€влени€ еды
	{
		lfile >> t;
		tFood.x = perevodN(t.c_str());
		lfile >> t;
		tFood.y = perevodN(t.c_str());
		FoodList.push(tFood);
	}

	//—оздание самой карты
	MAP = new BLOCKS[X * Y];
	for(int j = 0; j < Y; j++)
	{
		lfile >> t;
		for(int z = 0; z < X; z++)
		{
			switch(t[z])
			{
				case 'b':
					MAP[z + X * j] = BLOCKS::BLOCKED;
				break;
				case 'e':
					MAP[z + X * j] = BLOCKS::EMPTY;
				break;
			}
		}
	}

	return true;
}

void Level::Unload()
{
	if(MAP)
	{
		delete [] MAP;
		MAP = 0;
	}
}

Level::BLOCKS Level::get(int x, int y)
{
	return MAP[x + X * y]; //X и х разные переменные
}

Level::~Level()
{
	Unload();
}