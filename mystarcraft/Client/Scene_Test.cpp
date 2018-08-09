#include "StdAfx.h"
#include "Scene_Test.h"

#include "TileManager.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "LineMgr.h"
#include "Ingame_UIMgr.h"
#include "ObjPoolMgr.h"
#include "MouseMgr.h"

#include "UnitMgr.h"

#include "ScrollMgr.h"
#include "Area_Mgr.h"
#include "TimeMgr.h"
#include "Session_Mgr.h"

#include "Device.h"

#include "SCV.h"
#include "Medic.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Input_Test.h"
CScene_Test::CScene_Test(void)
{
}

CScene_Test::~CScene_Test(void)
{
	Release();
}

HRESULT CScene_Test::Initialize(void)
{
	m_fTime = 0.f;
	m_iFPS = 0;
	CKeyMgr::GetInstance()->SetInput_Device(SCENE_TEST);
	CUnitMgr::GetInstance()->Initialize();
	CTileManager::GetInstance()->Initialize();
	CIngame_UIMgr::GetInstance()->Initialize(TERRAN);
	CSession_Mgr::GetInstance()->SetTeamNumber(TEAM_0);
	LoadData();

	CObj* pobj = NULL;
	D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
	for(int i = 0; i < 10; ++i)
	{
		vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		vpos.x = 1000;
		vpos.y = 1000;
		pobj = new CSCV;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_SCV);
		pobj->SetPos(vpos);
		pobj->SetTeamNumber(TEAM_0);
		pobj->Initialize();

		pobj = new CMedic;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_MEDIC);
		pobj->SetPos(vpos);
		pobj->SetTeamNumber(TEAM_0);
		pobj->Initialize();
	}

	CTimeMgr::GetInstance()->InitTime();

	//SetFocus(NULL);
	return S_OK;
}

void CScene_Test::Update(void)
{
	CTimeMgr::GetInstance()->SetTime();

	CKeyMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
	CScrollMgr::update();
	CIngame_UIMgr::GetInstance()->Update();


	static float time = 0.f;
	time += GETTIME;

	CObjMgr::GetInstance()->Update();	

	//CArea_Mgr::GetInstance()->Areasize_debugrender(64 , 64);

	m_fTime += GETTIME;
}

void CScene_Test::Render(void)
{
	CTileManager::GetInstance()->RenderTile();
	CObjMgr::GetInstance()->Render();

	//CTileManager::GetInstance()->RenderFog();
	//CTileManager::GetInstance()->Render_Flowfield();	

	//CLineMgr::GetInstance()->RenderGrid(32/*크기*/, 128/*갯수*/);
	CLineMgr::GetInstance()->RectLineRender();

	CIngame_UIMgr::GetInstance()->Render();

	CFontMgr::GetInstance()->FontRender();


	CTimeMgr::GetInstance()->FPS_fix();
	RenderFPS();
}

void CScene_Test::Release(void)
{
	//CObjPoolMgr::DestroyInstance();
	CUnitMgr::DestroyInstance();
	CObjMgr::DestroyInstance();
	CIngame_UIMgr::DestroyInstance();
	CMouseMgr::DestroyInstance();
	//CAstar::DestroyInstance();
}

void CScene_Test::LoadData(void)
{
	int basecnt = 0;
	DWORD dwbyte = 0;
	HANDLE hFile = CreateFile(L"../Data/map/pyhon.dat" , 
		GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL);

	ReadFile(hFile , &basecnt, sizeof(int), &dwbyte, NULL); //임시,
	CloseHandle(hFile);

	hFile = CreateFile(L"../Data/map/pyhon.dat" , 
		GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	CObjMgr::GetInstance()->LoadObj(hFile);

	CloseHandle(hFile);
}

void CScene_Test::RenderFPS(void)
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
