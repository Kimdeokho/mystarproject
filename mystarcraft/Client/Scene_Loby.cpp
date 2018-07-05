#include "StdAfx.h"
#include "Scene_Loby.h"

#include "Device.h"
#include "TextureMgr.h"
#include "Login_UIMgr.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "TimeMgr.h"

#include "Loby_UIMgr.h"
#include "Session_Mgr.h"

CScene_Loby::CScene_Loby(void)
{
}

CScene_Loby::~CScene_Loby(void)
{
	Release();
}

HRESULT CScene_Loby::Initialize(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	if( 800 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"SCENE" , L"LOBY_SCENE800")->pTexture;
	else if( 640 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"SCENE" , L"LOBY_SCENE640")->pTexture;

	CKeyMgr::GetInstance()->SetInput_Device(SCENE_LOBY);
	CLoby_UIMgr::GetInstance()->Initialize();

	CTimeMgr::GetInstance()->InitTime();

	WRITE_TCP_PACKET(PT_ROOMLIST_RENEWAL , WriteBuffer
		, WRITE_PT_ROOMLIST_RENEWAL(WriteBuffer));
	return S_OK;
}

void CScene_Loby::Update(void)
{
	CTimeMgr::GetInstance()->SetTime();

	CKeyMgr::GetInstance()->Update();
	CLoby_UIMgr::GetInstance()->Update();
}

void CScene_Loby::Render(void)
{
	m_pDevice->GetSprite()->SetTransform(&m_matWorld);
	m_pDevice->GetSprite()->Draw(m_pBackTexture , 
		NULL , &D3DXVECTOR3(0.f , 0.f, 0.f), NULL
		, D3DCOLOR_ARGB(255,255,255,255));

	CLoby_UIMgr::GetInstance()->Render();
	CFontMgr::GetInstance()->FontRender();

	CTimeMgr::GetInstance()->FPS_fix();
}

void CScene_Loby::Release(void)
{
	CLoby_UIMgr::DestroyInstance();
}
