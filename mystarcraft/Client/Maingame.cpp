#include "StdAfx.h"
#include "Maingame.h"

#include "TimeMgr.h"
#include "SceneMgr.h"
#include "FontMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TileManager.h"
#include "ObjMgr.h"
#include "MouseMgr.h"
#include "KeyMgr.h"
#include "Area_Mgr.h"
#include "LineMgr.h"


CMaingame::CMaingame(void)
{
	m_pLoding = NULL;
}

CMaingame::~CMaingame(void)
{
	Release();
}

HRESULT CMaingame::Initialize(void)
{
	//공중유닛은 Y소팅을 하지 않는다 그냥 생산된 순서대로 렌더를 실행한다

	AllocConsole();
	freopen( "CONOUT$",  "wt", stdout);

	m_fTime = 0.f;
	m_iFPS = 0;
	srand(unsigned int(time(NULL)));
	lstrcpy(m_szFPS , L"hello world");
	CDevice::GetInstance()->InitDevice();
	
	CTimeMgr::GetInstance()->InitTime();
	CTimeMgr::GetInstance()->SetFps(&m_iFPS);
	CFontMgr::GetInstance()->Initialize();
	CLineMgr::GetInstance()->Initialize();

	m_pFont = CFontMgr::GetInstance();
	m_pDevice = CDevice::GetInstance();
	CKeyMgr::GetInstance();

	CSceneMgr::GetInstance()->SetScene(SCENE_LOGO);

	
	return S_OK;
}

void CMaingame::Update(void)
{
	CTimeMgr::GetInstance()->SetTime();

	CSceneMgr::GetInstance()->Update();

	m_fTime += GETTIME;
}

void CMaingame::Render(void)
{	
	m_pDevice->GetDevice()->Clear(0 , NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_XRGB(0,0,255), 1.f , 0);


	m_pDevice->Render_Begin();



	CSceneMgr::GetInstance()->Render();

	RenderFPS();

	m_pDevice->Render_End();	
	m_pDevice->GetDevice()->Present(NULL, NULL , NULL , NULL);
}
void CMaingame::RenderFPS(void)
{
	//m_fTime += GETTIME;
	m_iFPS += 1;
	if(m_fTime >= 1.0f)
	{
		wsprintf(m_szFPS , L"FPS %d" , m_iFPS);
		m_fTime = 0.f;
		m_iFPS = 0;
	}
	m_pFont->FontRender(m_szFPS , 40 , 0 , D3DCOLOR_ARGB(255,0,255,0));
}

void CMaingame::Release(void)
{
	CObjMgr::DestroyInstance();
	CTimeMgr::DestroyInstance();
	CSceneMgr::DestroyInstance();
	CFontMgr::DestroyInstance();
	CDevice::DestroyInstance();
	CTextureMgr::DestroyInstance();
	CTileManager::DestroyInstance();
	
	CKeyMgr::DestroyInstance();
	CArea_Mgr::DestroyInstance();
	CLineMgr::DestroyInstance();

	FreeConsole();
}