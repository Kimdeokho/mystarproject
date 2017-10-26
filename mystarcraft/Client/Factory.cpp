#include "StdAfx.h"
#include "Factory.h"

#include "Component.h"
#include "Com_TBuildingAnim.h"
#include "Com_Collision.h"
#include "Com_fog.h"
#include "Com_AirPathfind.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"

#include "UI_Select.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "Corpse.h"
#include "TimeMgr.h"
#include "ComanderMgr.h"

#include "Building_Preview.h"
#include "MouseMgr.h"

#include "Factory_addon.h"
CFactory::CFactory(void)
{
}

CFactory::~CFactory(void)
{
	Release();
}

void CFactory::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 56.f;
	m_vertex.right = 57.f;
	m_vertex.top =  40.f;
	m_vertex.bottom = 41.f;

	CTerran_building::building_area_Initialize(3 , 4);
	CTerran_building::building_pos_Initialize(3 , 4);

	m_sortID = SORT_GROUND;	
	m_ecategory = BUILDING;
	m_eOBJ_NAME = OBJ_FACTORY;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = BUILD;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_NOMAL;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.damage = 0;
	m_unitinfo.hp = 1;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 28;
	m_unitinfo.attack_range = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 1.f;

	m_com_anim = new CCom_TBuildingAnim(L"T_FACTORY",m_matWorld , m_curtex );
	m_com_pathfind = new CCom_AirPathfind(m_vPos);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select146" , m_vPos , 10);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);

	m_is_take_off = false;
}

void CFactory::Update(void)
{
	CTerran_building::building_area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();


	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"BUILD");
	}
	else if(TAKE_OFF == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"AIR");

		m_vPos.y -= GETTIME*20;

		if(m_vairpos.y - 48.f >= m_vPos.y)
		{
			//이륙 완료
			m_vPos.y = m_vairpos.y - 48.f;
			m_unitinfo.estate = AIR_IDLE;

			if(true == m_is_autoinstall)
			{
				m_is_autoinstall = false;
				m_is_partinstall = true;

				Landing_move( ((CBuilding_Preview*)m_sub_preview)->GetPreviewInfo().vpos );
			}
		}
	}
	else if(LANDING == m_unitinfo.estate)
	{
		m_vPos.y += GETTIME*20;

		if(m_vgroundpos.y <= m_vPos.y)
		{
			//설치해야 한다면..
			if(true == m_is_partinstall)
			{
				CObj* pobj = NULL;
				m_is_partinstall = false;
				if(T_FAC_ADDON == ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().ebuild)
				{
					pobj = new CFactory_addon(this/*아니면 오브젝트 아이디*/);
					pobj->SetPos(((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().vpos);
					pobj->Initialize();				
					CObjMgr::GetInstance()->AddObject(pobj , OBJ_FAC_ADDON);
				}
				m_partbuilding = pobj;
				((CTerran_building*)m_partbuilding)->Setlink(true);
			}
			else
			{
				int partidx = m_curidx32 + 3 + SQ_TILECNTX;
				m_partbuilding = CArea_Mgr::GetInstance()->Search_Partbuilding(m_curidx64 , partidx , OBJ_FAC_ADDON);
				if(NULL != m_partbuilding)
					((CTerran_building*)m_partbuilding)->Setlink(true);
			}

			m_is_take_off = false;
			m_vPos.y = m_vgroundpos.y;
			m_unitinfo.eMoveType = MOVE_GROUND;
			m_sortID = SORT_GROUND;
			m_unitinfo.eorder = ORDER_NONE;
			m_unitinfo.estate = IDLE;

			CTerran_building::building_area_Initialize(3, 4);
			COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
			m_componentlist.erase(iter);
		}

	}


	if(ORDER_LANDING_MOVE == m_unitinfo.eorder)
	{
		if(true == ((CCom_AirPathfind*)m_com_pathfind)->Getarrive() ) 
		{
			if(true == m_is_partinstall)
			{
				if(true == ((CBuilding_Preview*)m_main_preview)->Install_check() &&
					true == ((CBuilding_Preview*)m_sub_preview)->Install_check())
				{
					m_unitinfo.estate = LANDING;
					m_unitinfo.eorder = ORDER_NONE;
				}
				else
				{
					m_unitinfo.estate = AIR_IDLE;
					m_unitinfo.eorder = ORDER_NONE;
				}
			}
			else
			{
				if(true == ((CBuilding_Preview*)m_main_preview)->Install_check())
				{
					m_unitinfo.estate = LANDING;
					m_unitinfo.eorder = ORDER_NONE;
				}
				else
				{
					m_unitinfo.estate = AIR_IDLE;
					m_unitinfo.eorder = ORDER_NONE;
				}
			}
		}
	}



	if(true == m_is_preview)
	{
		D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		((CBuilding_Preview*)m_main_preview)->SetPos(vpos);
		vpos.x -= m_irow*32;
		vpos.y -= m_weight.y;
		((CBuilding_Preview*)m_sub_preview)->SetPos(vpos);
		CComanderMgr::GetInstance()->SetPreview(m_main_preview);
		CComanderMgr::GetInstance()->SetPreview(m_sub_preview);
	}

	//vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	//vpos.x -= CScrollMgr::m_fScrollX;
	//vpos.y -= CScrollMgr::m_fScrollY;
	//CFontMgr::GetInstance()->Setbatch_Font(L"@" , m_vPos.x - CScrollMgr::m_fScrollX, 
	//	m_vPos.y - CScrollMgr::m_fScrollY);
}

void CFactory::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CFactory::Release(void)
{
	CTerran_building::area_release();
	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);


	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
	Safe_Delete(m_com_pathfind);
	if(iter != m_componentlist.end())
	{
		m_componentlist.erase(iter);
	}
}

void CFactory::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);


	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	if(NULL != m_partbuilding)
	{
		((CTerran_building*)m_partbuilding)->Setlink(false);
		m_partbuilding = NULL;
	}
}

void CFactory::Inputkey_reaction(const int& nkey)
{
	m_is_preview = false;

	if(TAKE_OFF == m_unitinfo.estate ||
		LANDING == m_unitinfo.estate)
		return;

	if('C' == nkey)
	{
		m_is_preview = true;
		((CBuilding_Preview*)m_main_preview)->SetPreviewInfo(L"T_FACADDON", T_FAC_ADDON , 2 , 2 , this);
		((CBuilding_Preview*)m_sub_preview)->SetPreviewInfo(L"T_FACTORY", T_FACTORY , 3 , 4 , this);
	}
	if('L' == nkey)
	{
		if(false == m_is_take_off)
		{
			//이륙
			CTerran_building::TakeOff();
		}
		else
		{
			//착륙 프리뷰를 킨다
			m_is_preview = true;
			((CBuilding_Preview*)m_main_preview)->SetPreviewInfo(L"T_FACTORY", T_FACTORY , 3 , 4 , this);			
		}		
	}
	if(VK_LBUTTON == nkey)
	{
		if(true == ((CBuilding_Preview*)m_main_preview)->GetActive() &&
			true == ((CBuilding_Preview*)m_sub_preview)->GetActive())
		{
			if(true == ((CBuilding_Preview*)m_main_preview)->Install_check() &&
				true == ((CBuilding_Preview*)m_sub_preview)->Install_check())
			{

				((CBuilding_Preview*)m_main_preview)->SetActive(false);
				((CBuilding_Preview*)m_sub_preview)->SetActive(false);

				if(true == m_is_take_off)
				{
					//공중에서 부속품 설치
					m_is_partinstall = true;
					CTerran_building::Landing_move( ((CBuilding_Preview*)m_sub_preview)->GetPreviewInfo().vpos );

				}
				else
				{
					//원래 위치와 다를때... 공중에 뜨고, 이동후, 착지한다음 설치
					D3DXVECTOR2 vpt = CMouseMgr::GetInstance()->GetAddScrollvMousePt();//부속건물 클릭위치
					vpt.x = vpt.x - (m_irow/2)*SQ_TILESIZEX;//부속건물 클릭위치 조정
					int idx = CMyMath::Pos_to_index(vpt , 32);

					if(idx != m_curidx32)
					{
						m_is_autoinstall = true;
						m_is_partinstall = true;

						CTerran_building::TakeOff();

					}
					else
					{
						CObj* pobj = NULL;
						if(T_FAC_ADDON == ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().ebuild)
						{
							pobj = new CFactory_addon(this);
							pobj->SetPos(((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().vpos);
							pobj->Initialize();				
							CObjMgr::GetInstance()->AddObject(pobj , OBJ_FAC_ADDON);
						}
						m_partbuilding = pobj;
						((CTerran_building*)m_partbuilding)->Setlink(true);
					}
				}
			}
			else
			{
				m_is_preview = true; //설치에 실패하면 프리뷰를 계속 본다.
			}
		}
		else if(true == ((CBuilding_Preview*)m_main_preview)->GetActive())
		{
			//착륙 분기
			if(true == ((CBuilding_Preview*)m_main_preview)->Install_check())
			{
				((CBuilding_Preview*)m_main_preview)->SetActive(false);
				((CBuilding_Preview*)m_sub_preview)->SetActive(false);

				m_unitinfo.eorder = ORDER_LANDING_MOVE;
				D3DXVECTOR2 vclickpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				int idx32 = CMyMath::Pos_to_index(vclickpos , 32);
				vclickpos = CMyMath::index_to_Pos(idx32 , 128 , 32);
				CTerran_building::Landing_move(vclickpos);	

			}
			else
			{
				m_is_preview = true; //설치에 실패하면 프리뷰를 계속 본다.
			}
		}
	}
	if(VK_RBUTTON == nkey)
	{

		((CBuilding_Preview*)m_main_preview)->SetActive(false);
		((CBuilding_Preview*)m_sub_preview)->SetActive(false);

		if(AIR_IDLE == m_unitinfo.estate)
		{
			m_unitinfo.eorder = ORDER_MOVE;
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(goalpos);
		}
	}
}

void CFactory::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
