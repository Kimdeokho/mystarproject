#include "StdAfx.h"
#include "Gas_frag.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "MyMath.h"
CGas_frag::CGas_frag(CObj* pobj)
{
	m_pobj = pobj;
}

CGas_frag::~CGas_frag(void)
{
}

void CGas_frag::Initialize(void)
{
	m_eOBJ_NAME = OBJ_FRAGMENT;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_NONE;

	if(OBJ_SCV == m_pobj->GetOBJNAME())
		m_vecTex = CTextureMgr::GetInstance()->GetGeneralTexture(L"T_GAS_FRAG");
	if(OBJ_DRONE == m_pobj->GetOBJNAME())
		m_vecTex = CTextureMgr::GetInstance()->GetGeneralTexture(L"Z_GAS_FRAG");

	m_matWorld._11 = 1.2f;
	m_matWorld._22 = 1.2f;

	m_vPos = m_pobj->GetPos() + m_pobj->GetcurDir()*20;
}

void CGas_frag::Update(void)
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

void CGas_frag::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CGas_frag::Release(void)
{

}
