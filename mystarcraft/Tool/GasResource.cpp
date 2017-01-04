#include "StdAfx.h"
#include "GasResource.h"

#include "Device.h"
#include "TextureMgr.h"
#include "MyMouse.h"
#include "TileMgr.h"

CGasResource::CGasResource(void)
{
	m_ObjName = L"Gas";
}

CGasResource::~CGasResource(void)
{
}

void CGasResource::InitRect(void)
{
	m_vertex.left = 64;
	m_vertex.right = 64;
	m_vertex.bottom = 32;
	m_vertex.top = 32;

	CObj::InitCollRC();
}
void CGasResource::Initialize(void)
{
	InitRect();
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
	const vector<TEXINFO*>* vtemp = CTextureMgr::GetInstance()->GetGeneralTexture(m_ObjName);

	m_matWorld._41 = m_vPos.x - CMyMouse::GetInstance()->GetScrollPt().x;
	m_matWorld._42 = m_vPos.y - CMyMouse::GetInstance()->GetScrollPt().y;
	m_matWorld._11 = 1;
	m_matWorld._22 = 1;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

	CDevice::GetInstance()->GetSprite()->Draw( (*vtemp)[0]->pTexture ,
		NULL , &D3DXVECTOR3(64.f, 32.f, 0.f) , NULL
		, D3DCOLOR_ARGB(255,255,255,255));

	CObj::DrawRect();
}

void CGasResource::MinmapRender(const float fratioX , const float fratioY)
{
	const vector<TEXINFO*>* vtemp = CTextureMgr::GetInstance()->GetGeneralTexture(m_ObjName);

	m_matWorld._41 = m_vPos.x*fratioX;
	m_matWorld._42 = m_vPos.y*fratioY;
	m_matWorld._11 = fratioX;
	m_matWorld._22 = fratioY;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

	CDevice::GetInstance()->GetSprite()->Draw( (*vtemp)[0]->pTexture ,
		NULL , &D3DXVECTOR3(64.f, 32.f, 0.f) , NULL
		, D3DCOLOR_ARGB(255,255,255,255));

}
