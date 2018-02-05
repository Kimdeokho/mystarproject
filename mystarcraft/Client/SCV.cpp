#include "StdAfx.h"
#include "SCV.h"

#include "Com_Pathfind.h"
#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_SCVAnim.h"
#include "Com_Meleesearch.h"
#include "Com_Worksearch.h"
#include "Com_WSCV.h"

#include "UI_Select.h"

#include "ObjMgr.h"
#include "UnitMgr.h"
#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"

#include "MyMath.h"
#include "ComanderMgr.h"

#include "GeneraEff.h"

#include "Comandcenter.h"
#include "Factory.h"
#include "Starport.h"
#include "T_gas.h"
#include "Sience.h"
#include "Barrack.h"
#include "Engineering.h"
#include "Supplydepot.h"
#include "Academy.h"
#include "Armoury.h"
#include "Bunker.h"
#include "Turret.h"

#include "Building_Preview.h"
#include "TileManager.h"
#include "MouseMgr.h"
#include "TimeMgr.h"

#include "UI_Wireframe.h"
CSCV::CSCV(void)
{
}
CSCV::CSCV(const D3DXVECTOR2& vpos)
{
	m_vPos = vpos;
}
CSCV::~CSCV(void)
{
	Release();
}

void CSCV::Initialize(void)
{
	CObj::unit_area_Initialize();
	CWorkman::Initialize();

	m_eteamnumber = TEAM_0;
	m_ecategory = UNIT;
	m_eOBJ_NAME = OBJ_SCV;
	m_sortID = SORT_GROUND;

	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.maxhp = 60;
	m_unitinfo.hp = 60;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 84;
	m_unitinfo.search_range = 4*32;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 11.5;
	m_vertex.right = 11.5;
	m_vertex.top =  11.5;
	m_vertex.bottom = 11.5;



	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 , 16);
	m_com_anim = new CCom_SCVAnim(m_matWorld);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex);
	m_com_targetsearch = new CCom_Meleesearch(SEARCH_ONLY_ENEMY);
	m_com_worksearch = new CCom_Worksearch();
	m_com_weapon = new CCom_WSCV();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION ,  m_com_collision) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,m_com_targetsearch) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , m_com_weapon )) ;	


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_com_worksearch->Initialize(this);

	m_select_ui = new CUI_Select(L"Select32" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui , MOVE_GROUND);

	m_fterm_time = 0.f;
	m_vnormal = D3DXVECTOR2(0 ,0 );
	m_moveon = false;
	m_movetime = 0.f;
	m_build_idx = 0;
}

void CSCV::Update(void)
{
	m_select_ui->Update();
	if(BOARDING == m_unitinfo.estate)
	{
		((CUI_Select*)m_select_ui)->SetSelect(NONE_SELECT);
		return;
	}
	CObj::area_update();
	CWorkman::Fragment_Pos_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(!m_bSelect)
		m_ecmd_state = CMD_BASIC;

	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"ATTACK");
	}
	else if(MOVE == m_unitinfo.estate || COLLISION == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
	}


	if(ORDER_BUILDING == m_unitinfo.eorder)
	{
		//건물 짓는중이다.
		m_sortID = SORT_GROUND_EFF;

		if(NULL != m_charge_building)
		{			
			m_ecmd_state = CMD_BUILDING;

			MYRECT<float> rc = m_charge_building->GetMyRect();

			m_vbuild_pos[0].x = rc.left;
			m_vbuild_pos[0].y = rc.top;

			m_vbuild_pos[1].x = rc.right;
			m_vbuild_pos[1].y = rc.top;

			m_vbuild_pos[2].x = rc.right;
			m_vbuild_pos[2].y = rc.bottom;

			m_vbuild_pos[3].x = rc.left;
			m_vbuild_pos[3].y = rc.bottom;

			m_fterm_time += GETTIME;
			if(m_fterm_time > 1.2f)
			{
				int randvalue = rand()%4;
				if(m_build_idx == randvalue)
				{
					++m_build_idx;
					m_build_idx %= 4;
				}
				else
					m_build_idx = randvalue;

				m_moveon = true;
				m_fterm_time = 0.f;

			}
			else
			{
				m_unitinfo.estate = ATTACK;
				D3DXVec2Normalize(&m_vcurdir , &(m_charge_building->GetPos() - m_vPos));
			}

			if(m_moveon)
			{
				m_fterm_time = 0.f;
				D3DXVec2Normalize( &m_vcurdir , &(m_vbuild_pos[m_build_idx] - m_vPos) );

				m_movetime += GETTIME;
				if( m_movetime > 0.8f) 
				{
					m_movetime = 0.f;
					m_moveon = false;
				}
				else
				{
					if( int(m_vbuild_pos[m_build_idx].x) != int(m_vPos.x) &&
						int(m_vbuild_pos[m_build_idx].y) != int(m_vPos.y))
					{
						m_vPos += m_vcurdir * GETTIME * m_unitinfo.fspeed;
						m_unitinfo.estate = MOVE;
					}
				}
			}

			if(IDLE == m_charge_building->GetUnitinfo().estate)
			{
				//건물 완성
				m_sortID = SORT_GROUND;
				m_unitinfo.estate = IDLE;
				m_unitinfo.eorder = ORDER_NONE;
				m_charge_building = NULL;

				m_ecmd_state = CMD_BASIC;
			}
		}
	}

	if(true == m_is_preview)
	{
		((CBuilding_Preview*)m_main_preview)->SetPos(CMouseMgr::GetInstance()->GetAddScrollvMousePt());
		CComanderMgr::GetInstance()->SetPreview(m_main_preview);
	}

}

void CSCV::Render(void)
{
	if(BOARDING == m_unitinfo.estate)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_com_anim->Render();

	m_com_pathfind->Render();
	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CSCV::Inputkey_reaction(const int& nkey)
{
	m_is_preview = false;

	if(BOARDING == m_unitinfo.estate)
		return;

	
	if(VK_LBUTTON == nkey)
	{
		if(true == ((CBuilding_Preview*)m_main_preview)->GetActive())
		{
			m_ecmd_state = CMD_BASIC;
			D3DXVECTOR2  vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
			CArea_Mgr::GetInstance()->TargetChoice(vmousept);
			CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

			m_is_preview = true;
			if(true == ((CBuilding_Preview*)m_main_preview)->Install_check())
			{				
				COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
				iter->second = m_com_worksearch;
				((CCom_Targetsearch*)m_com_worksearch)->SetTarget(ptarget);

				m_unitinfo.eorder = ORDER_MOVE_BUILD;
				m_preview_info = ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo();

				CTileManager::GetInstance()->Flowfield_Pathfinding(m_preview_info.vcenter_pos);
				CUnitMgr::GetInstance()->Calculate_UnitCenterPt(m_preview_info.vcenter_pos);
				((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(m_preview_info.vcenter_pos , m_bmagicbox);
				((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
				((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();

				((CBuilding_Preview*)m_main_preview)->SetActive(false);

				//위치에 도착하면 건물생성
			}			
		}
	}
	if(VK_RBUTTON == nkey)
	{
		((CBuilding_Preview*)m_main_preview)->SetActive(false);

		m_unitinfo.estate = MOVE;
		m_unitinfo.eorder = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		CWorkman::SetMineral_mark(NULL);

		if(NULL != ptarget)
		{
			OBJID eobjid = ptarget->GetOBJNAME();

			if(OBJ_MINERAL == eobjid ||	OBJ_GAS == eobjid || OBJ_GASBUILDING == eobjid)
			{
				m_unitinfo.eorder = ORDER_GATHER;
				COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
				iter->second = m_com_worksearch;
				((CCom_Targetsearch*)m_com_worksearch)->SetTarget(ptarget);
			}
			else
			{
				//같은팀일때 아닐때 나눈다..
				COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
				iter->second = m_com_worksearch;
				((CCom_Targetsearch*)m_com_worksearch)->SetTarget(ptarget);
			}

		}
		else
		{
			COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
			iter->second = m_com_targetsearch;
			//((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);
		}

		if(NULL != m_com_pathfind)
		{
			if(NULL == ptarget)
				m_bmagicbox = true;
			else
				m_bmagicbox = false;

			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
	}

	if('B' == nkey)
	{
		if( true == CComanderMgr::GetInstance()->Active_Cmdbtn(6 , BTN_B_BUILD) )
			m_ecmd_state = CMD_B_VIEW;			
	}

	if('V' == nkey)
	{
		if( true == CComanderMgr::GetInstance()->Active_Cmdbtn(7 , BTN_V_BUILD) )
			m_ecmd_state = CMD_V_VIEW;
	}

	if('W' == nkey)
	{
		m_eteamnumber = TEAM_1;
	}
}
void CSCV::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	m_is_preview = false;

	if(BOARDING == m_unitinfo.estate)
		return;	

	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		CWorkman::SetMineral_mark(NULL);

		m_unitinfo.eorder = ORDER_MOVE_ATTACK;
		m_unitinfo.estate = MOVE;

		COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
		iter->second = m_com_targetsearch;
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
		CWorkman::SetMineral_mark(NULL);
	}

	if( 1 == CUnitMgr::GetInstance()->GetSelectunit_size())
	{
		if('B' == firstkey && 'A' == secondkey)
		{
			SetPreview_info(L"T_ACADEMY" , T_ACADEMY , 2 , 3);
		}
		if('B' == firstkey && 'B' == secondkey)
		{
			SetPreview_info(L"T_BARRACK" , T_BARRACK , 3 , 4);
		}
		if('B' == firstkey && 'C' == secondkey)
		{
			SetPreview_info(L"COMMANDCENTER" , T_COMMANDCENTER , 3 , 4);
		}
		if('B' == firstkey && 'E' == secondkey)
		{
			SetPreview_info(L"T_EB" , T_EB , 3 , 4);
		}
		if('B' == firstkey && 'R' == secondkey)
		{
			SetPreview_info(L"T_GAS" , T_GAS , 2 , 4);
		}
		if('B' == firstkey && 'S' == secondkey)
		{
			SetPreview_info(L"T_SUPPLY" , T_SUPPLY , 2 , 3);
		}
		if('B' == firstkey && 'T' == secondkey)
		{
			SetPreview_info(L"T_TURRET_PREVIEW" , T_TURRET , 2 , 2);
		}

		if('B' == firstkey && 'U' == secondkey)
		{
			SetPreview_info(L"T_BUNKER" , T_BUNKER , 2 , 3);
		}

		if('V' == firstkey && 'A' == secondkey)
		{
			SetPreview_info(L"T_ARMOURY" , T_ARMOURY , 2 , 3);
		}
		if('V' == firstkey && 'F' == secondkey)
		{
			SetPreview_info(L"T_FACTORY" , T_FACTORY , 3 , 4);
		}
		if('V' == firstkey && 'I' == secondkey)
		{
			SetPreview_info(L"T_SIENCE" , T_SIENCE , 3 , 4);
		}
		if('V' == firstkey && 'S' == secondkey)
		{
			SetPreview_info(L"T_STARPORT" , T_STARPORT , 3 , 4);
		}
	}

}

void CSCV::Release(void)
{
	CObj::area_release();

	if(NULL != m_com_targetsearch)
		Safe_Delete(m_com_targetsearch);
	if(NULL != m_com_worksearch)
		Safe_Delete(m_com_worksearch);

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
	m_componentlist.erase(iter);
}

void CSCV::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND ,1.4f);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}

void CSCV::Create_Building(void)
{


	CObj* pobj = NULL;
	if(CMyMath::Pos_to_index(m_vPos , 32) == 
		CMyMath::Pos_to_index(m_preview_info.vcenter_pos , 32))
	{
		if(false == ((CBuilding_Preview*)m_main_preview)->Install_check(m_preview_info))
		{
			m_unitinfo.eorder = ORDER_NONE;
			m_unitinfo.estate = IDLE;
			return;
		}


		m_unitinfo.eorder = ORDER_BUILDING;
		m_unitinfo.estate = BUILD;
		if(T_COMMANDCENTER == m_preview_info.ebuild)
		{
			pobj = new CComandcenter;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_COMMAND);			
		}
		else if(T_FACTORY == m_preview_info.ebuild)
		{
			pobj = new CFactory;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_FACTORY);
		}
		else if(T_STARPORT == m_preview_info.ebuild)
		{
			pobj = new CStarport;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_STARPORT);
		}
		else if(T_SIENCE == m_preview_info.ebuild)
		{
			pobj = new CSience;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_SIENCE);
		}
		else if(T_BARRACK == m_preview_info.ebuild)
		{
			pobj = new CBarrack;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_BARRACK);
		}
		else if(T_EB == m_preview_info.ebuild)
		{
			pobj = new CEngineering;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_EB);
		}
		else if(T_SUPPLY == m_preview_info.ebuild)
		{
			pobj = new CSupplydepot;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_SUPPLY);
		}
		else if(T_ACADEMY == m_preview_info.ebuild)
		{
			pobj = new CAcademy;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_ACADEMY);
		}
		else if(T_ARMOURY == m_preview_info.ebuild)
		{
			pobj = new CArmoury;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_ARMOURY);
		}
		else if(T_BUNKER == m_preview_info.ebuild)
		{
			pobj = new CBunker;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_BUNKER);
		}
		else if(T_TURRET == m_preview_info.ebuild)
		{
			pobj = new CTurret;
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_TURRET);
		}

		m_charge_building = pobj;

		((CCom_Pathfind*)m_com_pathfind)->ClearPath();
		((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
	}
}

void CSCV::Create_Building(CObj* pgas_resorce)
{
	CObj* pobj = NULL;

	if(CMyMath::Pos_to_index(m_vPos , 32) == 
		CMyMath::Pos_to_index(m_preview_info.vcenter_pos , 32))
	{
		if(T_GAS == m_preview_info.ebuild)
		{
			m_unitinfo.eorder = ORDER_BUILDING;
			m_unitinfo.estate = BUILD;

			pobj = new CT_gas(pgas_resorce);//가스건물이 가스자원을 가져가고
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_GASBUILDING);		

			m_charge_building = pobj;
		}

		((CCom_Pathfind*)m_com_pathfind)->ClearPath();
		((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
	}
	
}

void CSCV::SetPreview_info(const TCHAR* objkey , TERRAN_BUILD_TECH ebuild , const int& icol , const int& irow )
{
	//CComanderMgr::GetInstance()->Release_Cmdbtn(); 여기 수정해야함
	//자원이 모자르면 못보여줌
	//버튼 비활성화면 못보여줌

	m_is_preview = true;
	((CBuilding_Preview*)m_main_preview)->SetPreviewInfo(objkey ,ebuild, icol , irow , this);
}
void CSCV::Update_Cmdbtn(void)
{
	if(CMD_BUILDING == m_ecmd_state)
	{
		for(int i = 0; i < 9; ++i)
			CComanderMgr::GetInstance()->Create_Cmdbtn(0 , L"" , BTN_NONE);
	}
	else if(CMD_B_VIEW == m_ecmd_state)
	{
		CComanderMgr::GetInstance()->T_Cmdbtn_B_buildsetting();
	}
	else if(CMD_V_VIEW == m_ecmd_state)
	{
		CComanderMgr::GetInstance()->T_Cmdbtn_V_buildsetting();
	}
	else
	{
		CComanderMgr::GetInstance()->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
		CComanderMgr::GetInstance()->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
		CComanderMgr::GetInstance()->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
		CComanderMgr::GetInstance()->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
		CComanderMgr::GetInstance()->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

		CComanderMgr::GetInstance()->Create_Cmdbtn(6 , L"BTN_BBUILD" , BTN_B_BUILD);
		CComanderMgr::GetInstance()->Create_Cmdbtn(7 , L"BTN_VBUILD" , BTN_V_BUILD);
	}
}

void CSCV::Update_Wireframe(void)
{
	if(true == CComanderMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.estate))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_SCV" , D3DXVECTOR2(280,545));
		pui->Initialize();
		CComanderMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran SCV" , 400 , 510 );
	}

	D3DCOLOR font_color;

	int iratio = m_unitinfo.maxhp / m_unitinfo.hp;

	if( iratio <= 1)
		font_color = D3DCOLOR_ARGB(255,0,255,0);
	else if( 1 < iratio && iratio <= 2)
		font_color = D3DCOLOR_ARGB(255,255,255,0);
	else if( 2 < iratio)
		font_color = D3DCOLOR_ARGB(255,255,0,0);


	CFontMgr::GetInstance()->Setbatch_Font(L"%d/%d" , m_unitinfo.hp , m_unitinfo.maxhp,
		280 , 580 , font_color);
	
}
