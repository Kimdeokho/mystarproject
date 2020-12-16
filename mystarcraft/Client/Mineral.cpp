#include "StdAfx.h"
#include "Mineral.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ScrollMgr.h"
#include "Ingame_UIMgr.h"

#include "Com_Collision.h"
#include "TextureMgr.h"
#include "LineMgr.h"
#include "UI_Select.h"
#include "ObjMgr.h"
#include "FontMgr.h"
#include "TimeMgr.h"

#include "Workman.h"
CMineral::CMineral(void)
{
}

CMineral::~CMineral(void)
{
	Release();
}

void CMineral::Initialize(void)
{
	CObj::unit_area_Initialize();

	m_select_ui = NULL;
	//m_workman = NULL;

	D3DXMatrixIdentity(&m_matshadow);
	m_ecategory = CATEGORY_RESOURCE;
	m_eteamnumber = TEAM_NONE;
	m_sortID = SORT_GROUND;
	m_eOBJ_NAME = OBJ_MINERAL;

	m_unitinfo.eMoveType = MOVE_GROUND;

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

}
void CMineral::Update(void)
{

	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , int(m_workman_list.size()) , m_vPos.x ,m_vPos.y );
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_mineral_amount , m_vPos.x - CScrollMgr::m_fScrollX
	//	,m_vPos.y - CScrollMgr::m_fScrollY);

	//CObj::area_update();


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(!m_workman_list.empty())
	{
		list<CObj*>::iterator iter = m_workman_list.begin();
		list<CObj*>::iterator iter_end= m_workman_list.end();

		MYRECT<float> rc;
		float	collRange = 0;
		for( ; iter != iter_end; )
		{
			if(true == (*iter)->GetDestroy() )
			{
				iter = m_workman_list.erase(iter);
				continue;
			}
			//collRange = (*iter)->GetUnitinfo().fspeed * 3 * GETTIME ;

			//rc = (*iter)->GetMyRect();
			//rc.left -= collRange;
			//rc.right += collRange;
			//rc.top-= collRange;
			//rc.bottom += collRange;

			//if( false == MyIntersectrect(&m_rect , &rc) )
			//{
			//	((CWorkman*)(*iter))->SetMineral_mark(NULL);
			//	iter = m_workman_list.erase(iter);
			//}
			//else
			//	++iter;

			if(ORDER_RETURN_CARGO == (*iter)->GetUnitinfo().order)
			{
				((CWorkman*)(*iter))->SetMineral_mark(NULL);
				iter = m_workman_list.erase(iter);
			}
			else
				++iter;
		}
	}

	m_select_ui->Update();
}
void CMineral::Render(void)
{
	m_select_ui->Render();

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_matshadow = m_matWorld;
	m_matshadow._41 -= 8;
	m_matshadow._42 -= 8;

	m_pSprite->SetTransform(&m_matshadow);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(32,48,0) , NULL , D3DCOLOR_ARGB(100,0,0,0));

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(32,48,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}
bool CMineral::Setworkman( CObj* pworkman)
{
	//if(NULL == m_workman)
	//	m_workman = pworkman;

	if( m_workman_list.size() < 2)
	{
		//list<CObj*>::iterator iter = m_workman_list.begin();
		//list<CObj*>::iterator iter_end= m_workman_list.end();

		//for( ; iter != iter_end; ++iter)
		//{
		//	if( pworkman == (*iter))
		//		return true;
		//}

		m_workman_list.push_back(pworkman);
		return true;
	}
	else
	{
		return false;
	}
}
bool CMineral::decrease_workman(CObj* pworkman)
{
	if(!m_workman_list.empty())
	{
		list<CObj*>::iterator iter = m_workman_list.begin();
		list<CObj*>::iterator iter_end= m_workman_list.end();

		for( ; iter != iter_end; ++iter)
		{
			if( pworkman == (*iter))
			{
				m_workman_list.erase(iter);
				break;
				//return true;
			}
		}
	}
	m_workmancnt -= 1;
	return true;
}
unsigned CMineral::Getworkman_count(void)
{
	return m_workman_list.size();
}
bool CMineral::Getworkman(CObj* pworkman)
{
	if(!m_workman_list.empty())
	{
		if(pworkman == m_workman_list.front())
			return true;
		else
			return false;
	}
	return false;
}
bool CMineral::Check_workman(CObj* pworkman)
{
	if(!m_workman_list.empty())
	{
		list<CObj*>::iterator iter = m_workman_list.begin();
		list<CObj*>::iterator iter_end= m_workman_list.end();

		for( ; iter != iter_end; ++iter)
		{
			if( pworkman == (*iter))
				return true;
		}
	}
	return false;
}
void CMineral::Update_Wireframe(void)
{
	CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state);
}
void CMineral::SetMineral_amount(const int& iamount)
{
	m_mineral_amount += iamount;
}
void CMineral::Release(void)
{
	if(!m_workman_list.empty())
	{
		list<CObj*>::iterator iter = m_workman_list.begin();
		list<CObj*>::iterator iter_end= m_workman_list.end();

		for( ; iter != iter_end; ++iter)
		{
			((CWorkman*)(*iter))->SetMineral_mark(NULL);
		}
		m_workman_list.clear();
	}

	Safe_Delete(m_select_ui);
}