
#include "game.h"

//Ололо. Модуль числа
float Mod(float n)
{
	if(n < 0)
	{ n *= -1; }
	return n;
}

Game::Game(HINSTANCE* hinst, HINSTANCE* hprevinst, LPSTR* lpcmdln, int* ncmdshw) : Application(hinst, hprevinst, lpcmdln, ncmdshw)
{
	pDirect3D = 0;
	pDirect3DDevice = 0;
}

void Game::RenderScene(Level *pLevel, Snake *pSnake, float tmove)
{
	//Составление сцены
	SceneObjNum = 0;
	int z = 0;
	//float tX;
	//float tY;

	if(tmove == 1)
	{ tmove = 0; }

	float LevelX = pLevel->lenX();
	float LevelY = pLevel->lenY();

	for(int y = 0; y < pLevel->lenY(); y++)
	{
		for(int x = 0; x < pLevel->lenX(); x++)
		{
			if( (x == pLevel->CurrentFood().x) && (y == pLevel->CurrentFood().y))
			{
				//Scene.push_back(SCENENODE(2, x + LevelX, y));
				//Scene.push_back(SCENENODE(2, x - LevelX, y));
				//Scene.push_back(SCENENODE(2, x, y + LevelY));
				//Scene.push_back(SCENENODE(2, x, y - LevelY));
				//Scene.push_back(SCENENODE(2, x + LevelX, y + LevelY));
				//Scene.push_back(SCENENODE(2, x - LevelX, y - LevelY));
				//Scene.push_back(SCENENODE(2, x - LevelX, y + LevelY));
				//Scene.push_back(SCENENODE(2, x + LevelX, y - LevelY));
				Scene.push_back(SCENENODE(3, x, y));
				SceneObjNum += 1;
			}
			else
			{
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, LevelX, LevelY);
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, -LevelX, -LevelY);
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, -LevelX, LevelY);
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, LevelX, -LevelY);
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, LevelX, 0);
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, 0, LevelY);
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, -LevelX, 0);
				//SetSnakeNode(pLevel, pSnake, tmove, x, y, 0, -LevelY);
				SetSnakeNode(pLevel, pSnake, tmove, x, y, 0, 0);
			}
						
			//const D3DXVECTOR3 t(x, 0, y);
			//if(D3DXVec3Length(&t) < 25)
			//{
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x + LevelX, y + LevelY));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x - LevelX, y - LevelY));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x + LevelX, y - LevelY));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x - LevelX, y + LevelY));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x + LevelX, y));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x - LevelX, y));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x, y + LevelY));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x, y - LevelY));
			//	Scene.push_back(SCENENODE(ObjectsMap[x + (y * pLevel->lenX())], x, y));
			//	
			//	SceneObjNum += 9;
			//}
		}
	}
	
	pDirect3DDevice->Clear(1, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff010101, 1.0, 0);
	
	pDirect3DDevice->BeginScene();

		//Вывод
		for(int i = 0; i < SceneObjNum; i++)
		{
			Effects->SetLightDir(LightVector - D3DXVECTOR4(Scene[i].x, 0, Scene[i].y, 0));
			SceneObjects[Scene[i].num]->Move(D3DXVECTOR3(Scene[i].x, 0, Scene[i].y));
			SceneObjects[Scene[i].num]->RenderMesh();
		}

		for(int i = 0; i < StaticSceneObjNum; i++)
		{
			const D3DXVECTOR3 t = D3DXVECTOR3(StaticScene[i].x, 0, StaticScene[i].y) - D3DXVECTOR3(pSnake->getNode(0).x, 0, pSnake->getNode(0).y);
			if(D3DXVec3Length(&t) < 20)
			{
				Effects->SetLightDir(LightVector - D3DXVECTOR4(StaticScene[i].x, 0, StaticScene[i].y, 0));
				SceneObjects[StaticScene[i].num]->Move(D3DXVECTOR3(StaticScene[i].x, 0, StaticScene[i].y));
				SceneObjects[StaticScene[i].num]->RenderMesh();
			}
		}

	pDirect3DDevice->EndScene();
	pDirect3DDevice->Present(0, 0, 0, 0);

	Scene.clear();
}

void Game::SetSnakeNode(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY)
{
	game::COORDS dir = pSnake->getDirCoord();

	if( (pLevel->CurrentFood().x == dir.x) && (pLevel->CurrentFood().y == dir.y))
	{
		SetSnakeNodeWhenEat(pLevel, pSnake, tmove, x, y, ShiftX, ShiftY);
	}
	else if(pLevel->get(dir.x, dir.y) == 0)
	{
		SetSnakeNodeWhenStep(pLevel, pSnake, tmove, x, y, ShiftX, ShiftY);
	}
	else if(pLevel->get(dir.x, dir.y) == 1)
	{
		SetSnakeNodeWhenWall(pLevel, pSnake, tmove, x, y, ShiftX, ShiftY);
	}
}

void Game::SetSnakeNodeWhenEat(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY)
{
		float tX, tY;

		tmove *= 2;
		if(tmove > 1)
		{ tmove = 1; }

		for(int z = 0; z < pSnake->Length(); z++)
		{
			if((x == pSnake->getNode(z).x) && (y == pSnake->getNode(z).y))
			{
				if(pSnake->Length() == 1)
				{
					LightVector = D3DXVECTOR4(x, -0.3, y, 0);
					tX = x;
					tY = y;
				}
				else
				{
					game::COORDS move;
					if(z == 0)
					{
						move.x = (pSnake->getNextPos().x - x);
						move.y = (pSnake->getNextPos().y - y);
						if(Mod(move.x) > 1 || Mod(move.y) > 1)
						{
							if(move.x > 0)
							{
								tX = x + ((-1) * tmove);
								tY = y;
							}
							else if(move.x < 0)
							{
								tX = x + (1 * tmove);
								tY = y;
							}
							else if(move.y > 0)
							{
								tX = x;
								tY = y + ((-1) * tmove);
							}
							else if(move.y < 0)
							{
								tX = x;
								tY = y + (1 * tmove);
							}
						}
						else
						{
							tX = x + ( (pSnake->getNextPos().x - x) * tmove);
							tY = y + ( (pSnake->getNextPos().y - y) * tmove);
						}
						LightVector = D3DXVECTOR4(tX, -0.3, tY, 0);
					}
					else
					{
						move.x = (pSnake->getNode(z - 1).x - x);
						move.y = (pSnake->getNode(z - 1).y - y);
						if(Mod(move.x) > 1 || Mod(move.y) > 1)
						{
							if(move.x > 0)
							{
								tX = x + ((-1) * tmove);
								tY = y;
							}
							else if(move.x < 0)
							{
								tX = x + (1 * tmove);
								tY = y;
							}
							else if(move.y > 0)
							{
								tX = x;
								tY = y + ((-1) * tmove);
							}
							else if(move.y < 0)
							{
								tX = x;
								tY = y + (1 * tmove);
							}
						}
						else
						{
							tX = x + ( (pSnake->getNode(z - 1).x - x) * tmove);
							tY = y + ( (pSnake->getNode(z - 1).y - y) * tmove);
						}
					}
				}

				Scene.push_back(SCENENODE(2, tX + ShiftX, tY + ShiftY));
				SceneObjNum++;
				break;
			}
		}
}

void Game::SetSnakeNodeWhenStep(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY)
{
		float tX, tY;

		for(int z = 0; z < pSnake->Length(); z++)
		{
			if((x == pSnake->getNode(z).x) && (y == pSnake->getNode(z).y))
			{
				if(pSnake->Length() == 1)
				{
					LightVector = D3DXVECTOR4(x, -0.3, y, 0);
					tX = x;
					tY = y;
				}
				else
				{
					game::COORDS move;
					if(z == 0)
					{
						move.x = (pSnake->getNextPos().x - x);
						move.y = (pSnake->getNextPos().y - y);
						if(Mod(move.x) > 1 || Mod(move.y) > 1)
						{
							if(move.x > 0)
							{
								tX = x + ((-1) * tmove);
								tY = y;
							}
							else if(move.x < 0)
							{
								tX = x + (1 * tmove);
								tY = y;
							}
							else if(move.y > 0)
							{
								tX = x;
								tY = y + ((-1) * tmove);
							}
							else if(move.y < 0)
							{
								tX = x;
								tY = y + (1 * tmove);
							}
						}
						else
						{
							tX = x + ( (pSnake->getNextPos().x - x) * tmove);
							tY = y + ( (pSnake->getNextPos().y - y) * tmove);
						}

						LightVector = D3DXVECTOR4(tX, -0.3, tY, 0);
					}
					else
					{
						move.x = (pSnake->getNode(z - 1).x - x);
						move.y = (pSnake->getNode(z - 1).y - y);
						if(Mod(move.x) > 1 || Mod(move.y) > 1)
						{
							if(move.x > 0)
							{
								tX = x + ((-1) * tmove);
								tY = y;
							}
							else if(move.x < 0)
							{
								tX = x + (1 * tmove);
								tY = y;
							}
							else if(move.y > 0)
							{
								tX = x;
								tY = y + ((-1) * tmove);
							}
							else if(move.y < 0)
							{
								tX = x;
								tY = y + (1 * tmove);
							}
						}
						else
						{
							tX = x + ( (pSnake->getNode(z - 1).x - x) * tmove);
							tY = y + ( (pSnake->getNode(z - 1).y - y) * tmove);
						}
					}
				}

				Scene.push_back(SCENENODE(2, tX + ShiftX, tY + ShiftY));
				SceneObjNum++;
				break;
			}
		}
}

void Game::SetSnakeNodeWhenWall(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY)
{
		float tX, tY;

		for(int z = 0; z < pSnake->Length(); z++)
		{
			if((x == pSnake->getNode(z).x) && (y == pSnake->getNode(z).y))
			{
				if(pSnake->Length() == 1)
				{
					LightVector = D3DXVECTOR4(x, -0.3, y, 0);
					tX = x;
					tY = y;
				}
				else
				{
					game::COORDS move;
					if(z == 0)
					{
						tX = pSnake->getNode(0).x;
						tY = pSnake->getNode(0).y;
						LightVector = D3DXVECTOR4(tX, -0.3, tY, 0);
					}
					else
					{
						move.x = (pSnake->getNode(z - 1).x - x);
						move.y = (pSnake->getNode(z - 1).y - y);
						if(Mod(move.x) > 1 || Mod(move.y) > 1)
						{
							if(move.x > 0)
							{
								tX = x + ((-1) * tmove);
								tY = y;
							}
							else if(move.x < 0)
							{
								tX = x + (1 * tmove);
								tY = y;
							}
							else if(move.y > 0)
							{
								tX = x;
								tY = y + ((-1) * tmove);
							}
							else if(move.y < 0)
							{
								tX = x;
								tY = y + (1 * tmove);
							}
						}
						else
						{
							tX = x + ( (pSnake->getNode(z - 1).x - x) * (tmove / 2));
							tY = y + ( (pSnake->getNode(z - 1).y - y) * (tmove / 2));
						}
					}
				}

				Scene.push_back(SCENENODE(2, tX + ShiftX, tY + ShiftY));
				SceneObjNum++;
				break;
			}
		}
}

void Game::DelApp()
{

}

void Game::SetMainProc( LRESULT (CALLBACK *proc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) )
{
	MainProc = proc;
}

HWND Game::Wnd()
{
	return hWnd;
}

bool Game::MsgLoop(Level *pLevel, Snake *pSnake, GameTimer* Timer)
{
	//if(true)
 //   {
    	ShowWindow(hWnd, SW_SHOWDEFAULT);//рисовать окно
    	UpdateWindow(hWnd);				 //обновить окно
    	ZeroMemory(&msg, sizeof(msg));
		
    	while(msg.message != WM_QUIT)
    	{
    		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    		{
    			TranslateMessage(&msg);
    			DispatchMessage(&msg);
    		}
    		else
			{
				float moveTime = Timer->AnimTime();
				if(moveTime == 1)
				{
					if(!pSnake->Move())
					{
						MessageBox(0, "Не повезло. Может в другой раз.", "Loose", MB_OK);
						PostQuitMessage(0);
						return 0;
					}
					else if(!pLevel->FoodLeft())
					{
						MessageBox(0,	"                             Победа!\n"
										"Ожидайте дальнейшего развития игры!", "Win", MB_OK);
						PostQuitMessage(0);
						return 0;
					}
				}

				GameCamera->GetViewMatrix(&ViewMatrix, moveTime, pLevel);
				Effects->SetViewMatrix(ViewMatrix);
				RenderScene(pLevel, pSnake, moveTime);
			}
    	}
    //}

	return 0;
}

bool Game::CreateApp(UINT X, UINT Y, std::string wclassname, std::string wname)
{
	return Application::CreateApp(X, Y, wclassname, wname);
}

bool Game::InitGame(int X, int Y, bool Windowed, std::string game_name, Snake *pSnake, Level* level)
{
	CreateApp(X, Y, game_name, game_name);

	game::COORDS t = pSnake->getNode(0);
	GameCamera = new SnakeCamera(D3DXVECTOR3(t.x, 3.2, t.y), D3DXVECTOR3(t.x, 3, t.y), pSnake);

	//Инициализация
	if((pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{ return false; }
	
	//Заполняем асновные параметры
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;
	
    //Получить формат пиксела
	D3DDISPLAYMODE d3ddm;
	pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	
	if(!Windowed)
	{//Полноэкранный режим
	    d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
	    d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
	    d3dpp.BackBufferCount = 3;
	    
		//Установка параметров полноэкранного режима
		
		//d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.SwapEffect	   = D3DSWAPEFFECT_FLIP;
		d3dpp.Windowed		   = FALSE;
		d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}
	else
	{//Оконный режим
	   	d3dpp.BackBufferWidth = X;
	    d3dpp.BackBufferHeight = Y;
		RECT wndRect;
		RECT clientRect;
		GetWindowRect(hWnd, &wndRect);
		GetClientRect(hWnd, &clientRect);
		
		int iWinWidth = X + (wndRect.right - wndRect.left) -
				(clientRect.right - clientRect.left);
		int iWinHeight = Y + (wndRect.bottom - wndRect.top) -
				(clientRect.bottom - clientRect.top);
		
		MoveWindow(hWnd, wndRect.left, wndRect.top,
					iWinWidth, iWinHeight, TRUE);
		
		//Установка параметров
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.SwapEffect	   = D3DSWAPEFFECT_DISCARD;
		d3dpp.Windowed		   = TRUE;
	}
	
	//Создать 3D-устройство
	DWORD Flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		
	HRESULT hRes;
	if(FAILED(hRes = pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, hWnd, Flags,
		&d3dpp, &pDirect3DDevice)))
	{ return FAILED(hRes); }
	
	//Установить перспективу и соотношение сторон
	float Aspect =
		(float)d3dpp.BackBufferWidth/(float)d3dpp.BackBufferHeight;

//Матрица проекции
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DX_PI/3.0f,
			Aspect, 1.0f, 1000.0f);
	pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &ProjMatrix);
	
	pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//откл освещение Direct3D
	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Создание объектов сцены
	for(int n = 0; n < 3; n++)
	{ Meshes.push_back(new FASTMESH(this)); }

	if(	!Meshes[0]->LoadMesh("res\\MESH\\floor.x") ||
		!Meshes[1]->LoadMesh("res\\MESH\\wall.x") ||
		!Meshes[2]->LoadMesh("res\\MESH\\ball.x")
		 )
	{ return false; }

	//Effects.push_back(new LIGHT_EFFECT(this));

	Effects = new SNAKE_EFFECTS(this);

	if(!Effects->LoadEffect("res\\EFFECT\\test.txt"))
	{ return false; }

	
	GameCamera->GetViewMatrix(&ViewMatrix, 0, level);
	//reinterpret_cast<LIGHT_EFFECT*>(Effects[0])->SetLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	//reinterpret_cast<LIGHT_EFFECT*>(Effects[0])->SetModelColor(D3DXVECTOR4(0.0f, 0.6f, 0.0f, 1.0f));
	Effects->SetProjMatrix(ProjMatrix);
	Effects->SetViewMatrix(ViewMatrix);

	SceneObjects.push_back(new GRAPHICS_OBJECT(D3DXVECTOR3(0, 0, 0), Meshes[0], Effects, 1));
	SceneObjects.push_back(new GRAPHICS_OBJECT(D3DXVECTOR3(0, 0, 0), Meshes[1], Effects, 0));
	SceneObjects.push_back(new GRAPHICS_OBJECT(D3DXVECTOR3(0, 0, 0), Meshes[2], Effects, 2));
	SceneObjects.push_back(new GRAPHICS_OBJECT(D3DXVECTOR3(0, 0, 0), Meshes[2], Effects, 3));

	ObjectsMap = 0;
	ObjectsMap = new int[level->lenX() * level->lenY()];

	for(int i = 0; i < level->lenX() * level->lenY(); i++)
	{
		ObjectsMap[i] = 0;
	}
	
	for(int y = 0; y < level->lenY(); y++)
	{
		for(int x = 0; x < level->lenX(); x++)
		{
			if(level->get(x, y) == Level::BLOCKS::BLOCKED)
			{ ObjectsMap[x + (level->lenX() * y)] = 1; }
			else
			{ ObjectsMap[x + (level->lenX() * y)] = 0; }
		}
	}

	float LevelX = level->lenX();
	float LevelY = level->lenY();

	StaticSceneObjNum = 0;

	for(int y = 0; y < level->lenY(); y++)
	{
		for(int x = 0; x < level->lenX(); x++)
		{
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x + LevelX, y + LevelY));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x - LevelX, y - LevelY));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x + LevelX, y - LevelY));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x - LevelX, y + LevelY));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x + LevelX, y));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x - LevelX, y));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x, y + LevelY));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x, y - LevelY));
				StaticScene.push_back(SCENENODE(ObjectsMap[x + (y * level->lenX())], x, y));
				
				StaticSceneObjNum += 9;
		}
	}
	
	return true;
}

Game::~Game()
{
	if(pDirect3DDevice)
	{ pDirect3DDevice->Release(); }

	if(pDirect3D)
	{ pDirect3D->Release(); }
}