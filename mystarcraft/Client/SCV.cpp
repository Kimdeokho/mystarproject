#include "StdAfx.h"
#include "SCV.h"

#include "Com_Pathfind.h"
#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_SCVAnim.h"
#include "Com_Meleesearch.h"
#include "Com_Worksearch.h"
#include "Com_WSCV.h"
#include "Com_CC.h"

#include "UI_Select.h"

#include "ObjMgr.h"
#include "UnitMgr.h"
#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "KeyMgr.h"
#include "MyCmd_Building.h"
#include "Input_Interface.h"

#include "SoundDevice.h"
#include "Session_Mgr.h"

#include "MyMath.h"
#include "Ingame_UIMgr.h"

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
#include "UI_Cmd_info.h"
#include "UI_Energy_bar.h"
#include "UI_Resource.h"
#include "Skill_Defensive.h"

CSCV::CSCV(const D3DXVECTOR2& vpos)
{
	m_vPos = vpos;
}

CSCV::CSCV(void)
{

}

CSCV::~CSCV(void)
{
	Release();
}

void CSCV::Initialize(void)
{
	CObj::unit_area_Initialize();
	CWorkman::Initialize();

	m_ecategory = CATEGORY_UNIT;
	m_eOBJ_NAME = OBJ_SCV;
	m_sortID = SORT_GROUND;

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.maxhp = 60;
	m_unitinfo.hp = 60;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 84;
	m_unitinfo.attack_range = 1*32;
	m_unitinfo.air_attack_range = 1*32;
	m_unitinfo.search_range = 6*32;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 11.5f;
	m_vertex.right = 11.5f;
	m_vertex.top =  11.5f;
	m_vertex.bottom = 11.5f;



	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 , 16);
	m_com_anim = new CCom_SCVAnim(m_matWorld);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex);
	m_com_targetsearch = new CCom_Meleesearch(SEARCH_ONLY_ENEMY);
	m_com_worksearch = new CCom_Worksearch();
	m_com_weapon = new CCom_WSCV();
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc )) ;	
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

	m_energybar_ui = new CUI_Energy_bar(this , 32);
	m_energybar_ui->Initialize();

	m_fterm_time = 0.f;
	m_vnormal = D3DXVECTOR2(0 ,0 );
	m_moveon = false;
	m_movetime = 0.f;
	m_build_idx = 0;
}

void CSCV::Update(void)
{	
	CObj::area_update();
	CWorkman::Fragment_Pos_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(!m_isSelect)
		m_ecmd_state = CMD_BASIC;

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


	if(ORDER_BUILDING == m_unitinfo.order)
	{
		//�ǹ� �������̴�.
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
			if(m_fterm_time > 1.2f) //SCV�� ������ �ð�
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
				m_unitinfo.state = ATTACK;
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
					if(CMyMath::pos_distance(m_vbuild_pos[m_build_idx] , m_vPos) > 2*2)
					{
						m_vPos += m_vcurdir * GETTIME * m_unitinfo.fspeed;
						m_unitinfo.state = MOVE;
					}
					else
					{
						m_movetime = 0.f;
						m_moveon = false;
					}
				}
			}

			if(IDLE == m_charge_building->GetUnitinfo().state)
			{
				if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
					CSoundDevice::GetInstance()->PlayVoiceSound(SND_V_SCV_COMPL , OBJ_SCV);
				//�ǹ� �ϼ�
				m_sortID = SORT_GROUND;
				m_unitinfo.state = IDLE;
				m_unitinfo.order = ORDER_NONE;
				m_charge_building = NULL;

				m_ecmd_state = CMD_BASIC;
			}
		}
	}

	if(true == m_is_preview)
	{
		m_main_preview->SetPos(CMouseMgr::GetInstance()->GetAddScrollvMousePt());
		CIngame_UIMgr::GetInstance()->SetPreview(m_main_preview);
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CSCV::Render(void)
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

void CSCV::Inputkey_reaction(const int& nkey)
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
			m_is_preview = false;

			//��ġ�� �����ϸ� �ǹ�����
		}
		//else
		//{
		//	//���� �� ���ٴ� ���̽�

		//	m_unitinfo.order = ORDER_NONE;
		//	m_main_preview->SetActive(false);
		//	m_is_preview = false;
		//}
	}


	m_is_preview = false;
	if(VK_RBUTTON == nkey)
	{
		/*((CBuilding_Preview*)m_main_preview)->SetActive(false);*/
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		CWorkman::SetMineral_mark(NULL);

		if(NULL != ptarget)
		{
			OBJID eobjid = ptarget->GetOBJNAME();

			if(OBJ_MINERAL == eobjid ||	OBJ_GAS == eobjid || OBJ_T_GAS == eobjid)
			{
				m_unitinfo.order = ORDER_GATHER;
				COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
				iter->second = m_com_worksearch;
				((CCom_Targetsearch*)m_com_worksearch)->SetTarget(ptarget);
			}
			else
			{
				//�������϶� �ƴҶ� ������..
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
			D3DXVECTOR2 goalpos;

			goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
	}

	if('W' == nkey)
	{
		m_eteamnumber = TEAM_1;
	}

	if(VK_ESCAPE == nkey)
	{
		m_ecmd_state = CMD_BASIC;
	}
}
void CSCV::Inputkey_reaction(const int& firstkey , const int& secondkey)
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
		CWorkman::SetMineral_mark(NULL);
	}
}
bool CSCV::Input_cmd(const int& nkey , bool* waitkey)
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

	if(VK_LBUTTON == nkey)
	{
		//���⼭ ����� �Է��ϰ�
		if(true == m_is_preview )
		{
			if(true == m_main_preview->Install_check())
			{
				//�ǹ���ġ�ϴ� ���� �߰�
				if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
					CSoundDevice::GetInstance()->PlayEffSound(SND_EFF_TBUILD , 0);

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
					CSoundDevice::GetInstance()->PlayVoiceSound(SND_V_SCV_ERR , OBJ_SCV);
			}
		}
	}

	else if('B' == nkey && CMD_BASIC == m_ecmd_state)
	{
		CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		if( true == pui->active_cmdbtn(6 , BTN_B_BUILD) )
		{
			m_ecmd_state = CMD_B_VIEW;
			//waitkey[nkey] = true;
		}
	}

	else if('V' == nkey && CMD_BASIC == m_ecmd_state)
	{
		CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		if( true == pui->active_cmdbtn(7 , BTN_V_BUILD) )
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
				if('A' == nkey)
				{
					MYRECT<float> vtx(40.f , 43.f , 32.f , 25.f);
					SetPreview_info(L"T_ACADEMY" , T_ACADEMY , 2 , 3 , vtx );
				}
				else if('B' == nkey)
				{
					MYRECT<float> vtx(48.f , 57.f , 40.f , 33.f);
					SetPreview_info(L"T_BARRACK" , T_BARRACK , 3 , 4 , vtx);
				}
				else if('C' == nkey)
				{
					MYRECT<float> vtx(58.f , 59.f , 41.f , 32.f);
					SetPreview_info(L"COMMANDCENTER" , T_COMMANDCENTER , 3 , 4 , vtx);
				}
				else if('E' == nkey)
				{
					MYRECT<float> vtx(48.f , 49.f , 32.f , 35.f);
					SetPreview_info(L"T_EB" , T_EB , 3 , 4 , vtx);
				}
				else if('R' == nkey)
				{
					MYRECT<float> vtx(56.f , 57.f , 32.f , 32.f);
					SetPreview_info(L"T_GAS" , T_GAS , 2 , 4 , vtx);
				}
				else if('S' == nkey)
				{
					MYRECT<float> vtx(38.f , 39.f , 22.f , 27.f);
					SetPreview_info(L"T_SUPPLY" , T_SUPPLY , 2 , 3 , vtx);
				}
				else if('T' == nkey)
				{
					MYRECT<float> vtx(16.f , 17.f , 32.f , 17.f);
					SetPreview_info(L"T_TURRET_PREVIEW" , T_TURRET , 2 , 2 , vtx);
				}
				else if('U' == nkey)
				{
					MYRECT<float> vtx(32.f , 33.f , 24.f , 17.f);
					SetPreview_info(L"T_BUNKER" , T_BUNKER , 2 , 3 , vtx);
				}
				else
					m_is_preview = false;
			}
			else if(CMD_V_VIEW == m_ecmd_state)
			{
				m_is_preview = true;

				if('A' == nkey)
				{
					MYRECT<float> vtx(48.f , 48.f , 32.f , 23.f);
					SetPreview_info(L"T_ARMOURY" , T_ARMOURY , 2 , 3 , vtx);
				}
				else if('F' == nkey)
				{
					MYRECT<float> vtx(56.f , 57.f , 40.f , 41.f);
					SetPreview_info(L"T_FACTORY" , T_FACTORY , 3 , 4 , vtx);
				}
				else if('I' == nkey)
				{
					MYRECT<float> vtx(48.f , 49.f , 38.f , 39.f);
					SetPreview_info(L"T_SIENCE" , T_SIENCE , 3 , 4 , vtx);
				}
				else if('S' == nkey)
				{
					MYRECT<float> vtx(48.f , 49.f , 40.f , 39.f);
					SetPreview_info(L"T_STARPORT" , T_STARPORT , 3 , 4 , vtx);
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
bool CSCV::Input_cmd(const int& firstkey , const int& secondkey)
{
	if( 1 == CUnitMgr::GetInstance()->GetSelectunit_size())
	{
		m_is_preview = true;
		if('B' == firstkey && 'A' == secondkey)
		{
			MYRECT<float> vtx(40.f , 43.f , 32.f , 25.f);
			SetPreview_info(L"T_ACADEMY" , T_ACADEMY , 2 , 3 , vtx );
		}
		else if('B' == firstkey && 'B' == secondkey)
		{
			MYRECT<float> vtx(48.f , 57.f , 40.f , 33.f);
			SetPreview_info(L"T_BARRACK" , T_BARRACK , 3 , 4 , vtx);
		}
		else if('B' == firstkey && 'C' == secondkey)
		{
			MYRECT<float> vtx(58.f , 59.f , 41.f , 32.f);
			SetPreview_info(L"COMMANDCENTER" , T_COMMANDCENTER , 3 , 4 , vtx);
		}
		else if('B' == firstkey && 'E' == secondkey)
		{
			MYRECT<float> vtx(48.f , 49.f , 32.f , 35.f);
			SetPreview_info(L"T_EB" , T_EB , 3 , 4 , vtx);
		}
		else if('B' == firstkey && 'R' == secondkey)
		{
			MYRECT<float> vtx(56.f , 57.f , 32.f , 32.f);
			SetPreview_info(L"T_GAS" , T_GAS , 2 , 4 , vtx);
		}
		else if('B' == firstkey && 'S' == secondkey)
		{
			MYRECT<float> vtx(38.f , 39.f , 22.f , 27.f);
			SetPreview_info(L"T_SUPPLY" , T_SUPPLY , 2 , 3 , vtx);
		}
		else if('B' == firstkey && 'T' == secondkey)
		{
			MYRECT<float> vtx(16.f , 17.f , 32.f , 17.f);
			SetPreview_info(L"T_TURRET_PREVIEW" , T_TURRET , 2 , 2 , vtx);
		}
		else if('B' == firstkey && 'U' == secondkey)
		{
			MYRECT<float> vtx(32.f , 33.f , 24.f , 17.f);
			SetPreview_info(L"T_BUNKER" , T_BUNKER , 2 , 3 , vtx);
		}
		else if('V' == firstkey && 'A' == secondkey)
		{
			MYRECT<float> vtx(48.f , 48.f , 32.f , 23.f);
			SetPreview_info(L"T_ARMOURY" , T_ARMOURY , 2 , 3 , vtx);
		}
		else if('V' == firstkey && 'F' == secondkey)
		{
			MYRECT<float> vtx(56.f , 57.f , 40.f , 41.f);
			SetPreview_info(L"T_FACTORY" , T_FACTORY , 3 , 4 , vtx);
		}
		else if('V' == firstkey && 'I' == secondkey)
		{
			MYRECT<float> vtx(48.f , 49.f , 38.f , 39.f);
			SetPreview_info(L"T_SIENCE" , T_SIENCE , 3 , 4 , vtx);
		}
		else if('V' == firstkey && 'S' == secondkey)
		{
			MYRECT<float> vtx(48.f , 49.f , 40.f , 39.f);
			SetPreview_info(L"T_STARPORT" , T_STARPORT , 3 , 4 , vtx);
		}
	}

	return false;
}

void CSCV::Create_Building(void)
{
	//if(CMyMath::Pos_to_index(m_vPos , 32) == 
	//	CMyMath::Pos_to_index(m_preview_info.vcenter_pos , 32))

	CObj* pobj = NULL;

	float fdistance = CMyMath::pos_distance(m_vPos , m_preview_info.vcenter_pos);
	float onestep = GETTIME*m_unitinfo.fspeed;

	if( fdistance < 32*32)
	{
		if(false == (m_main_preview)->Install_check(m_preview_info))
		{
			if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
				CSoundDevice::GetInstance()->PlayVoiceSound(SND_V_SCV_ERR , OBJ_SCV);

			m_unitinfo.order = ORDER_NONE;
			m_unitinfo.state = IDLE;
			m_ecmd_state = CMD_BASIC;		
			return;
		}


		m_unitinfo.order = ORDER_BUILDING;
		m_unitinfo.state = BUILD;
		if(T_COMMANDCENTER == m_preview_info.ebuild)
		{
			pobj = new CComandcenter(6.0f);
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_COMMAND);			
		}
		else if(T_FACTORY == m_preview_info.ebuild)
		{
			pobj = new CFactory;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_FACTORY);
		}
		else if(T_STARPORT == m_preview_info.ebuild)
		{
			pobj = new CStarport;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_STARPORT);
		}
		else if(T_SIENCE == m_preview_info.ebuild)
		{
			pobj = new CSience;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_SIENCE);
		}
		else if(T_BARRACK == m_preview_info.ebuild)
		{
			pobj = new CBarrack;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_BARRACK);
		}
		else if(T_EB == m_preview_info.ebuild)
		{
			pobj = new CEngineering;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_EB);
		}
		else if(T_SUPPLY == m_preview_info.ebuild)
		{
			pobj = new CSupplydepot;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_SUPPLY);
		}
		else if(T_ACADEMY == m_preview_info.ebuild)
		{
			pobj = new CAcademy;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_ACADEMY);
		}
		else if(T_ARMOURY == m_preview_info.ebuild)
		{
			pobj = new CArmoury;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_ARMOURY);
		}
		else if(T_BUNKER == m_preview_info.ebuild)
		{
			pobj = new CBunker;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_BUNKER);
		}
		else if(T_TURRET == m_preview_info.ebuild)
		{
			pobj = new CTurret;
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_TURRET);
		}

		if(NULL != pobj)
		{
			pobj->SetTeamNumber(m_eteamnumber);
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
		}

		m_charge_building = pobj;

		((CCom_Pathfind*)m_com_pathfind)->ClearPath();
		((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
	}
	else
	{
		//if(false == m_main_preview->Install_check(m_preview_info))
		//{
		//	if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
		//		CSoundDevice::GetInstance()->PlayVoiceSound(SND_V_SCV_ERR , OBJ_SCV);

		//	m_unitinfo.order = ORDER_NONE;
		//	m_unitinfo.state = IDLE;
		//	m_ecmd_state = CMD_BASIC;
		//}
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
			if(false == (m_main_preview)->Install_check(m_preview_info))
			{
				m_unitinfo.order = ORDER_NONE;
				m_unitinfo.state = IDLE;
				return;
			}

			m_unitinfo.order = ORDER_BUILDING;
			m_unitinfo.state = BUILD;

			pobj = new CT_gas(pgas_resorce);//�����ǹ��� �����ڿ��� ��������
			pobj->SetTeamNumber(m_eteamnumber);
			pobj->SetPos(m_preview_info.vpos );
			pobj->Initialize();
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_T_GAS);		

			m_charge_building = pobj;
		}

		((CCom_Pathfind*)m_com_pathfind)->ClearPath();
		((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
	}

}
void CSCV::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
{
	CSkill* pskill = ((CCom_CC*)m_com_cc)->GetDefensive();

	float tempdamage = 0.f;

	if(NULL != pskill)
	{
		int shild = ((CSkill_Defensive*)pskill)->GetShild();
		((CSkill_Defensive*)pskill)->SetDamage(idamage);

		if( shild - idamage > 0)
			return;
		else
			tempdamage = float(idamage - shild); 
	}
	else
		tempdamage = (float)idamage - (m_unitinfo.armor + m_upg_info[UPG_T_BIO_ARMOR].upg_cnt);

	if( ARMOR_SMALL == m_unitinfo.eArmorType)
	{
		if(DAMAGE_BOOM == edamagetype)
			tempdamage *= 0.5f;
	}
	else if( ARMOR_MEDIUM == m_unitinfo.eArmorType)
	{
		if(DAMAGE_VIBRATE == edamagetype)
			tempdamage *= 0.5f;
		else if(DAMAGE_BOOM == edamagetype)
			tempdamage *= 0.75f;
	}
	else if( ARMOR_LARGE == m_unitinfo.eArmorType)
	{
		if(DAMAGE_VIBRATE == edamagetype)
			tempdamage *= 0.25f;
	}

	m_unitinfo.hp -= (int)(tempdamage + 0.5f);

	if(m_unitinfo.hp <= 0)
	{
		//ų�� + 1 �ϸ� �Ǵµ�..
		m_unitinfo.hp = 0;
		m_bdestroy = true;
		Dead();
	}
	if(m_unitinfo.hp >= m_unitinfo.maxhp)
		m_unitinfo.hp = m_unitinfo.maxhp;
}
void CSCV::Update_Cmdbtn(void)
{	
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	if(CMD_BUILDING == m_ecmd_state)
	{
		for(int i = 0; i < 9; ++i)
			pui->Create_Cmdbtn(0 , L"" , BTN_NONE);
	}
	else if(CMD_B_VIEW == m_ecmd_state)
	{
		pui->T_Cmdbtn_B_buildsetting(m_eteamnumber);
		pui->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
	}
	else if(CMD_V_VIEW == m_ecmd_state)
	{
		pui->T_Cmdbtn_V_buildsetting(m_eteamnumber);
		pui->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
	}
	else
	{
		pui->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
		pui->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
		pui->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
		pui->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
		pui->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

		pui->Create_Cmdbtn(6 , L"BTN_BBUILD" , BTN_B_BUILD);
		pui->Create_Cmdbtn(7 , L"BTN_VBUILD" , BTN_V_BUILD);
	}
}

void CSCV::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_SCV" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran SCV" ,interface_pos.x + 320 , interface_pos.y + 390 );
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
	CFontMgr::GetInstance()->Setbatch_Font(L"���ݷ�: %d + %d" ,temp_info.damage , 0,
		interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));
	CFontMgr::GetInstance()->Setbatch_Font(L"����:%d + %d",m_unitinfo.armor, m_upg_info[UPG_T_BIO_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}


void CSCV::Dead(void)
{
	CSoundDevice::GetInstance()->PlayBattleSound(SND_B_SCVDTH , m_vPos);

	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND ,1.4f);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

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

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(-1 , m_eteamnumber);
}