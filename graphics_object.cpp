
#include "game.h"
#include <math.h>
#define GAME_DEBUG

FASTMESH::FASTMESH(Game* gm) : PMesh(0), MeshScale(1.0f), GameObj(gm)
{
	D3DXMatrixIdentity(&WorldCoords);
}

BOOL FASTMESH::LoadMesh(std::string filename)
{
	UnloadMesh();
		
	LPD3DXBUFFER pD3DXMtrlBuffer;

	D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, GameObj->pDirect3DDevice,
						NULL, &pD3DXMtrlBuffer, NULL, 0, &PMesh);

	if(!PMesh)
	{

#ifdef GAME_DEBUG
		MessageBox(0, "Невозможно загрузить меш", 0, 0);
#endif

		return false;
	}
		
	return true;
}
	
void FASTMESH::UnloadMesh()
{
	if(PMesh != 0)
	{ PMesh->Release(); }
}

void FASTMESH::Scale(float s)
{
	MeshScale = s;
}

void FASTMESH::SetCoords(D3DXVECTOR3 coords)
{
	D3DXMATRIX newWorldMatrix(
								1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								coords.x, coords.y, coords.z, 1
							 );

	WorldCoords = newWorldMatrix;
}

D3DXMATRIX FASTMESH::GetWorldCoords()
{
	return WorldCoords;
}

FASTMESH::~FASTMESH()
{
	UnloadMesh();
}

//*************************************************************************************

//FASTEFFECT::FASTEFFECT(Game* gm) : Effect(0), GameObj(gm)
//{ }

bool SNAKE_EFFECTS::LoadFromFile(std::string filename)
{
	UnloadEffect();

	TechNum = 0;
	Technics.clear();
	WorldMatrixHandle = 0;
	ViewMatrixHandle = 0;
	ProjMatrixHandle = 0;
	LightDirHandle = 0;

	HRESULT hr = 0;
        
	ID3DXBuffer* errorBuffer    = 0;
	hr = D3DXCreateEffectFromFile(
								 GameObj->pDirect3DDevice,
								 filename.c_str(),
								0,
								0,
								D3DXSHADER_DEBUG,
								0,
								&Effect,
								&errorBuffer
								);

	//Сообщения об ошибках
	if(errorBuffer)
	{

#ifdef GAME_DEBUG
		 MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
#endif

		 errorBuffer->Release();
	}

	if(FAILED(hr))
	{

#ifdef GAME_DEBUG
		MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
#endif

		return false;
	}
		
	int n = 0;
	char buf[5];
	static_cast<std::string>(_itoa(n++, buf, -1));
	std::string techName = "T" + std::string(buf);

	D3DXHANDLE Techniq = 0;
	Techniq = Effect->GetTechniqueByName(techName.c_str());

	while((Techniq != 0))// && (Effect->ValidateTechnique(Techniq) != S_OK))
	{
		if(Techniq == 0)
		{

#ifdef GAME_DEBUG
			MessageBox(0, "Инициализация хендла основной техники не удалась", 0, 0);
#endif

			return false;
		}

		Technics.push_back(Techniq);
		Techniq = 0;

		static_cast<std::string>(_itoa(n++, buf, -1));
		std::string techName = "T" + std::string(buf);

		Techniq = 0;
		Techniq = Effect->GetTechniqueByName(techName.c_str());
	}
    
//	if(Techniq == 0)
//	{
//
//#ifdef GAME_DEBUG
//		MessageBox(0, "Инициализация хендла основной техники не удалась", 0, 0);
//#endif
//
//		return false;
//	}
//
//	Technics.push_back(Techniq);

	TechNum = n + 1; 

	return true;
}

void SNAKE_EFFECTS::UnloadEffect()
{
//		if(Effect)
//		{ Effect->Release(); }
}

//BOOL FASTEFFECT::RenderMesh(FASTMESH* Mesh)
//{
//	if(GameObj->pDirect3DDevice && Mesh && Effect)
//	{    	    	
//    	Effect->SetTechnique(Techniq);
//    	
//    	UINT numPases = 0;
//    	
//    	Effect->Begin(&numPases, 0);
//    	
//    	for(UINT i = 0; i < numPases; i++)
//    	{
//			Effect->BeginPass(i);
//            
//			Mesh->PMesh->DrawSubset(0);
//            
//			Effect->EndPass();
//		}
//        
//		Effect->End();
//
//		return true;
//	}
//	else
//	{ return false; }
//}


//*********************************************************************************************8

SNAKE_EFFECTS::SNAKE_EFFECTS(Game* gm) : Effect(0), GameObj(gm)
{
}

BOOL SNAKE_EFFECTS::LoadEffect(std::string filename)
{
	BOOL result = LoadFromFile(filename);
	if(result == false)
	{ return result; }

	WorldMatrixHandle = Effect->GetParameterByName(0, "WorldMatrix");
	ViewMatrixHandle = Effect->GetParameterByName(0, "ViewMatrix");
	ProjMatrixHandle = Effect->GetParameterByName(0, "ProjectionMatrix");
	//MeshColorHandle = Effect->GetParameterByName(0, "MeshColor");
	//LightColorHandle = Effect->GetParameterByName(0, "LightColor");
	LightDirHandle = Effect->GetParameterByName(0, "LightDir");

	if((WorldMatrixHandle == 0) || (ViewMatrixHandle == 0) || (ProjMatrixHandle == 0) ||
		//(MeshColorHandle == 0) || (LightColorHandle == 0) ||
		(LightDirHandle == 0))
	{

#ifdef GAME_DEBUG
		MessageBox(0, "Инициализация дополнительных хендлов еффекта не сработала", 0, 0);
#endif

		return false;
	}

	SetProjMatrix(GameObj->ProjMatrix);
	SetViewMatrix(GameObj->ViewMatrix);

	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	SetWorldMatrix(m);

	//D3DXVECTOR4 col(1.0f, 1.0f, 1.0f, 1.0f);
	//SetModelColor(col);
	//SetLightColor(col);

	return true;
}

BOOL SNAKE_EFFECTS::RenderMesh(FASTMESH* Mesh, int tNum)
{
	SetWorldMatrix(Mesh->GetWorldCoords());
	
	if(GameObj->pDirect3DDevice && Mesh && Effect)
	{    	    	
    	Effect->SetTechnique(Technics[tNum]);
    	
    	UINT numPases = 0;
    	
    	Effect->Begin(&numPases, 0);
    	
    	for(UINT i = 0; i < numPases; i++)
    	{
			Effect->BeginPass(i);
            
			Mesh->PMesh->DrawSubset(0);
            
			Effect->EndPass();
		}
        
		Effect->End();

		return true;
	}
	else
	{ return false; }
}

//void LIGHT_EFFECT::SetLightColor(const D3DXVECTOR4 &col)
//{
//	Effect->SetVector(LightColorHandle, &col);
//}
//
//void LIGHT_EFFECT::SetModelColor(const D3DXVECTOR4 &col)
//{
//	Effect->SetVector(MeshColorHandle, &col);
//}

void SNAKE_EFFECTS::SetProjMatrix(const D3DXMATRIX &mat)
{
	Effect->SetMatrix(ProjMatrixHandle, &mat);
}

void SNAKE_EFFECTS::SetViewMatrix(const D3DXMATRIX &mat)
{
	Effect->SetMatrix(ViewMatrixHandle, &mat);
}

void SNAKE_EFFECTS::SetWorldMatrix(const D3DXMATRIX &mat)
{
	Effect->SetMatrix(WorldMatrixHandle, &mat);
}

void SNAKE_EFFECTS::SetLightDir(const D3DXVECTOR4 &dir)
{
	Effect->SetVector(LightDirHandle, &dir);
}

SNAKE_EFFECTS::~SNAKE_EFFECTS()
{
	UnloadEffect();
}

//***************************************************************************************

GRAPHICS_OBJECT::GRAPHICS_OBJECT(const D3DXVECTOR3& coords, FASTMESH* mesh, SNAKE_EFFECTS* effects, int ThNum) : Coords(coords), Mesh(mesh), Effects(effects), TechNum(ThNum)
{ }

void GRAPHICS_OBJECT::Move(const D3DXVECTOR3& coords)
{
	Coords = coords;
	Mesh->SetCoords(Coords);
}

D3DXVECTOR3 GRAPHICS_OBJECT::ObjCoords() const
{
	return Coords;
}

BOOL GRAPHICS_OBJECT::RenderMesh() const
{
	return Effects->RenderMesh(Mesh, TechNum);
}
