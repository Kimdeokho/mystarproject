#include "StdAfx.h"
#include "Mineral_frag.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "MyMath.h"

CMineral_frag::CMineral_frag(CObj* pobj)
{
	m_pobj = pobj;
}

CMineral_frag::~CMineral_frag(void)
{
}

void CMineral_frag::Initialize(void)
{
	m_eOBJ_NAME = OBJ_FRAGMENT;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_NONE;
	m_vecTex = CTextureMgr::GetInstance()->GetGeneralTexture(L"MINERAL_FRAG");

	m_matWorld._11 = 1.2f;
	m_matWorld._22 = 1.2f;

	m_vPos = m_pobj->GetPos() + m_pobj->GetcurDir()*20;
}

void CMineral_frag::Update(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos , 64);

	if(m_oldidx32 != m_curidx32)
	{
		m_oldidx32 = m_curidx32;		
	}

	if(m_oldidx64 != m_curidx64)
	{
		m_oldidx64 = m_curidx64;
	}

	m_vPos = m_pobj->GetPos() + m_pobj->GetcurDir()*20;
}

void CMineral_frag::Render(void)
{
	
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CMineral_frag::Release(void)
{

}
