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
#include "TimeMgr.h"

#include "Device.h"

#include "SCV.h"
#include "Medic.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Input_Stage.h"
CScene_Stage::CScene_Stage(void)
{
}

CScene_Stage::~CScene_Stage(void)
{
	Release();
}

HRESULT CScene_Stage::Initialize(void)
{
	m_fTime = 0.f;
	m_iFPS = 0;
	CKeyMgr::GetInstance()->SetInput_Device(SCENE_STAGE);
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

	CTimeMgr::GetInstance()->InitTime();
	return S_OK;
}

void CScene_Stage::Update(void)
{
	CTimeMgr::GetInstance()->SetTime();

	CKeyMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
	CScrollMgr::update();
	CComanderMgr::GetInstance()->Update();
	

	static float time = 0.f;
	time += GETTIME;

	CObjMgr::GetInstance()->Update();	

	//CArea_Mgr::GetInstance()->Areasize_debugrender(64 , 64);

	m_fTime += GETTIME;
}
void CScene_Stage::Render(void)
{
	CTileManager::GetInstance()->RenderTile();
	CObjMgr::GetInstance()->Render();

	//CTileManager::GetInstance()->RenderFog();
	//CTileManager::GetInstance()->Render_Flowfield();	
	
	//CLineMgr::GetInstance()->RenderGrid(32/*Å©±â*/, 128/*°¹¼ö*/);
	CLineMgr::GetInstance()->RectLineRender();

	CComanderMgr::GetInstance()->Render();

	CFontMgr::GetInstance()->FontRender();


	CTimeMgr::GetInstance()->FPS_fix();
	RenderFPS();
}
void CScene_Stage::RenderFPS(void)
{
	m_iFPS += 1;

	if(m_fTime >= 1.0f)
	{

		wsprintf(m_szFPS , L"FPS %d" , m_iFPS);
		m_fTime = 0.f;
		m_iFPS = 0;
	}
	CFontMgr::GetInstance()->FontRender(m_szFPS , 40 , 0 , D3DCOLOR_ARGB(255,0,255,0));
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
		GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	CObjMgr::GetInstance()->LoadObj(hFile);
}
