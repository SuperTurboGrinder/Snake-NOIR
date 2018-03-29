
#ifndef GAME_H
#define GAME_H

#include "application.h"
#include "level.h"
#include "snake.h"
#include "game_timer.h"
#include <d3dx9.h>
#include <vector>
#include "Camera.h"
#include <math.h>

class Game;

class FASTMESH
{
	friend class SNAKE_EFFECTS;
	private:
		ID3DXMesh* PMesh;
		D3DXMATRIX WorldCoords;
		float MeshScale;
		Game* GameObj;
	public:
		FASTMESH(Game* gm);
		BOOL LoadMesh(std::string filename);
		void UnloadMesh();
		void Scale(float s);
		void SetCoords(D3DXVECTOR3 coords);
		D3DXMATRIX GetWorldCoords();
		~FASTMESH();
};

//class FASTEFFECT
//{
//	protected:
//		ID3DXEffect* Effect;
//		int TechNum;
//		std::vector<D3DXHANDLE> Technics;
//		Game* GameObj;
//	public:
//		FASTEFFECT(Game* gm);
//		virtual BOOL LoadEffect(std::string filename);
//		void UnloadEffect();
//		virtual BOOL RenderMesh(FASTMESH* Mesh);
//		virtual ~FASTEFFECT();
//};

class SNAKE_EFFECTS /* : public FASTEFFECT*/
{
	private:
		ID3DXEffect* Effect;
		int TechNum;
		std::vector<D3DXHANDLE> Technics;
		Game* GameObj;

		D3DXHANDLE WorldMatrixHandle;
		D3DXHANDLE ViewMatrixHandle;
		D3DXHANDLE ProjMatrixHandle;
		D3DXHANDLE LightDirHandle;

		void UnloadEffect();
		bool LoadFromFile(std::string filename);
	public:
		SNAKE_EFFECTS(Game* gm);
		BOOL LoadEffect(std::string filename);
		BOOL RenderMesh(FASTMESH* Mesh, int ThNum);
		void SetProjMatrix(const D3DXMATRIX &mat);
		void SetViewMatrix(const D3DXMATRIX &mat);
		void SetWorldMatrix(const D3DXMATRIX &mat);
		void SetLightDir(const D3DXVECTOR4 &dir);
		~SNAKE_EFFECTS();
};
	
class GRAPHICS_OBJECT
{
	private:
		D3DXVECTOR3 Coords;
		FASTMESH* Mesh;
		SNAKE_EFFECTS* Effects;
		int TechNum;
	public:
		GRAPHICS_OBJECT(const D3DXVECTOR3& coords, FASTMESH* mesh, SNAKE_EFFECTS* effect, int TechNum);
		void Move(const D3DXVECTOR3& coords);
		D3DXVECTOR3 ObjCoords() const;
		BOOL RenderMesh() const;
};

class Game : public Application
{
	public:
		LPDIRECT3D9 pDirect3D;
		LPDIRECT3DDEVICE9 pDirect3DDevice;
		D3DXMATRIX ProjMatrix;
		D3DXMATRIX ViewMatrix;
	private:
		std::vector<GRAPHICS_OBJECT*> SceneObjects;
		std::vector<FASTMESH*> Meshes;
		SNAKE_EFFECTS* Effects;
		struct SCENENODE
		{
			SCENENODE(int n, float tx, float ty) : num(n), x(tx), y(ty)
			{}

			bool operator < (SCENENODE& t)
			{
				D3DXVECTOR3 a(x, 0, y);
				D3DXVECTOR3 b(t.x, 0, t.y);
				return (D3DXVec3Length(&a) < D3DXVec3Length(&b));
			}

			int num;
			float x;
			float y;
		};

		std::vector<SCENENODE> Scene;
		std::vector<SCENENODE> StaticScene;
		int StaticSceneObjNum;
		int SceneObjNum;
		int* ObjectsMap; //Массив - карта трёхмерных объектов
		SnakeCamera* GameCamera;
		D3DXVECTOR4 LightVector;
	private:
		bool CreateApp(UINT X, UINT Y, std::string wclassname, std::string wname);
		void RenderScene(Level *pLevel, Snake *pSnake, float tmove);
		void SetSnakeNode(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY);
		void SetSnakeNodeWhenEat(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY);
		void SetSnakeNodeWhenStep(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY);
		void SetSnakeNodeWhenWall(Level *pLevel, Snake *pSnake, float tmove, int x, int y, int ShiftX, int ShiftY);
	public:
		Game(HINSTANCE* hinst, HINSTANCE* hprevinst, LPSTR* lpcmdln, int* ncmdshw);
		
		//Интерфейс класса приложения
		virtual void DelApp();
		virtual void SetMainProc( LRESULT (CALLBACK *proc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) ); //установка оконной ппоцедуры
		virtual HWND Wnd();
		bool MsgLoop(Level *pLevel, Snake *pSnake, GameTimer* Timer);

		bool InitGame(int X, int Y, bool Windowed, std::string game_name, Snake *pSnake, Level* level);

		~Game();
};


#endif