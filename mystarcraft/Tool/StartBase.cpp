#include "StdAfx.h"
#include "StartBase.h"

#include "Device.h"
#include "TextureMgr.h"
#include "MyMouse.h"
#include "TileMgr.h"
CStartBase::CStartBase(void)
{
	m_ObjName = L"Startbase";
}

CStartBase::~CStartBase(void)
{
}
void CStartBase::InitRect(void)
{
	m_vertex.left = 64;
	m_vertex.right = 64;
	m_vertex.bottom = 48;
	m_vertex.top = 48;

	CObj::InitCollRC();
}
void CStartBase::Initialize(void)
{
	InitRect();
}

void CStartBase::Render(void)
{
	const TEXINFO* vtemp = CTextureMgr::GetInstance()->GetSingleTexture(L"ETC", m_ObjName);

	m_matWorld._41 = m_vPos.x - CMyMouse::GetInstance()->GetScrollPt().x;
	m_matWorld._42 = m_vPos.y - CMyMouse::GetInstance()->GetScrollPt().y;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

	CDevice::GetInstance()->GetSprite()->Draw( vtemp->pTexture ,
		NULL , &D3DXVECTOR3(64.f, 48.f, 0.f) , NULL
		, D3DCOLOR_ARGB(255,255,255,255));

}

void CStartBase::Restore_TIleOption(void)
{
	int posX = (int)m_vPos.x + SQ_TILESIZEX/2;
	int posY = (int)m_vPos.y;

	int idx = (posY/SQ_TILESIZEY)*SQ_TILECNTX + (posX / SQ_TILESIZEX);
	idx = idx - SQ_TILECNTX - 2;

	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 4; ++j)
			CTileMgr::GetInstance()->SetTileOption(idx + i*SQ_TILECNTX + j , MOVE_OK);
}
