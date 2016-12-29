#include "StdAfx.h"
#include "GasResource.h"

#include "Device.h"
#include "TextureMgr.h"
#include "MyMouse.h"
#include "TileMgr.h"

CGasResource::CGasResource(void)
{
	m_TexName = L"Gas";
}

CGasResource::~CGasResource(void)
{
}

void CGasResource::InitRect(void)
{
	m_collRc.left = long(m_vPos.x - 64);
	m_collRc.right = long(m_vPos.x + 64);
	m_collRc.top = (long)(m_vPos.y - 32);
	m_collRc.bottom = long(m_vPos.y + 32);

	m_vRectPoint[0] = D3DXVECTOR2( float(m_collRc.left) , float(m_collRc.top) );
	m_vRectPoint[1] = D3DXVECTOR2( float(m_collRc.right) , float(m_collRc.top) );
	m_vRectPoint[2] = D3DXVECTOR2( float(m_collRc.right) , float(m_collRc.bottom) );
	m_vRectPoint[3] = D3DXVECTOR2( float(m_collRc.left) , float(m_collRc.bottom) );
	m_vRectPoint[4] = m_vRectPoint[0];
}
void CGasResource::Restore_TIleOption(void)
{
	int posX = (int)m_vPos.x + SQ_TILESIZEX/2;
	int posY = (int)m_vPos.y + SQ_TILESIZEY/2;

	int idx = (posY/SQ_TILESIZEY)*SQ_TILECNTX + (posX / SQ_TILESIZEX);
	idx = idx - SQ_TILECNTX - 2;

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 4; ++j)
			CTileMgr::GetInstance()->SetTileOption(idx + i*SQ_TILECNTX + j , MOVE_OK);
}
void CGasResource::Render(void)
{
	const vector<TEXINFO*>* vtemp = CTextureMgr::GetInstance()->GetGeneralTexture(m_TexName);

	m_matWorld._41 = m_vPos.x - CMyMouse::GetInstance()->GetScrollPt().x;
	m_matWorld._42 = m_vPos.y - CMyMouse::GetInstance()->GetScrollPt().y;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

	CDevice::GetInstance()->GetSprite()->Draw( (*vtemp)[0]->pTexture ,
		NULL , &D3DXVECTOR3(64.f, 32.f, 0.f) , NULL
		, D3DCOLOR_ARGB(255,255,255,255));

	CObj::DrawRect();
}
