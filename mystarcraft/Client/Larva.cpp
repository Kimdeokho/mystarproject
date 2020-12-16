#include "StdAfx.h"
#include "Larva.h"

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
#include "ObjMgr.h"
#include "UI_Resource.h"

#include "Com_fog.h"
#include "Com_LarvaAnim.h"
#include "Com_CC.h"
#include "Com_Collision.h"
#include "Com_Larvahatch.h"

#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "Building_Preview.h"
#include "GeneraEff.h"

#include "Input_Interface.h"

#include "MyCmd_Building.h"
#include "UI_Cmd_info.h"
#include "Corpse.h"
#include "Larva_egg.h"

#include "FontMgr.h"
#include "UI_Wireframe.h"

CLarva::CLarva(int hatch_number)
{
	m_hatch_num = hatch_number;
}

CLarva::~CLarva(void)
{
	Release();
}

void CLarva::Initialize(void)
{
	CObj::unit_area_Initialize();

	m_eOBJ_NAME = OBJ_LARVA;
	m_sortID = SORT_CORPSE;
	m_ecategory = CATEGORY_UNIT;

	m_unitinfo.etribe = TRIBE_ZERG;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = BURROW;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.maxhp = 40;
	m_unitinfo.hp = 40;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 22;
	m_unitinfo.attack_range = 0*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 0*32;
	m_unitinfo.fog_range = 32*4*2;
	m_unitinfo.armor = 10;

	m_vertex.left = 8.0f;
	m_vertex.right = 8.0f;
	m_vertex.top =  8.0f;
	m_vertex.bottom = 8.0f;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	m_com_anim = new CCom_LarvaAnim(m_matWorld);
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false)) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select22" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 22 , 30);
	m_energybar_ui->Initialize();

	m_waypoint[0].x = m_vPos.x - 16;
	m_waypoint[0].y = m_vPos.y + 4;

	m_waypoint[1].x = m_vPos.x + 16;
	m_waypoint[1].y = m_vPos.y + 4;

	m_waypoint[2].x = m_vPos.x + 16;
	m_waypoint[2].y = m_vPos.y + 16;

	m_waypoint[3].x = m_vPos.x - 16;
	m_waypoint[3].y = m_vPos.y + 16;

	m_moveswitch = false;
	m_idletime = 0.f;
	m_standard = rand()%3 + 1;
	m_wayidx = rand()%4;
}

void CLarva::Update(void)
{
	CObj::area_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(BURROW == m_unitinfo.state)
	{
		if(!m_moveswitch)
			m_com_anim->SetAnimation(L"IDLE");
		else
			m_com_anim->SetAnimation(L"MOVE");
	}
	//else if(MOVE == m_unitinfo.state)
	//{
	//	m_com_anim->SetAnimation(L"MOVE");
	//}


	/*
	움직이고, 몇초간 멈추고
	*/

	if(!m_moveswitch)
	{
		//IDLE
		m_unitinfo.state = BURROW;
		m_idletime += GETTIME;
		if(m_standard < (DWORD)m_idletime)
		{
			m_wayidx += 1;
			m_wayidx %= 4;

			D3DXVec2Normalize(&m_vcurdir , &(m_waypoint[m_wayidx] - m_vPos));
			m_moveswitch = true;
			m_idletime = 0.f;
		}
	}
	else
	{
		m_unitinfo.state = BURROW;

		float onestep = GETTIME * m_unitinfo.fspeed;		
		m_vPos += m_vcurdir * onestep;

		if(CMyMath::pos_distance(m_vPos , m_waypoint[m_wayidx]) < onestep*onestep*2)
		{
			m_standard = rand()%2 + 1;
			m_moveswitch = false;
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CLarva::Render(void)
{
	if(false == m_unitinfo.is_active)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();
	m_com_cc->Render();
	m_energybar_ui->Render();

	//m_com_pathfind->Render();
	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CLarva::Inputkey_reaction(const int& nkey)
{
	CObj* pobj = NULL;

	CUI_Resource* pui = CIngame_UIMgr::GetInstance()->GetResource_UI();
	if('D' == nkey)
	{
		if(pui->is_excess_of_population(1 , m_eteamnumber) &&
			pui->SetResource(-50 , 0,  m_eteamnumber))
		{
			pui->SetPopvalue(1.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_DRONE , m_hatch_num , 1 , L"DRONE_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if('H' == nkey)
	{
		if(pui->is_excess_of_population(1 , m_eteamnumber) &&
			pui->SetResource(-75 , -25,  m_eteamnumber))
		{
			pui->SetPopvalue(1.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_HYDRA , m_hatch_num , 1 , L"HYDRA_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if('Z' == nkey)
	{
		if(pui->is_excess_of_population(1 , m_eteamnumber) &&
			pui->SetResource(-50 , 0,  m_eteamnumber))
		{
			pui->SetPopvalue(1.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_ZERGLING , m_hatch_num , 2 , L"ZERGLING_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if('U' == nkey)
	{
		if(pui->is_excess_of_population(4 , m_eteamnumber) &&
			pui->SetResource(-200 , -200,  m_eteamnumber))
		{
			pui->SetPopvalue(4.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_ULTRA , m_hatch_num , 1 , L"ULTRA_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if('O' == nkey)
	{
		if(pui->SetResource(-100 , 0,  m_eteamnumber))
		{		
			pobj = new CLarva_egg(1.f , OBJ_OVERLOAD , m_hatch_num , 1 , L"OVER_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
		
	}

	if('M' == nkey)
	{
		if(pui->is_excess_of_population(2 , m_eteamnumber) &&
			pui->SetResource(-100 , -100,  m_eteamnumber))
		{
			pui->SetPopvalue(2.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_MUTAL , m_hatch_num , 1 , L"MUTAL_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if('S' == nkey)
	{
		if(pui->is_excess_of_population(1 , m_eteamnumber) &&
			pui->SetResource(-25 , -75,  m_eteamnumber))
		{
			pui->SetPopvalue(1.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_SCOURGE , m_hatch_num , 2 , L"SCOURGE_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if('Q' == nkey)
	{
		if(pui->is_excess_of_population(2 , m_eteamnumber) &&
			pui->SetResource(-100 , -100,  m_eteamnumber))
		{
			pui->SetPopvalue(2.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_QUEEN , m_hatch_num , 1 , L"QUEEN_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if('F' == nkey)
	{
		if(pui->is_excess_of_population(1 , m_eteamnumber) &&
			pui->SetResource(-50 , -150,  m_eteamnumber))
		{
			pui->SetPopvalue(1.f , m_eteamnumber);
			pobj = new CLarva_egg(1.f , OBJ_DEFILER , m_hatch_num , 1 , L"DEFILER_BIRTH");
			CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
		}
	}

	if( NULL != pobj)
	{
		pobj->SetPos(m_vPos);
		pobj->SetTeamNumber(m_eteamnumber);
		pobj->Initialize();

		CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
		SetDestroy(true);
	}
}

void CLarva::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
}

bool CLarva::Input_cmd(const int& nkey , bool* waitkey)
{
	if('D' == nkey)
		return true;

	else if('H' == nkey)
		return true;

	else if('Z' == nkey)
		return true;

	else if('U' == nkey)
		return true;

	else if('O' == nkey)
		return true;

	else if('M' == nkey)
		return true;

	else if('S' == nkey)
		return true;

	else if('Q' == nkey)
		return true;

	else if('B' == nkey)
		return true;

	else if('F' == nkey)
		return true;

	return false;
}

bool CLarva::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}

void CLarva::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	pui->Create_Cmdbtn(0 , L"BTN_DRONE" , BTN_DRONE , true);
	pui->Create_Cmdbtn(2 , L"BTN_OVERLOAD" , BTN_OVERLOAD , true);

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_SPWANING_POOL ,m_eteamnumber))
		pui->Create_Cmdbtn(1 , L"BTN_ZERGLING" , BTN_ZERGLING , true);
	else
		pui->Create_Cmdbtn(1 , L"BTN_ZERGLING" , BTN_ZERGLING , false);

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_HYDRADEN ,m_eteamnumber))
		pui->Create_Cmdbtn(3 , L"BTN_HYDRA" , BTN_HYDRA , true);
	else
		pui->Create_Cmdbtn(3 , L"BTN_HYDRA" , BTN_HYDRA, false);

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_SPIRE ,m_eteamnumber))
	{
		pui->Create_Cmdbtn(4 , L"BTN_MUTAL" , BTN_MUTAL , true);
		pui->Create_Cmdbtn(5 , L"BTN_SCOURGE" , BTN_SCOURGE , true);
	}
	else
	{
		pui->Create_Cmdbtn(4 , L"BTN_MUTAL" , BTN_MUTAL , false);
		pui->Create_Cmdbtn(5 , L"BTN_SCOURGE" , BTN_SCOURGE , false);
	}

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_QUEEN_NEST ,m_eteamnumber))
		pui->Create_Cmdbtn(6 , L"BTN_QUEEN" , BTN_QUEEN , true);
	else
		pui->Create_Cmdbtn(6 , L"BTN_QUEEN" , BTN_QUEEN , false);

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_ULTRA_CAVE ,m_eteamnumber))
		pui->Create_Cmdbtn(7 , L"BTN_ULTRA" , BTN_ULTRA , true);
	else
		pui->Create_Cmdbtn(7 , L"BTN_ULTRA" , BTN_ULTRA , false);

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_DEFILER_MOUND ,m_eteamnumber))
		pui->Create_Cmdbtn(8 , L"BTN_DEFILER" , BTN_DEFILER , true);
	else
		pui->Create_Cmdbtn(8 , L"BTN_DEFILER" , BTN_DEFILER , false);
}

void CLarva::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_LARVA" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Larva" ,interface_pos.x + 320 , interface_pos.y + 390 );
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

	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, 0
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}

void CLarva::Dead(void)
{
	CObj* pobj = new CCorpse(L"LARVA_DEAD" , L"LARVA_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);
}

void CLarva::Release(void)
{
	CObj::area_release();

	CObj* pobj = CObjMgr::GetInstance()->obj_alivecheck(m_hatch_num);
	if(NULL != pobj)
	{
		CCom_Larvahatch* pcom = ((CCom_Larvahatch*)(pobj->GetComponent(COM_LARVAHATCH)));

		if(NULL != pcom)
		{
			pcom->Decrease_Larva(this);
		}
	}
}
