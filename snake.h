
#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <vector>
#include "game_types.h"
#include "level.h"

class Snake
{
	public:
		enum dir {UP, DOWN, RIGHT, LEFT};

		struct JUMP
		{
			JUMP(dir D, int t) : DIR(D), timer(t)
			{}

			dir DIR;
			int timer;
		};

	private:
		Level* level;
		int length;
		std::deque<game::COORDS> Nodes;
		std::vector<JUMP> Jumps;
		game::COORDS OldTeilNode;
		int TeilPos;
		bool IsEat; //Проверка на удлинение на предидущем ходу
		dir eDir;
		game::COORDS direction;
		bool isStoped;
	public:
		Snake(Level* l);

		//Контроль направления движения
		void Stop();
		void Up();
		void Down();
		void Right();
		void Left();

		int Length();
		game::COORDS getNode(int n);
		game::COORDS getNextPos();
		Snake::dir getDir();
		game::COORDS getDirCoord();
		game::COORDS getOldTeil();
		int getTeilNum();
		dir getJump(int);
		int getJumpTimer(int);
		int NumOfJumps();
		bool isEat();

		bool Move();
		//значение функции: true - можно делать следующий ход, false - конец (столкновение с препятствием)
		//При съедании еды змейка автоматически растёт

	private:
		game::COORDS SearchNextStep();
		void one_step();
		void feed();
};

#endif