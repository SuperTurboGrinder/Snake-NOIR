#ifndef CAMERA_H
#define CAMERA_H


#include <d3dx9.h>
#include "snake.h"
#include "game_types.h"

class MainCamera
{
	protected:
		D3DXVECTOR3 Position;
		D3DXVECTOR3 EyesView;
		D3DXVECTOR3 UpVector;
		D3DXVECTOR3 RtVector;
	public:
		MainCamera(D3DXVECTOR3 pos, D3DXVECTOR3 eye);
		//D3DXVECTOR3 Pos();
		//D3DXVECTOR3 Eye();
		//D3DXVECTOR3 Up();
		//D3DXVECTOR3 Rgt();
		virtual void GetViewMatrix(D3DXMATRIX*);
};

class FloatingCamera : public MainCamera
{
	private:
		float RotSpeed;
		float MovSpeed;
	public:
		FloatingCamera(D3DXVECTOR3 pos);

		void SetRotSpeed(float);
		float GetRotSpeed();
		void SetMovSpeed(float);
		float GetMovSpeed();

		void RotUp();
		void RotDown();
		void RotRight();
		void RotLeft();

		void Mov();
};

class SnakeCamera : public MainCamera
{
	private:
		Snake* TheSnake;
		game::COORDS OldTeilPos;

		void SetCameraPos(float, Level* level);
	public:
		SnakeCamera(D3DXVECTOR3, D3DXVECTOR3, Snake*);
		void GetViewMatrix(D3DXMATRIX*, float, Level* level);
};

#endif