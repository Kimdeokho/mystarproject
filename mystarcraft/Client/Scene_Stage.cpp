#include "StdAfx.h"
#include "Scene_Stage.h"

#include "TileManager.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "LineMgr.h"
#include "ObjPoolMgr.h"
#include "Astar.h"

CScene_Stage::CScene_Stage(void)
{
}

CScene_Stage::~CScene_Stage(void)
{
	Release();
}

HRESULT CScene_Stage::Initialize(void)
{
		
	CTileManager::GetInstance()->Initialize();
	LoadData();

	return S_OK;
}

void CScene_Stage::Update(void)
{
	CKeyMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();

	CFontMgr::GetInstance()->Setnumber_combine_Font(L"OBJ NUM%d" , CObjPoolMgr::m_Obj_Cnt , 400, 300);
}
void CScene_Stage::Render(void)
{
	CTileManager::GetInstance()->RenderTile();
	CObjMgr::GetInstance()->Render();

	CTileManager::GetInstance()->RenderFog();

	CFontMgr::GetInstance()->FontRender();
	CLineMgr::GetInstance()->LineRender();
}
void CScene_Stage::Release(void)
{
	CObjPoolMgr::DestroyInstance();
	//CAstar::DestroyInstance();
}

void CScene_Stage::LoadData(void)
{
	HANDLE hFile = CreateFile(L"../Data/map/test2.dat" , 
		GENERIC_READ , 0 , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	CObjMgr::GetInstance()->LoadObj(hFile);
}
