#include "StdAfx.h"
#include "Scene_Room.h"

#include "Device.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "TextureMgr.h"
#include "Room_UIMgr.h"
#include "Loby_UIMgr.h"

#include "RoomSession_Mgr.h"
#include "TimeMgr.h"
#include "Session_Mgr.h"
#include "RoomSession_Mgr.h"
CScene_Room::CScene_Room(void)
{
}

CScene_Room::~CScene_Room(void)
{
	Release();
}

HRESULT CScene_Room::Initialize(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	if( 800 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"SCENE" , L"LOBY_SCENE800")->pTexture;
	else if( 640 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"SCENE" , L"LOBY_SCENE640")->pTexture;

	CKeyMgr::GetInstance()->SetInput_Device(SCENE_ROOM);
	CRoom_UIMgr::GetInstance()->Initialize();
	CRoomSession_Mgr::GetInstance()->Initialize();

	CTimeMgr::GetInstance()->InitTime();

	return S_OK;
}

void CScene_Room::Update(void)
{
	CTimeMgr::GetInstance()->SetTime();
	CKeyMgr::GetInstance()->Update();
	CRoom_UIMgr::GetInstance()->Update();
}

void CScene_Room::Render(void)
{
	m_pDevice->GetSprite()->SetTransform(&m_matWorld);
	m_pDevice->GetSprite()->Draw(m_pBackTexture , 
		NULL , &D3DXVECTOR3(0.f , 0.f, 0.f), NULL
		, D3DCOLOR_ARGB(255,255,255,255));

	CRoom_UIMgr::GetInstance()->Render();
	CFontMgr::GetInstance()->FontRender();

	CTimeMgr::GetInstance()->FPS_fix();
}

void CScene_Room::Release(void)
{
	CRoom_UIMgr::DestroyInstance();
	CRoomSession_Mgr::DestroyInstance();
}
