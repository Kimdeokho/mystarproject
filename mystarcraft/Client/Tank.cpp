#include "StdAfx.h"
#include "Tank.h"

#include "Tankbody.h"
#include "Tankbarrel.h"

#include "Siegebody.h"
#include "Siegebarrel.h"


#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_Pathfind.h"
#include "Com_TankbodyAnim.h"
#include "Com_SiegebodyAnim.h"

#include "TileManager.h"

#include "ScrollMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "MyMath.h"
#include "UnitMgr.h"
#include "GeneraEff.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "UI_Select.h"
CTank::CTank(void)
{
	m_tankbarrel = NULL;

}

CTank::~CTank(void)
{
	Release();
}

void CTank::Initialize(void)
{
	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	CObj::unit_area_Initialize();
	CUnit::Initialize();

	m_sortID = SORT_GROUND;	
	m_ecategory = UNIT;
	m_eteamnumber = TEAM_0;


	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_NOMAL;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.damage = 0;
	m_unitinfo.hp = 150;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 68;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 16;
	m_vertex.right = 16;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_bsiegemode = false;
	m_btransform_ready = false;


	m_tankbarrel = new CTankbarrel(this);
	m_tankbarrel->SetObjID(m_obj_id);



	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 , 16);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex , true);
	m_com_anim = new CCom_TankbodyAnim(m_matWorld , m_curtex);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_tankbarrel->Initialize();//포신은 제일 마지막에 초기화

	m_select_ui = new CUI_Select(L"Select62" , m_vPos , 8);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);
}

void CTank::Update(void)
{
	CObj::area_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();




	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(MOVE == m_unitinfo.estate || COLLISION == m_unitinfo.estate)
	{
		if(true == m_bsiegemode )
		{
		}
		else
		{
			((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
			m_tankbarrel->Setdir(m_vcurdir);
		}
	}
	else if(TRANSFORMING == m_unitinfo.estate)
	{
		if(true == m_btransform_ready)
		{
			if(true == ((CCom_Animation*)m_com_anim)->GetAnimation_end())
			{
				m_unitinfo.estate = IDLE;

				if(false == m_bsiegemode)
				{
					m_bsiegemode = true;
					((CTankbarrel*)m_tankbarrel)->TransformSiegebarrel();
					
				}
				else
				{
					//시즈모드 상태였다면
					m_bsiegemode = false;
					Transform_Tankbody();
					((CTankbarrel*)m_tankbarrel)->TransformTankbarrel();
				}
				m_btransform_ready = false;
			}			
		}
		else
		{
			if(false == m_bsiegemode)
			{
				if( true == ((CCom_Animation*)m_com_anim)->GetRotationComplete() &&
					true == ((CTankbarrel*)m_tankbarrel)->GetTransformReady())
				{
					m_btransform_ready = true; // 변신 준비완료
					Transform_Siegebody();
				}
			}
		}

	}

	if(ORDER_NONE == m_unitinfo.eorder)
	{
		m_tankbarrel->SetOrder(ORDER_NONE);
	}

	barrel_sync();

	m_tankbarrel->Update();

}

void CTank::Render(void)
{

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_matWorld);
	if(TEAM_1 == m_eteamnumber)
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,0,0));
	}
	else
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}

	//CLineMgr::GetInstance()->collisionbox_render(m_rect);

	m_tankbarrel->Render();

	if(NULL != m_com_pathfind)
		m_com_pathfind->Render();
}
void CTank::Transform(void)
{
	((CCom_Animation*)m_com_anim)->SetAnimation(L"SIEGEBODY_TRANS");
}
void CTank::Transform_Tankbody(void)
{

	//변신한다
	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_ANIMATION);
	Safe_Delete(m_com_anim);
	m_componentlist.erase(iter);


	m_com_anim = new CCom_TankbodyAnim(m_matWorld , m_curtex);
	m_com_anim->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32, 16);
	m_com_pathfind->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind));

	((CCom_Collision*)m_com_collision)->SetCollsearch(true);

	//변신 끝
	m_btransform_ready = false;
	
}

void CTank::Transform_Siegebody(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
	m_componentlist.erase(iter);			

	iter = m_componentlist.find(COM_ANIMATION);
	m_componentlist.erase(iter);

	Safe_Delete(m_com_pathfind);
	Safe_Delete(m_com_anim);

	m_com_anim = new CCom_SiegebodyAnim(m_matWorld , m_curtex);
	m_com_anim->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));

	((CCom_Collision*)m_com_collision)->SetCollsearch(false);
}

void CTank::barrel_sync(void)
{
	float fradian = CMyMath::dgree_to_radian( m_curdiridx*22.5f );
	D3DXVECTOR2 vbarrelpos;

	if( -1 == m_matWorld._11 )
	{
		if(m_curdiridx <= 3)
		{
			vbarrelpos.x = sinf(fradian)*13;
			vbarrelpos.y = cosf(fradian)*6;
		}
		else if(m_curdiridx == 4)
		{
			vbarrelpos.x = sinf(fradian)*13;
			vbarrelpos.y = -7;
		}
		else
		{
			vbarrelpos.x = sinf(fradian)*10;
			vbarrelpos.y = cosf(fradian)*17;
		}

	}
	else
	{
		if(m_curdiridx <= 3)
		{
			vbarrelpos.x = -sinf(fradian)*13;
			vbarrelpos.y = cosf(fradian)*6;
		}
		else if(m_curdiridx == 4)
		{
			vbarrelpos.x = -sinf(fradian)*13;
			vbarrelpos.y = -7;
		}
		else
		{
			vbarrelpos.x = -sinf(fradian)*10;
			vbarrelpos.y = cosf(fradian)*17;
		}
	}

	vbarrelpos += m_vPos;
	((CTankbarrel*)m_tankbarrel)->SetbarrelPos(m_vPos , vbarrelpos);

}
void CTank::Inputkey_reaction(const int& nkey)
{
	if( 'O' == nkey)
	{
		//시즈모드 업글이 되어있따면

		if(TRANSFORMING != m_unitinfo.estate)
		{
			m_unitinfo.estate = TRANSFORMING;
			if(false == m_bsiegemode)
			{				
				m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);
			}
		}

	}
	if( 'W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		m_tankbarrel->SetTeamNumber(TEAM_1);
	}
	if(VK_RBUTTON == nkey)
	{
		if(true == m_bsiegemode)
		{
		}
		else
		{
			if(TRANSFORMING != m_unitinfo.estate)
			{
				m_unitinfo.estate = MOVE;
				m_unitinfo.eorder = ORDER_MOVE;
				m_tankbarrel->SetOrder(ORDER_MOVE);
				m_tankbarrel->SetState(MOVE);

				if(NULL != m_com_pathfind)
				{
					D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();


					((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos);
					((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CMyMath::Pos_to_index(goalpos ,32));
					((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
					((CCom_Pathfind*)m_com_pathfind)->StartPathfinding(m_bmagicbox);
					m_bmagicbox = false;
				}
			}

		}
	}

	m_tankbarrel->Inputkey_reaction(nkey);
}

void CTank::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		if(true == m_bsiegemode)
		{

		}
		else
		{
			if(TRANSFORMING != m_unitinfo.estate)
			{
				m_unitinfo.eorder = ORDER_MOVE_ATTACK;
				m_unitinfo.estate = MOVE;
				m_tankbarrel->SetOrder(ORDER_MOVE_ATTACK);
				m_tankbarrel->SetState(MOVE);
				

				if(NULL != m_com_pathfind)
				{
					D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();


					((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos);
					((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CMyMath::Pos_to_index(goalpos ,32));
					((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
					((CCom_Pathfind*)m_com_pathfind)->StartPathfinding(m_bmagicbox);
					m_bmagicbox = false;
				}
			}

		}

	}

	m_tankbarrel->Inputkey_reaction(firstkey , secondkey);
}
bool CTank::GetTransformReady(void)
{
	return m_btransform_ready;
}
void CTank::SetTransformReady(bool btransform_ready)
{
	m_btransform_ready = btransform_ready;
}
void CTank::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	Safe_Delete(m_tankbarrel);
}

CComponent* CTank::GetComponent(COMPONENT_LIST ecom_name)
{
	if(COM_TARGET_SEARCH == ecom_name)
		return m_tankbarrel->GetComponent(ecom_name);
	else
	{
		COMPONENT_PAIR::iterator iter;
		iter = m_componentlist.find(ecom_name);

		if(iter != m_componentlist.end())
			return iter->second;

		return NULL;
	}
}

void CTank::Dead(void)
{

	CObj* pobj = new CGeneraEff(L"LARGEBANG" , m_vPos , D3DXVECTOR2(0.85f,0.85f) , SORT_GROUND );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}

