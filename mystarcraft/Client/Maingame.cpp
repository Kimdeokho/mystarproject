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
#include "Session_Mgr.h"
#include "RoomSession_Mgr.h"
#include "SoundDevice.h"


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
/*
	_CrtSetBreakAlloc(250552);
	_CrtMemDumpAllObjectsSince(0); */

	//AllocConsole();
	//freopen( "CONOUT$",  "wt", stdout);

	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);	// 2.2 버전을 로드합니다.

	m_fTime = 0.f;
	m_iFPS = 0;
	srand(unsigned int(time(NULL)));
	lstrcpy(m_szFPS , L"hello world");
	CDevice::GetInstance()->InitDevice();
	
	//CTimeMgr::GetInstance()->InitTime();
	CTimeMgr::GetInstance()->SetFps(&m_iFPS);
	CFontMgr::GetInstance()->Initialize();
	CLineMgr::GetInstance()->Initialize();
	CSession_Mgr::GetInstance()->Initialize();
	CSoundDevice::GetInstance()->Init();
	

	m_pFont = CFontMgr::GetInstance();
	m_pDevice = CDevice::GetInstance();

	CSceneMgr::GetInstance()->SetScene(SCENE_LOGO);

	srand(unsigned(10));
	return S_OK;
}

void CMaingame::Update(void)
{
	//CTimeMgr::GetInstance()->SetTime();

	CSession_Mgr::GetInstance()->Update();
	CSceneMgr::GetInstance()->Update();	
}

void CMaingame::Render(void)
{		
	//뷰포트 또는 뷰포트내의 직사각형 세트를 지정한 RGBA 색에 클리어 해, 
	//깊이 버퍼를 클리어 해, 스텐실 버퍼를 삭제한다.
	m_pDevice->GetDevice()->Clear(0 , NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_XRGB(0,0,0), 1.f , 0);


	m_pDevice->Render_Begin();

	CSceneMgr::GetInstance()->Render();	

	m_pDevice->Render_End();	
	m_pDevice->GetDevice()->Present(NULL, NULL , NULL , NULL);
	//장치가 소유하는 백 버퍼의 순서 중(안)에서, 다음의 버퍼의 컨텐츠를 제시한다.

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
	CSession_Mgr::DestroyInstance();
	
	CKeyMgr::DestroyInstance();
	CArea_Mgr::DestroyInstance();
	CLineMgr::DestroyInstance();
	CRoomSession_Mgr::DestroyInstance();
	CSoundDevice::DestroyInstance();

	CStream::Extingtion_pointer();

	//FreeConsole();
}