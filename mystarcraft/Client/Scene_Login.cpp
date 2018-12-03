#include "StdAfx.h"
#include "Scene_Login.h"

#include "Device.h"
#include "TextureMgr.h"
#include "Login_UIMgr.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

CScene_Login::CScene_Login(void)
{
}

CScene_Login::~CScene_Login(void)
{
	Release();
}

HRESULT CScene_Login::Initialize(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	if( 800 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"SCENE" , L"LOBY_SCENE800")->pTexture;
	else if( 640 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"SCENE" , L"LOBY_SCENE640")->pTexture;

	CKeyMgr::GetInstance()->SetInput_Device(SCENE_LOGIN);
	CLogin_UIMgr::GetInstance()->Initialize();

	CTimeMgr::GetInstance()->InitTime();

	CMouseMgr::GetInstance()->Initialize();
	return S_OK;
}

void CScene_Login::Update(void)
{
	CTimeMgr::GetInstance()->SetTime();
	CKeyMgr::GetInstance()->Update();
	CLogin_UIMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Lobby_Update();
}

void CScene_Login::Render(void)
{
	m_pDevice->GetSprite()->SetTransform(&m_matWorld);
	m_pDevice->GetSprite()->Draw(m_pBackTexture , 
		NULL , &D3DXVECTOR3(0.f , 0.f, 0.f), NULL
		, D3DCOLOR_ARGB(255,255,255,255));

	CLogin_UIMgr::GetInstance()->Render();

	CFontMgr::GetInstance()->FontRender();

	CMouseMgr::GetInstance()->Render();

	CTimeMgr::GetInstance()->FPS_fix();
}

void CScene_Login::Release(void)
{
	CLogin_UIMgr::DestroyInstance();
}
