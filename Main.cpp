
#include "game.h"

Level testLevel;
Snake* MySnake;
GameTimer Timer;
int Direct = -1;
int tLast = 0;
//-1 - не задано
//1 - вверх
//2 - направо
//3 - вниз
//4 - налево

void ProcessKeys(WPARAM wparam)
{
	int oldDir = Direct;
	switch(wparam)
	{
		case 0x57: //VK_W
		case VK_UP:
		{
			if(Direct == -1)
			{
				Direct = 1;
				Timer.Start();
			}
		} break;

		case 0x53: //VK_S
		case VK_DOWN:
		{
			if(Direct == -1)
			{
				Direct = 3;
				Timer.Start();
			}
		} break;

		case 0x41: //VK_A
		case VK_LEFT:
		{
			if(Direct == -1)
			{
				Direct = 4;
				Timer.Start();
			}
			else
			{ Direct--; }
		} break;

		case 0x44: //VK_D
		case VK_RIGHT:
		{
			if(Direct == -1)
			{
				Direct = 2;
				Timer.Start();
			}
			else
			{ Direct++; }
		} break;
	}

	if(Direct == 0)
	{ Direct = 4; }
	else if(Direct == 5)
	{ Direct = 1; }

	Snake::dir tDir;
	game::COORDS dir;

	if(MySnake->Length() > 1)
	{
		dir.x = MySnake->getNode(0).x - MySnake->getNode(1).x;
		dir.y = MySnake->getNode(0).y - MySnake->getNode(1).y;

		if(dir.x > 0)
		{ tDir = Snake::dir::RIGHT; }
		else if(dir.x < 0)
		{ tDir = Snake::dir::LEFT; }
		else if(dir.y > 0)
		{ tDir = Snake::dir::DOWN; }
		else if(dir.y < 0)
		{ tDir = Snake::dir::UP; }
	}

	switch(Direct)
	{
		case 1:
		{
			if( (MySnake->Length() > 1) && (tDir == Snake::dir::DOWN) )
			{ Direct = oldDir; }
			else
			{ MySnake->Up(); }
		}
		break;

		case 2:
		{
			if( (MySnake->Length() > 1) && (tDir == Snake::dir::LEFT) )
			{ Direct = oldDir; }
			else
			{ MySnake->Right(); }
		}
		break;

		case 3:
		{
			if( (MySnake->Length() > 1) && (tDir == Snake::dir::UP) )
			{ Direct = oldDir; }
			else
			{ MySnake->Down(); }
		}
		break;

		case 4:
		{
			if( (MySnake->Length() > 1) && (tDir == Snake::dir::RIGHT) )
			{ Direct = oldDir; }
			else
			{ MySnake->Left(); }
		}
		break;
	}
}

LRESULT CALLBACK MainWinProc(HWND hwnd,
							 UINT msg,
							 WPARAM wparam,
							 LPARAM lparam)
{
	switch(msg)
	{
		case WM_KEYUP:
		{
			ProcessKeys(wparam);
		} break;
		case WM_DESTROY:
		{
			// <Тут удалить объекты игры>
			PostQuitMessage(0);
			return 0;
		} break;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

//функция WinMain
//входная точка приложения

int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE hprevinstance,
				   LPSTR lpcmdline,
				   int ncmdshow)
{
	if(!testLevel.Load("test.map"))
	{ MessageBox(0, "Ошибка файла уровня", "Level::Load()", MB_OK); }

	MySnake = new Snake(&testLevel);
	MySnake->Stop();
	Timer.SetAnimTime(350);

	Game MyGame(&hinstance, &hprevinstance, &lpcmdline, &ncmdshow);
	MyGame.SetMainProc(MainWinProc);
	if(!MyGame.InitGame(1600, 900, true, "SNAKE NOIR", MySnake, &testLevel))
	{ MessageBox(MyGame.Wnd(), "Ошибка запуска игры", "Game::InitGame()", MB_OK); }

	Timer.Start();


	// <Тут инициализировать игру>

	MyGame.MsgLoop(&testLevel, MySnake, &Timer);

	return 0;
}


//#include <iostream>
//#include <conio.h>
//#include "level.h"
//#include "snake.h"
//#include "game_timer.h"
//
//int main()
//{
//	Level testLevel;
//	testLevel.Load("test.map");
//
//	Snake MySnake(&testLevel);
//	GameTimer Timer;
//	Timer.SetAnimTime(500);
//	MySnake.Up();
//	Timer.Start();
//
//	char input = 0;
//	int z;
//
//	while(input != 'q')
//	{
//		system("cls");
//		for(int y = 0; y < testLevel.lenY(); y++)
//		{
//			for(int x = 0; x < testLevel.lenX(); x++)
//			{
//				if( (x == testLevel.CurrentFood().x) && (y == testLevel.CurrentFood().y))
//				{ std::cout << char(177); }
//				else
//				{
//					for(z = 0; z <= MySnake.Length(); z++)
//					{
//						if( (z == MySnake.Length()) || ((x == MySnake.getNode(z).x) && (y == MySnake.getNode(z).y)) )
//						{ break; }
//					}
//
//					if(z == MySnake.Length())
//					{
//						if(testLevel.get(x, y) == Level::BLOCKS::BLOCKED)
//						{ std::cout << char(178); }
//						else
//						{ std::cout << char(176); }
//					}
//					else
//					{
//						std::cout << 'o';
//					}
//				}
//			}
//			std::cout << std::endl;
//		}
//
//		//input = _getch();
//
//		switch(input)
//		{
//			case 'w':
//				MySnake.Up();
//			break;
//			case 's':
//				MySnake.Down();
//			break;
//			case 'a':
//				MySnake.Left();
//			break;
//			case 'd':
//				MySnake.Right();
//			break;
//		}
//
//		if(Timer.AnimTime() == 1)
//		{
//			if(!MySnake.Move())
//			{
//				system("cls");
//				std::cout << "You loose" << std::endl;
//				break;
//			}
//			else if(!testLevel.FoodLeft())
//			{
//				system("cls");
//				std::cout << "You win" << std::endl;
//				break;
//			}
//		}
//	}
//
//	system("pause");
//}