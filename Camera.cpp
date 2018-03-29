
#include "Camera.h"


MainCamera::MainCamera(D3DXVECTOR3 pos, D3DXVECTOR3 eye) : Position(pos),
															EyesView(eye - pos)															
{ }

void MainCamera::GetViewMatrix(D3DXMATRIX* V)
{
	// Делаем оси камеры ортогональными 
	D3DXVec3Normalize(&EyesView, &EyesView);

	D3DXVec3Cross(&UpVector, &EyesView, &RtVector);
	D3DXVec3Normalize(&UpVector, &UpVector);

	D3DXVec3Cross(&RtVector, &UpVector, &EyesView);
	D3DXVec3Normalize(&RtVector, &RtVector);

	// Строим матрицу вида:
	float x = -D3DXVec3Dot(&RtVector, &Position);
	float y = -D3DXVec3Dot(&UpVector, &Position);
	float z = -D3DXVec3Dot(&EyesView, &Position);

	(*V)(0, 0) = RtVector.x;
	(*V)(0, 1) = UpVector.x;
	(*V)(0, 2) = EyesView.x;
	(*V)(0, 3) = 0.0f; 

	(*V)(1, 0) = RtVector.y;
	(*V)(1, 1) = UpVector.y;
	(*V)(1, 2) = EyesView.y;
	(*V)(1, 3) = 0.0f;

	(*V)(2, 0) = RtVector.z;
	(*V)(2, 1) = UpVector.z;
	(*V)(2, 2) = EyesView.z;
	(*V)(2, 3) = 0.0f;

	(*V)(3, 0) = x;
	(*V)(3, 1) = y;
	(*V)(3, 2) = z;
	(*V)(3, 3) = 1.0f;
}


FloatingCamera::FloatingCamera(D3DXVECTOR3 pos) : MainCamera(pos, D3DXVECTOR3(0, 0, 0)),
												  MovSpeed(0.1f),
												  RotSpeed(0.05f)
{}

void FloatingCamera::SetRotSpeed(float s)
{
	RotSpeed = s;
}

float FloatingCamera::GetRotSpeed()
{
	return RotSpeed;
}

void FloatingCamera::SetMovSpeed(float s)
{
	MovSpeed = s;
}

float FloatingCamera::GetMovSpeed()
{
	return MovSpeed;
}

/*
void FloatingCamera::KEYprocessing(UINT key)
{
	if(!isStop)
	{
		D3DXMATRIX rot(1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
		D3DXMATRIX rotX(rot), rotZ(rot), rotY(rot);
		switch(key)
		{
			case VK_UP:
			case VK_DOWN:
				if(key == VK_UP)
				{ 
					D3DXMatrixRotationX(&rotX, GetRotSpeed());
					D3DXMatrixRotationZ(&rotZ, GetRotSpeed());
				}
				else
				{
					D3DXMatrixRotationX(&rotX, -GetRotSpeed());
					D3DXMatrixRotationX(&rotZ, -GetRotSpeed());
				}
				D3DXMatrixMultiply(&rot, &rotX, &rotZ);
				MultEyesView(rot);
			break;

			case VK_RIGHT:
			case VK_LEFT:
				if(key == VK_RIGHT)
				{
					D3DXMatrixRotationY(&rotY, GetRotSpeed());
				}
				else
				{
					D3DXMatrixRotationY(&rotY, -GetRotSpeed());
				}
				MultEyesView(rotY);
			break;

			case VK_SPACE:
				D3DXVECTOR3 m(Eye());
				D3DXMATRIX mov(1, 0, 0, m.x * GetMovSpeed(),
							   0, 1, 0, m.y * GetMovSpeed(),
							   0, 0, 1, m.z * GetMovSpeed(),
							   0, 0, 0, 1);
				MultPosition(mov);
			break;
		}
	}
}
*/

void FloatingCamera::RotUp()
{
	D3DXMATRIX M;
	D3DXMatrixRotationAxis(&M, &RtVector, RotSpeed);

	D3DXVec3TransformCoord(&UpVector, &UpVector, &M);
	D3DXVec3TransformCoord(&EyesView, &EyesView, &M);
}

void FloatingCamera::RotDown()
{
	D3DXMATRIX M;
	D3DXMatrixRotationAxis(&M, &RtVector, -RotSpeed);

	D3DXVec3TransformCoord(&UpVector, &UpVector, &M);
	D3DXVec3TransformCoord(&EyesView, &EyesView, &M);
}

void FloatingCamera::RotRight()
{
	D3DXMATRIX M;
	D3DXMatrixRotationAxis(&M, &UpVector, RotSpeed);

	D3DXVec3TransformCoord(&RtVector, &RtVector, &M);
	D3DXVec3TransformCoord(&EyesView, &EyesView, &M);
}

void FloatingCamera::RotLeft()
{
	D3DXMATRIX M;
	D3DXMatrixRotationAxis(&M, &UpVector, -RotSpeed);

	D3DXVec3TransformCoord(&RtVector, &RtVector, &M);
	D3DXVec3TransformCoord(&EyesView, &EyesView, &M);
}

void FloatingCamera::Mov()
{
	Position += EyesView * MovSpeed;
}

SnakeCamera::SnakeCamera(D3DXVECTOR3 pos, D3DXVECTOR3 eye, Snake *snake) : MainCamera(pos, eye), TheSnake(snake), OldTeilPos(snake->getNode(0))
{
	game::COORDS t = TheSnake->getNode(0);
	
	MainCamera::Position = D3DXVECTOR3(t.x, -4, t.y + 1);

	MainCamera::EyesView.x = MainCamera::Position.x + 0.002f;
	MainCamera::EyesView.y = -1.3f - MainCamera::Position.y;
	MainCamera::EyesView.z = MainCamera::Position.z - 1;
	
	D3DXVec3Normalize(&EyesView, &EyesView);

	MainCamera::RtVector.x = -MainCamera::EyesView.z;
	MainCamera::RtVector.y = 0.00002f;
	MainCamera::RtVector.z = MainCamera::EyesView.x;
	D3DXVec3Normalize(&RtVector, &RtVector);

	D3DXVec3Cross(&UpVector, &EyesView, &RtVector);
	D3DXVec3Normalize(&UpVector, &UpVector);
}

void SnakeCamera::GetViewMatrix(D3DXMATRIX* V, float move, Level* level)
{
	game::COORDS t;

	SetCameraPos(move, level);

	t = TheSnake->getNode(0);
	
	if( (TheSnake->Length() != 1) && (move != 1.0f))
	{
		t.x = t.x + ( (TheSnake->getNextPos().x - t.x) * move);
		t.y = t.y + ( (TheSnake->getNextPos().y - t.y) * move);
	}
	MainCamera::EyesView.x = t.x - MainCamera::Position.x + 0.002f;
	MainCamera::EyesView.y = -1.3f - MainCamera::Position.y;
	MainCamera::EyesView.z = t.y - MainCamera::Position.z + 0.002f;
	
	D3DXVec3Normalize(&EyesView, &EyesView);

	MainCamera::RtVector.x = -MainCamera::EyesView.z;
	MainCamera::RtVector.y = 0.00002f;
	MainCamera::RtVector.z = MainCamera::EyesView.x;
	D3DXVec3Normalize(&RtVector, &RtVector);

	D3DXVec3Cross(&UpVector, &EyesView, &RtVector);
	D3DXVec3Normalize(&UpVector, &UpVector);

	//матрица вида:
	float x = -D3DXVec3Dot(&RtVector, &Position);
	float y = -D3DXVec3Dot(&UpVector, &Position);
	float z = -D3DXVec3Dot(&EyesView, &Position);

	(*V)(0, 0) = RtVector.x;
	(*V)(0, 1) = UpVector.x;
	(*V)(0, 2) = EyesView.x;
	(*V)(0, 3) = 0.0f; 

	(*V)(1, 0) = RtVector.y;
	(*V)(1, 1) = UpVector.y;
	(*V)(1, 2) = EyesView.y;
	(*V)(1, 3) = 0.0f;

	(*V)(2, 0) = RtVector.z;
	(*V)(2, 1) = UpVector.z;
	(*V)(2, 2) = EyesView.z;
	(*V)(2, 3) = 0.0f;

	(*V)(3, 0) = x;
	(*V)(3, 1) = y;
	(*V)(3, 2) = z;
	(*V)(3, 3) = 1.0f;
}

void SnakeCamera::SetCameraPos(float tmove, Level* level)
{
	static float startHeig = -2.0f;

	MainCamera::Position.y = -2.0f;
	if(startHeig < 0)
	{
		MainCamera::Position.y += startHeig;
		if(TheSnake->Length() > 1)
		{ startHeig += 0.1f; }
	}

	if(TheSnake->Length() > 1)
	{
		OldTeilPos = TheSnake->getOldTeil();
		MainCamera::Position.x = OldTeilPos.x;
		MainCamera::Position.z = OldTeilPos.y;

		if(tmove != 1)
		{
			if(TheSnake->isEat())
			{ tmove = 0; }

			game::COORDS t = TheSnake->getNode(TheSnake->getTeilNum());
			
			//MainCamera::Position.x += (t.x - MainCamera::Position.x) * tmove;
			//MainCamera::Position.z += (t.y - MainCamera::Position.z) * tmove;
			//float x = (t.x - MainCamera::Position.x) * tmove;
			//float y = (t.y - MainCamera::Position.z) * tmove;

			if(TheSnake->NumOfJumps() != 0)
			{
				Snake::dir D;
				for(int i = 0; i < TheSnake->NumOfJumps(); i++)
				{
						D = TheSnake->getJump(i);
						switch(D)
						{
							case Snake::UP:
								{ MainCamera::Position.z += level->lenY(); }
							break;

							case Snake::DOWN:
								{ MainCamera::Position.z -= level->lenY(); }
							break;

							case Snake::RIGHT:
								{
									MainCamera::Position.x -= level->lenX();
		/*							if(TheSnake->getJumpTimer(i) == TheSnake->getTeilNum())
									{ MainCamera::Position.x += -2 * x; }*/
								}
							break;

							case Snake::LEFT:
								{
									MainCamera::Position.x += level->lenX();
									/*if(TheSnake->getJumpTimer(i) == TheSnake->getTeilNum() + 1)
									{ MainCamera::Position.x += +2 * x; }*/
								}
							break;
						}
				}
			}
			MainCamera::Position.x += (t.x - MainCamera::Position.x) * tmove;
			MainCamera::Position.z += (t.y - MainCamera::Position.z) * tmove;

		}
	}

}