
#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>
#include <string>
#include <queue>
#include "game_types.h"

class Level
{
	public:
		enum BLOCKS { EMPTY, BLOCKED };
	protected:
		BLOCKS* MAP;

		int X;
		int Y;
		game::COORDS Spawn;
		std::queue<game::COORDS> FoodList;

	public:
		Level();
		game::COORDS SpawnCOORDS();
		int lenX();
		int lenY();
		game::COORDS CurrentFood();
		bool FoodLeft();
		void Eaten();
		bool Load(std::string level_file);
		void Unload();
		BLOCKS get(int x, int y);
		~Level();
};

#endif