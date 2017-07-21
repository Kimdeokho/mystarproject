#include "StdAfx.h"
#include "Scene_Stage.h"

#include "TileManager.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "LineMgr.h"
#include "ObjPoolMgr.h"
#include "Astar.h"
#include "UnitMgr.h"

CScene_Stage::CScene_Stage(void)
{
}

CScene_Stage::~CScene_Stage(void)
{
	Release();
}

HRESULT CScene_Stage::Initialize(void)
{
		
	CUnitMgr::GetInstance()->Initialize();
	CTileManager::GetInstance()->Initialize();
	LoadData();

	return S_OK;
}

void CScene_Stage::Update(void)
{
	CKeyMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();
}
void CScene_Stage::Render(void)
{
	CTileManager::GetInstance()->RenderTile();
	CObjMgr::GetInstance()->Render();

	//CTileManager::GetInstance()->RenderFog();
	CLineMgr::GetInstance()->RenderGrid(64 , 64);

	CFontMgr::GetInstance()->FontRender();
	CLineMgr::GetInstance()->LineRender();
}
void CScene_Stage::Release(void)
{
	CObjPoolMgr::DestroyInstance();
	CUnitMgr::DestroyInstance();
	//CAstar::DestroyInstance();
}

void CScene_Stage::LoadData(void)
{
	HANDLE hFile = CreateFile(L"../Data/map/test2.dat" , 
		GENERIC_READ , 0 , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	CObjMgr::GetInstance()->LoadObj(hFile);
}
