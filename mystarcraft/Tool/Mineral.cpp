#include "StdAfx.h"
#include "Mineral.h"

#include "Device.h"
#include "TextureMgr.h"
#include "MyMouse.h"
#include "TileMgr.h"

CMineral::CMineral(void)
{
	m_ObjName = L"Mineral";
}
CMineral::~CMineral(void)
{
}

void CMineral::InitRect(void)
{
	m_vertex.left = 32;
	m_vertex.right = 32;
	m_vertex.bottom = 16;
	m_vertex.top = 16;

	CObj::InitCollRC();

}
void CMineral::Initialize()
{
	InitRect();
}
void CMineral::Restore_TIleOption(void)
{
	int posX = (int)m_vPos.x + SQ_TILESIZEX/2;
	int posY = (int)m_vPos.y;

	int idx = (posY/SQ_TILESIZEY)*SQ_TILECNTX + (posX / SQ_TILESIZEX);

	for(int i = idx - 1; i < idx + 1; ++i)
		CTileMgr::GetInstance()->SetTileOption(i , MOVE_OK);
}
void CMineral::Render(void)
{

	const vector<TEXINFO*>* vtemp = CTextureMgr::GetInstance()->GetGeneralTexture(m_ObjName);

	m_matWorld._41 = m_vPos.x - CMyMouse::GetInstance()->GetScrollPt().x;
	m_matWorld._42 = m_vPos.y - CMyMouse::GetInstance()->GetScrollPt().y;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

	CDevice::GetInstance()->GetSprite()->Draw( (*vtemp)[0]->pTexture ,
		NULL , &D3DXVECTOR3(32.f, 48.f, 0.f) , NULL
		, D3DCOLOR_ARGB(255,255,255,255));

	CObj::DrawRect();
}
