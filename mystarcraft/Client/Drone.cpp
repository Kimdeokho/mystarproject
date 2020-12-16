#include "stdafx.h"
#include "Drone.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"
#include "Area_Mgr.h"
#include "KeyMgr.h"
#include "UnitMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "Session_Mgr.h"

#include "Com_fog.h"
#include "Com_Pathfind.h"
#include "Com_Meleesearch.h"
#include "Com_DroneAnim.h"
#include "Com_Worksearch.h"
#include "Com_WDrone.h"
#include "Com_CC.h"
#include "Com_Collision.h"

#include "UI_Select.h"

#include "UI_Energy_bar.h"
#include "Building_Preview.h"
#include "GeneraEff.h"

#include "SoundDevice.h"
#include "Session_Mgr.h"

#include "Input_Interface.h"

#include "MyCmd_Building.h"
#include "UI_Cmd_info.h"
#include "Corpse.h"

#include "Hatchery.h"
#include "Z_gas.h"
#include "FColony.h"
#include "Spwaning.h"
#include "Chamber.h"
#include "Hydraden.h"
#include "Spire.h"
#include "Queennest.h"
#include "Ultra_cave.h"
#include "Defiler_mound.h"

#include "UI_Wireframe.h"
#include "UI_Resource.h"
#include "FontMgr.h"

CDrone::CDrone(void)
{
}

CDrone::~CDrone(void)
{
	Release();
}

void CDrone::Initialize(void)
{
	CObj::unit_area_Initialize();
	CWorkman::Initialize();

	m_vcurdir = D3DXVECTOR2(1, 1);
	
	m_eOBJ_NAME = OBJ_DRONE;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_UNIT;

	m_unitinfo.etribe = TRIBE_ZERG;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.maxhp = 40;
	m_unitinfo.hp = 40;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 84;
	m_unitinfo.attack_range = 1*32;
	m_unitinfo.air_attack_range = 1*32;
	m_unitinfo.search_range = 6*32;
	m_unitinfo.fog_range = 512;


	float fvtx = 11.5f;

	m_vertex.left = fvtx;
	m_vertex.right = fvtx;
	m_vertex.top =  fvtx;
	m_vertex.bottom = fvtx;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;

	//m_panimation = new CCom_DroneAnim(m_matWorld , m_curtex);

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,16);
	m_com_anim = new CCom_DroneAnim(m_matWorld);
	m_com_targetsearch = new CCom_Meleesearch(SEARCH_ONLY_ENEMY);
	m_com_worksearch = new CCom_Worksearch();
	m_com_weapon = new CCom_WDrone();
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH , m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex)) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , m_com_weapon )) ;

	//m_fspeed = float(rand()%170 + 30);

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_com_worksearch->Initialize(this);


	m_select_ui = new CUI_Select(L"Select32" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 32);
	m_energybar_ui->Initialize();

}

void CDrone::Update(void)
{
	CObj::area_update();


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(IDLE == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"ATTACK");
	}
	else if(MOVE == m_unitinfo.state || COLLISION == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"MOVE");
	}
	else if(GATHER == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"GATHER");
	}

	if(ORDER_BUILDING == m_unitinfo.order)
	{
		SetDestroy(true);
	}

	if(true == m_is_preview)
	{
		m_main_preview->SetPos(CMouseMgr::GetInstance()->GetAddScrollvMousePt());
		CIngame_UIMgr::GetInstance()->SetPreview(m_main_preview);
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CDrone::Render(void)
{

	if(false == m_unitinfo.is_active)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();
	m_com_cc->Render();
	m_energybar_ui->Render();

	m_com_pathfind->Render();
	CLineMgr::GetInstance()->collisionbox_render(m_rect);

}

void CDrone::Create_Building(void)
{
	CObj* pobj = NULL;

	float fdistance = CMyMath::pos_distance(m_vPos , m_preview_info.vcenter_pos);
	float onestep = GETTIME*m_unitinfo.fspeed;

	if( fdistance < onestep*onestep*2)
	//if( fdistance < 17*17)
	{
		if(false == m_main_preview->Install_check(m_preview_info))
		{
			if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
				CSoundDevice::GetInstance()->PlayVoiceSound(SND_V_DRONE_ERR , OBJ_DRONE);

			m_unitinfo.order = ORDER_NONE;
			m_unitinfo.state = IDLE;
			m_ecmd_state = CMD_BASIC;
			return;
		}

		m_unitinfo.order = ORDER_BUILDING;
		m_unitinfo.state = BUILD;
		if(Z_HATCHERY == m_preview_info.ebuild)
		{
			pobj = new CHatchery(1.5f);
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_HATCERY);			
		}
		else if(Z_FCOLONY == m_preview_info.ebuild)
		{
			pobj = new CFColony;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_FCOLONY);
		}
		else if(Z_SPWANING_POOL == m_preview_info.ebuild)
		{
			pobj = new CSpwaning;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_SPAWNING);
		}
		else if(Z_CHAMBER == m_preview_info.ebuild)
		{
			pobj = new CChamber;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_CHAMBER);
		}
		else if(Z_HYDRADEN == m_preview_info.ebuild)
		{
			pobj = new CHydraden;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_HYDRADEN);
		}
		else if(Z_SPIRE == m_preview_info.ebuild)
		{
			pobj = new CSpire;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_SPIRE);
		}
		else if(Z_QUEEN_NEST == m_preview_info.ebuild)
		{
			pobj = new CQueennest;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_QUEEN_NEST);
		}
		else if(Z_ULTRA_CAVE == m_preview_info.ebuild)
		{
			pobj = new CUltra_cave;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_ULTRA_CAVE);
		}
		else if(Z_DEFILER_MOUND == m_preview_info.ebuild)
		{
			pobj = new CDefiler_mound;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_DEFILER_MOUND);
		}


		if(NULL != pobj)
		{
			pobj->SetTeamNumber(m_eteamnumber);
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
		}

		//m_charge_building = pobj;

		((CCom_Pathfind*)m_com_pathfind)->ClearPath();
		((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
	}
	else
	{
		//if(false == m_main_preview->Install_check(m_preview_info))
		//{
		//	if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
		//		CSoundDevice::GetInstance()->PlayVoiceSound(SND_V_DRONE_ERR , OBJ_DRONE);

		//	m_unitinfo.order = ORDER_NONE;
		//	m_unitinfo.state = IDLE;
		//	m_ecmd_state = CMD_BASIC;
		//}
	}
}

void CDrone::Create_Building(CObj* pgas_resorce)
{
	CObj* pobj = NULL;

	if(CMyMath::Pos_to_index(m_vPos , 32) == 
		CMyMath::Pos_to_index(m_preview_info.vcenter_pos , 32))
	{
		if(Z_GAS == m_preview_info.ebuild)
		{
			if(false == m_main_preview->Install_check(m_preview_info))
			{
				m_unitinfo.order = ORDER_NONE;
				m_unitinfo.state = IDLE;
				return;
			}

			m_unitinfo.order = ORDER_BUILDING;
			m_unitinfo.state = BUILD;

			pobj = new CZ_gas(pgas_resorce);//가스건물이 가스자원을 가져가고
			pobj->SetTeamNumber(m_eteamnumber);
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_Z_GAS);		

			//m_charge_building = pobj;
		}

		((CCom_Pathfind*)m_com_pathfind)->ClearPath();
		((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
	}

}

void CDrone::Inputkey_reaction(const int& nkey)
{
	if(false == m_unitinfo.is_active)
		return;

	if(9999 == nkey)
	{
		//m_ecmd_state = CMD_BASIC;		

		//if(true == m_main_preview->Install_check(m_preview_info))
		{				
			CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
			COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
			iter->second = m_com_worksearch;
			((CCom_Targetsearch*)m_com_worksearch)->SetTarget(ptarget);

			m_unitinfo.order = ORDER_MOVE_BUILD;

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(m_preview_info.vcenter_pos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();

			m_main_preview->SetActive(false);			

			//위치에 도착하면 건물생성
		}
		//else
		//{
		//	m_unitinfo.order = ORDER_NONE;
		//	m_main_preview->SetActive(false);
		//}
		m_is_preview = false;
	}

	m_is_preview = false;
	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		CWorkman::SetMineral_mark(NULL);

		if(NULL != ptarget)
		{
			OBJID eobjid = ptarget->GetOBJNAME();

			if(OBJ_MINERAL == eobjid ||	OBJ_GAS == eobjid || OBJ_Z_GAS == eobjid)
			{
				m_unitinfo.order = ORDER_GATHER;
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
		}

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}

		//무기가 발사준비 완료일때 무브시킨다
	}
	if('Q' == nkey)
	{
		//m_fspeed = 30;
		m_eteamnumber = TEAM_0;
	}

	if('A' == nkey)
	{		
	}
	if('Z' == nkey)
	{
	}
}
void CDrone::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	m_is_preview = false;

	if(false == m_unitinfo.is_active)
		return;	

	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		CWorkman::SetMineral_mark(NULL);

		m_unitinfo.order = ORDER_MOVE_ATTACK;
		m_unitinfo.state = MOVE;

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
	}
}
bool CDrone::Input_cmd(const int& nkey , bool* waitkey)
{
	if(false == m_unitinfo.is_active)
		return false;

	if(VK_RBUTTON == nkey)
	{
		if(true == m_is_preview)
		{
			m_main_preview->SetActive(false);
			m_is_preview = false;
		}

		if(ORDER_MOVE_BUILD == m_unitinfo.order)
			m_ecmd_state = CMD_BASIC;

		if(CMD_BUILDING == m_ecmd_state)
			m_ecmd_state = CMD_BASIC;
	}

	else if(VK_LBUTTON == nkey)
	{
		//여기서 명령을 입력하고
		if(true == m_is_preview)
		{			
			if(true == m_main_preview->Install_check())
			{
				if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
					CSoundDevice::GetInstance()->PlayEffSound(SND_EFF_ZBUILD , 0);

				D3DXVECTOR2 vmousept = CMouseMgr::GetInstance()->GetClick_Pos();
				CArea_Mgr::GetInstance()->TargetChoice(vmousept);

				m_main_preview->SetActive(false);
				m_is_preview = false;
				m_preview_info = m_main_preview->GetPreviewInfo();
				CKeyMgr::GetInstance()->GetInputDevice()->PushCommand(CMyCmd_Building::StaticCreate(m_preview_info));
			}
			else
			{
				if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
					CSoundDevice::GetInstance()->PlayVoiceSound(SND_V_DRONE_ERR , OBJ_DRONE);
			}
		}		
	}

	else if('B' == nkey && CMD_BASIC == m_ecmd_state)
	{
		CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		if( true == pui->active_cmdbtn(6 , BTN_B_ZBUILD) )
		{
			m_ecmd_state = CMD_B_VIEW;
			//waitkey[nkey] = true;
		}
	}

	else if('V' == nkey && CMD_BASIC == m_ecmd_state)
	{
		CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		if( true == pui->active_cmdbtn(7 , BTN_V_ZBUILD) )
		{
			m_ecmd_state = CMD_V_VIEW;
			//waitkey[nkey] = true;
		}
	}

	else if(VK_ESCAPE == nkey)
		m_ecmd_state = CMD_BASIC;

	else if(CMD_B_VIEW == m_ecmd_state || CMD_V_VIEW == m_ecmd_state)
	{
		if( 1 == CUnitMgr::GetInstance()->GetSelectunit_size())
		{		
			if(CMD_B_VIEW == m_ecmd_state)
			{
				m_is_preview = true;
				if('H' == nkey)
				{
					// L , R , T ,B
					MYRECT<float> vtx(49.f , 50.f , 32.f , 33.f);
					SetPreview_info(L"Z_HATCHERY" , Z_HATCHERY , 3 , 4 , vtx);
				}
				else if('C' == nkey)
				{
					MYRECT<float> vtx(24.f , 24.f, 24.f, 24.f);
					SetPreview_info(L"Z_FCOLONY" , Z_FCOLONY , 2 , 2 , vtx);
				}
				else if('E' == nkey)
				{
					MYRECT<float> vtx(64.f , 64.f , 32.f , 32.f);
					SetPreview_info(L"Z_GAS" , Z_GAS , 2 , 4 , vtx);
				}
				else if('S' == nkey)
				{
					MYRECT<float> vtx(36.f , 41.f , 28.f , 19.f);
					SetPreview_info(L"Z_SPWANING_POOL" , Z_SPWANING_POOL , 2 , 3 , vtx);
				}
				else if('V' == nkey)
				{
					MYRECT<float> vtx(44 , 48-15 , 32 , 32-11);
					SetPreview_info(L"Z_CHAMBER" , Z_CHAMBER , 2 , 3 , vtx);
				}
				else if('D' == nkey)
				{
					MYRECT<float> vtx(48-8 , 48-7 , 32 , 32-7);
					SetPreview_info(L"Z_HYDRADEN" , Z_HYDRADEN , 2 , 3 , vtx);
				}
				else
					m_is_preview = false;
			}
			else if(CMD_V_VIEW == m_ecmd_state)
			{
				m_is_preview = true;
				if('S' == nkey)
				{
					MYRECT<float> vtx(32-4 , 32-3 , 32 , 32-7);
					SetPreview_info(L"Z_SPIRE" , Z_SPIRE , 2 , 2 , vtx);
				}
				else if('Q' == nkey)
				{
					MYRECT<float> vtx(48-10 , 48-15 , 32-8 , 32-3);
					SetPreview_info(L"Z_QUEEN_NEST" , Z_QUEEN_NEST , 2 , 3 , vtx);
				}
				else if('U' == nkey)
				{
					MYRECT<float> vtx(48-8, 48-15, 32 , 32);
					SetPreview_info(L"Z_ULTRA_CAVE" , Z_ULTRA_CAVE , 2 , 3 , vtx);
				}
				else if('F' == nkey)
				{
					MYRECT<float> vtx(64-16 , 64-15 , 32 , 32-27);
					SetPreview_info(L"Z_DEFILER_MOUND" , Z_DEFILER_MOUND , 2 , 4 , vtx);
				}
				else
					m_is_preview = false;
			}
		}
	}

	else if('A' == nkey)
		waitkey[nkey] = true;

	return false;
}

bool CDrone::Input_cmd(const int& firstkey , const int& secondkey)
{
	//if( 1 == CUnitMgr::GetInstance()->GetSelectunit_size())
	//{
	//	m_is_preview = true;
	//	if('B' == firstkey && 'H' == secondkey)
	//	{
	//		// L , R , T ,B
	//		MYRECT<float> vtx(49.f , 50.f , 32.f , 33.f);
	//		SetPreview_info(L"Z_HATCHERY" , Z_HATCHERY , 3 , 4 , vtx);
	//	}
	//	else if('B' == firstkey && 'C' == secondkey)
	//	{
	//		MYRECT<float> vtx(24.f , 24.f, 24.f, 24.f);
	//		SetPreview_info(L"Z_FCOLONY" , Z_FCOLONY , 2 , 2 , vtx);
	//	}
	//	else if('B' == firstkey && 'E' == secondkey)
	//	{
	//		MYRECT<float> vtx(64.f , 64.f , 32.f , 32.f);
	//		SetPreview_info(L"Z_GAS" , Z_GAS , 2 , 4 , vtx);
	//	}
	//	else if('B' == firstkey && 'S' == secondkey)
	//	{
	//		MYRECT<float> vtx(36.f , 41.f , 28.f , 19.f);
	//		SetPreview_info(L"Z_SPWANING_POOL" , Z_SPWANING_POOL , 2 , 3 , vtx);
	//	}
	//	else if('B' == firstkey && 'V' == secondkey)
	//	{
	//		MYRECT<float> vtx(44 , 48-15 , 32 , 32-11);
	//		SetPreview_info(L"Z_CHAMBER" , Z_CHAMBER , 2 , 3 , vtx);
	//	}
	//	else if('B' == firstkey && 'D' == secondkey)
	//	{
	//		MYRECT<float> vtx(48-8 , 48-7 , 32 , 32-7);
	//		SetPreview_info(L"Z_HYDRADEN" , Z_HYDRADEN , 2 , 3 , vtx);
	//	}
	//	else if('V' == firstkey && 'S' == secondkey)
	//	{
	//		MYRECT<float> vtx(32-4 , 32-3 , 32 , 32-7);
	//		SetPreview_info(L"Z_SPIRE" , Z_SPIRE , 2 , 2 , vtx);
	//	}
	//	else if('V' == firstkey && 'Q' == secondkey)
	//	{
	//		MYRECT<float> vtx(48-10 , 48-15 , 32-8 , 32-3);
	//		SetPreview_info(L"Z_QUEEN_NEST" , Z_QUEEN_NEST , 2 , 3 , vtx);
	//	}
	//	else if('V' == firstkey && 'U' == secondkey)
	//	{
	//		MYRECT<float> vtx(48-8, 48-15, 32 , 32);
	//		SetPreview_info(L"Z_ULTRA_CAVE" , Z_ULTRA_CAVE , 2 , 3 , vtx);
	//	}
	//	else if('V' == firstkey && 'F' == secondkey)
	//	{
	//		MYRECT<float> vtx(64-16 , 64-15 , 32 , 32-27);
	//		SetPreview_info(L"Z_DEFILER_MOUND" , Z_DEFILER_MOUND , 2 , 4 , vtx);
	//	}
	//	else
	//		m_is_preview = false;
	//}

	return false;
}
void CDrone::Update_Cmdbtn(void)
{	
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	if(CMD_BUILDING == m_ecmd_state)
	{
		for(int i = 0; i < 9; ++i)
			pui->Create_Cmdbtn(0 , L"" , BTN_NONE);
	}
	else if(CMD_B_VIEW == m_ecmd_state)
	{
		pui->Z_Cmdbtn_B_buildsetting(m_eteamnumber);
		pui->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
	}
	else if(CMD_V_VIEW == m_ecmd_state)
	{
		pui->Z_Cmdbtn_V_buildsetting(m_eteamnumber);
		pui->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
	}
	else
	{
		pui->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
		pui->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
		pui->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
		pui->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
		pui->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

		pui->Create_Cmdbtn(6 , L"BTN_ZBBUILD" , BTN_B_ZBUILD);
		pui->Create_Cmdbtn(7 , L"BTN_ZVBUILD" , BTN_V_ZBUILD);
	}
}

void CDrone::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		//m_ecmd_state = CMD_BASIC;
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_DRONE" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Drone" ,interface_pos.x + 320 , interface_pos.y + 390 );
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
		interface_pos.x + 195 , interface_pos.y + 460 , font_color);

	WEAPON_INFO temp_info = ((CCom_Weapon*)m_com_weapon)->GetWeapon_info();
	CFontMgr::GetInstance()->Setbatch_Font(L"공격력: %d + %d" ,temp_info.damage , 0,
		interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));
	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_Z_GROUND_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}

void CDrone::Dead(void)
{
	CSoundDevice::GetInstance()->PlayBattleSound(SND_B_DRDTH , m_vPos);

	CObj* pobj = new CCorpse(L"DRONEDEAD" , L"DRONEWRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

}
void CDrone::Release(void)
{
	CObj::area_release();

	if(NULL != m_com_targetsearch)
		Safe_Delete(m_com_targetsearch);
	if(NULL != m_com_worksearch)
		Safe_Delete(m_com_worksearch);

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
	m_componentlist.erase(iter);

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(-1 , m_eteamnumber);
}

