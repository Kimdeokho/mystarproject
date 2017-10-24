#include "StdAfx.h"
#include "Scene_Stage.h"

#include "TileManager.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "LineMgr.h"
#include "ComanderMgr.h"
#include "ObjPoolMgr.h"
#include "MouseMgr.h"

#include "Astar.h"
#include "UnitMgr.h"

#include "ScrollMgr.h"
#include "Area_Mgr.h"

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
	CComanderMgr::GetInstance()->Initialize();
	LoadData();

	return S_OK;
}

void CScene_Stage::Update(void)
{
	CKeyMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();

	CComanderMgr::GetInstance()->Update();
	CScrollMgr::update();

	CArea_Mgr::GetInstance()->Areasize_debugrender(64 , 64);
}
void CScene_Stage::Render(void)
{
	CTileManager::GetInstance()->RenderTile();
	CObjMgr::GetInstance()->Render();

	CTileManager::GetInstance()->RenderFog();
	//CTileManager::GetInstance()->Render_Flowfield();
	

	CFontMgr::GetInstance()->FontRender();
	CLineMgr::GetInstance()->RenderGrid(64/*Å©±â*/, 64/*°¹¼ö*/);
	CLineMgr::GetInstance()->LineRender();

	CComanderMgr::GetInstance()->Render();
}
void CScene_Stage::Release(void)
{
	CObjPoolMgr::DestroyInstance();
	CUnitMgr::DestroyInstance();
	CObjMgr::DestroyInstance();
	CComanderMgr::DestroyInstance();
	CMouseMgr::DestroyInstance();
	//CAstar::DestroyInstance();
}

void CScene_Stage::LoadData(void)
{
	HANDLE hFile = CreateFile(L"../Data/map/test3.dat" , 
		GENERIC_READ , 0 , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	CObjMgr::GetInstance()->LoadObj(hFile);
}
