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

#include "SCV.h"
#include "Medic.h"

#include "MyMath.h"
#include "ScrollMgr.h"
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
	CComanderMgr::GetInstance()->Initialize(TERRAN);
	LoadData();

	CObj* pobj = NULL;
	D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
	for(int i = 0; i < 10; ++i)
	{
		vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		vpos.x = CScrollMgr::m_fScrollX;
		vpos.y = CScrollMgr::m_fScrollY;
		pobj = new CSCV;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_SCV);
		pobj->SetPos(vpos);
		pobj->Initialize();

		pobj = new CMedic;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_MEDIC);
		pobj->SetPos(vpos);
		pobj->Initialize();
	}

	return S_OK;
}

void CScene_Stage::Update(void)
{
	CKeyMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();

	CComanderMgr::GetInstance()->Update();
	CScrollMgr::update();

	//CArea_Mgr::GetInstance()->Areasize_debugrender(64 , 64);
}
void CScene_Stage::Render(void)
{
	CTileManager::GetInstance()->RenderTile();
	CObjMgr::GetInstance()->Render();

	CTileManager::GetInstance()->RenderFog();
	//CTileManager::GetInstance()->Render_Flowfield();	
	
	//CLineMgr::GetInstance()->RenderGrid(32/*Å©±â*/, 128/*°¹¼ö*/);
	CLineMgr::GetInstance()->RectLineRender();

	CComanderMgr::GetInstance()->Render();

	CFontMgr::GetInstance()->FontRender();
}
void CScene_Stage::Release(void)
{
	//CObjPoolMgr::DestroyInstance();
	CUnitMgr::DestroyInstance();
	CObjMgr::DestroyInstance();
	CComanderMgr::DestroyInstance();
	CMouseMgr::DestroyInstance();
	//CAstar::DestroyInstance();
}

void CScene_Stage::LoadData(void)
{
	HANDLE hFile = CreateFile(L"../Data/map/test4.dat" , 
		GENERIC_READ , 0 , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	CObjMgr::GetInstance()->LoadObj(hFile);
}
