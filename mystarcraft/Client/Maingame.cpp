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
	m_fTime = 0.f;
	m_iFPS = 0;
	lstrcpy(m_szFPS , L"");
	CDevice::GetInstance()->InitDevice();
	
	CTimeMgr::GetInstance()->InitTime();
	CFontMgr::GetInstance()->Initialize();

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

	//for(int i = 1; i < 20000; ++i)
	//{
	//	int a = 10+i;
	//}
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
	m_fTime += GETTIME;
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
	CTimeMgr::DestroyInstance();
	CSceneMgr::DestroyInstance();
	CFontMgr::DestroyInstance();
	CDevice::DestroyInstance();
	CTextureMgr::DestroyInstance();
	CTileManager::DestroyInstance();
	CObjMgr::DestroyInstance();
	CKeyMgr::DestroyInstance();
}