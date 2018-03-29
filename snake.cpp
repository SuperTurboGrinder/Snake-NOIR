
#include "snake.h"

Snake::Snake(Level* l)
{
	level = l;
	Nodes.push_back(level->SpawnCOORDS());
	isStoped = true;
	length = 1;
	direction.x = Nodes[0].x;
	direction.y = Nodes[0].y;
	OldTeilNode = level->SpawnCOORDS();
	TeilPos = 1;
	Jumps.clear();
}

void Snake::Stop()
{
	isStoped = true;
}

void Snake::Up()
{
	if(isStoped)
	{ isStoped = false; }
	
	game::COORDS t(Nodes[0]);
	t.y--;
	if((length == 1) || ((Nodes[1].x != t.x) && (Nodes[1].y != t.y)))
	{
		if(t.y < 0)
		{
			t.y = level->lenY() - 1;
			Jumps.push_back(JUMP(UP, TeilPos + 1));
		}
		direction = t;
		eDir = UP;
	}
}

void Snake::Down()
{
	if(isStoped)
	{ isStoped = false; }
	
	game::COORDS t(Nodes[0]);
	t.y++;
	if((length == 1) || ((Nodes[1].x != t.x) && (Nodes[1].y != t.y)))
	{
		if(t.y >= level->lenY())
		{
			t.y = 0;
			Jumps.push_back(JUMP(DOWN, TeilPos + 1));
		}
		direction = t;
		eDir = DOWN;
	}
}

void Snake::Right()
{
	if(isStoped)
	{ isStoped = false; }
	
	game::COORDS t(Nodes[0]);
	t.x++;
	if((length == 1) || ((Nodes[1].x != t.x) && (Nodes[1].y != t.y)))
	{
		if(t.x >= level->lenX())
		{
			t.x = 0;
			Jumps.push_back(JUMP(RIGHT, TeilPos + 1));
		}
		direction = t;
		eDir = RIGHT;
	}
}

void Snake::Left()
{
	if(isStoped)
	{ isStoped = false; }

	game::COORDS t(Nodes[0]);
	t.x--;
	if((length == 1) || ((Nodes[1].x != t.x) && (Nodes[1].y != t.y)))
	{
		if(t.x < 0)
		{
			t.x = level->lenX() - 1;
			Jumps.push_back(JUMP(LEFT, TeilPos + 1));
		}
		direction = t;
		eDir = LEFT;
	}
}

int Snake::Length()
{
	return length;
}

game::COORDS Snake::getNode(int n)
{
	return Nodes[n];
}

game::COORDS Snake::getNextPos()
{
	return direction;
}

Snake::dir Snake::getDir()
{
	return eDir;
}

game::COORDS Snake::getDirCoord()
{
	return direction;
}

game::COORDS Snake::getOldTeil()
{
	return OldTeilNode;
}

int Snake::getTeilNum()
{
	return TeilPos;
}

Snake::dir Snake::getJump(int i)
{
	return Jumps[i].DIR;
}

int Snake::getJumpTimer(int i)
{
	return Jumps[i].timer;
}

int Snake::NumOfJumps()
{
	return Jumps.size();
}

bool Snake::isEat()
{
	return IsEat;
}

bool Snake::Move()
{
	if(!isStoped)
	{
		IsEat = false;
		if(level->get(direction.x, direction.y) == level->BLOCKS::BLOCKED)
		{ return false; }
		else if( (direction.x == (level->CurrentFood()).x) && (direction.y == (level->CurrentFood()).y) )
		{
			feed();

			if( (TeilPos < 4) && (TeilPos < length - 1) )
			{ TeilPos++; }
			OldTeilNode = getNode(TeilPos);

			level->Eaten();
			IsEat = true;
			return true;
		}
		else
		{
			if(length == 1)
			{ feed(); }
			else
			{
				if( (TeilPos < 4) && (TeilPos < length - 1) )
				{ TeilPos++; }
				OldTeilNode = getNode(TeilPos);
				one_step();
			}
			return true;
		}
	}
	return true;
}

game::COORDS Snake::SearchNextStep()
{
	game::COORDS t;
	t = direction;
	if(eDir == RIGHT)
	{
		t.x++;
		if(t.x >= level->lenX())
		{
			t.x = 0;
			Jumps.push_back(JUMP(RIGHT, TeilPos + 1));
		}
	}
	else if(eDir == LEFT)
	{
		t.x--;
		if(t.x < 0)
		{
			t.x = level->lenX() - 1;
			Jumps.push_back(JUMP(LEFT, TeilPos + 1));
		}
	}
	else if(eDir == DOWN)
	{
		t.y++;
		if(t.y >= level->lenY())
		{
			t.y = 0;
			Jumps.push_back(JUMP(DOWN, TeilPos + 1));
		}
	}
	else if(eDir == UP)
	{
		t.y--;
		if(t.y < 0)
		{
			t.y = level->lenY() - 1;
			Jumps.push_back(JUMP(LEFT, TeilPos + 1));
		}
	}
	return t;
}

void Snake::one_step()
{
	if(Jumps.size() > 0)
	{
		for(int i = 0; i < Jumps.size(); i++)
		{
			Jumps[i].timer--;
		}
		
		if(Jumps[0].timer < 0)
		{ Jumps.erase(Jumps.begin()); }
	}

	game::COORDS next = SearchNextStep();
	Nodes.pop_back();
	Nodes.push_front(direction);
	direction = next;
}

void Snake::feed()
{
	Nodes.push_front(direction);
	direction = SearchNextStep();
	length++;
}