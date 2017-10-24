#include "StdAfx.h"
#include "Mineral.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ScrollMgr.h"

#include "Com_Collision.h"
#include "TextureMgr.h"
#include "LineMgr.h"
#include "UI_Select.h"
#include "ObjMgr.h"
#include "FontMgr.h"
CMineral::CMineral(void)
{
}

CMineral::~CMineral(void)
{
}

void CMineral::Initialize(void)
{
	CObj::unit_area_Initialize();

	m_select_ui = NULL;
	m_workman = NULL;

	D3DXMatrixIdentity(&m_matshadow);
	m_ecategory = RESOURCE;
	m_eteamnumber = TEAM_NONE;
	m_sortID = SORT_GROUND;
	m_eOBJ_NAME = OBJ_MINERAL;

	m_mineral_amount = 50000;

	m_vertex.left = 32;
	m_vertex.right = 32;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_workmancnt = 0;
	
	m_vecTex = CTextureMgr::GetInstance()->GetGeneralTexture(L"Mineral");

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex)) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		(iter)->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select72" , m_vPos , 9);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);
}
void CMineral::Update(void)
{
	
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_mineral_amount , m_vPos.x - CScrollMgr::m_fScrollX
	//	,m_vPos.y - CScrollMgr::m_fScrollY);

	CObj::area_update();


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();

}
void CMineral::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_matshadow = m_matWorld;
	m_matshadow._41 -= 8;
	m_matshadow._42 -= 8;

	m_pSprite->SetTransform(&m_matshadow);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(32,48,0) , NULL , D3DCOLOR_ARGB(100,0,0,0));

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(32,48,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));

	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}
void CMineral::Setworkman( const CObj* pworkman)
{
	m_workman = pworkman;
}
void CMineral::decrease_workman()
{
	m_workman = NULL;
	m_workmancnt -= 1;
}
const CObj* CMineral::Getworkman(void)
{
	return m_workman;
}
void CMineral::Release(void)
{

}

void CMineral::SetMineral_amount(const int& iamount)
{
	m_mineral_amount += iamount;
}

