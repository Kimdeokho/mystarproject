#include "StdAfx.h"
#include "Hatchery.h"

#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Ingame_UIMgr.h"
#include "FontMgr.h"
#include "UnitMgr.h"
#include "Area_Mgr.h"

#include "Com_fog.h"
#include "Com_Creep.h"
#include "Com_ZBuildingAnim.h"
#include "Com_Collision.h"
#include "Com_Larvahatch.h"

#include "UI_Wireframe.h"
#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"

CHatchery::CHatchery(void)
{
}
CHatchery::CHatchery(const float& fbuildtime )
{
	m_unitinfo.fbuildtime = fbuildtime;
}
CHatchery::~CHatchery(void)
{
	Release();
}

void CHatchery::Initialize(void)
{
	m_vertex.left = 49.f;
	m_vertex.right = 50.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 33.f;

	CZerg_building::building_area_Initialize(3 , 4);
	CZerg_building::building_pos_Initialize(3 , 4);

	m_eOBJ_NAME = OBJ_HATCERY;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_BUILDING;
	
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;	
	m_unitinfo.maxhp = 1250;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;

	m_com_anim = new CCom_ZBuildingAnim(L"Z_HATCHERY" , m_matWorld);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) );
	

	if(fabsf(m_unitinfo.fbuildtime) < FLT_EPSILON)
	{
		m_com_anim->SetAnimation(L"IDLE");

		m_unitinfo.hp = m_unitinfo.maxhp;
		m_build_hp = (float)m_unitinfo.maxhp;
		m_unitinfo.state = IDLE;

		m_com_creep = new CCom_Creep(true);
		m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CREEP , m_com_creep )); //외부에서 넣어야할듯

		m_com_larvahatch = new CCom_Larvahatch(3);
		m_componentlist.insert(COMPONENT_PAIR::value_type(COM_LARVAHATCH , m_com_larvahatch));	
	}
	else
	{
		m_com_anim->SetTextureName(L"Z_COCOON");
		m_com_anim->SetAnimation(L"BUILD_S");
		m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
		m_unitinfo.state = BUILD;
	}

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select146" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 146 , m_vertex.bottom*1.5f);
	m_energybar_ui->Initialize();

	m_build_maxhp = m_unitinfo.maxhp;
}

void CHatchery::Update(void)
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
	else if(BUILD == m_unitinfo.state)
	{
		m_com_anim->SetTextureName(L"Z_COCOON");

		m_build_hp += m_fbuild_tick * GETTIME;		

		if(OBJ_HATCERY == m_eOBJ_NAME)
		{
			if( 3 > (m_build_maxhp / m_build_hp) )
				m_com_anim->SetAnimation(L"BUILD_M");
			else
				m_com_anim->SetAnimation(L"BUILD_S");

			m_unitinfo.hp = (int)m_build_hp;

			if(m_unitinfo.hp >= m_build_maxhp )
				m_com_anim->SetAnimation(L"BIRTH_M");
		}
		else if(OBJ_LAIR == m_eOBJ_NAME)
		{
			m_com_anim->SetAnimation(L"BUILD_M");

			if(m_build_hp >= m_build_maxhp)
				m_com_anim->SetAnimation(L"BIRTH_M");
		}
		else if(OBJ_HIVE == m_eOBJ_NAME)
		{
			if( (m_build_maxhp / m_build_hp) > 2)
				m_com_anim->SetAnimation(L"BUILD_M");
			else
				m_com_anim->SetAnimation(L"BUILD_L");

			if(m_build_hp >= m_build_maxhp)
				m_com_anim->SetAnimation(L"BIRTH_L");
		}

		if(m_build_hp >= m_build_maxhp)
		{
			m_unitinfo.hp = m_build_maxhp;
			m_unitinfo.maxhp = m_build_maxhp;
			m_unitinfo.state = STATE_NONE;

			if(NULL ==GetComponent(COM_CREEP))
			{
				m_com_creep = new CCom_Creep(false);
				m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CREEP , m_com_creep )); //외부에서 넣어야할듯
				m_com_creep->Initialize(this);
			}
			if(NULL ==GetComponent(COM_LARVAHATCH))
			{
				m_com_larvahatch = new CCom_Larvahatch(1);
				m_componentlist.insert(COMPONENT_PAIR::value_type(COM_LARVAHATCH , m_com_larvahatch));
				m_com_larvahatch->Initialize(this);
			}

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_HATCHERY , 1);
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , 111 , 400 , 300);
}
void CHatchery::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();

	m_com_anim->Render();

	m_energybar_ui->Render();
}

void CHatchery::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		m_rallypoint = CUnitMgr::GetInstance()->GetUnitGoalPos();

		if(this == ptarget)
		{
			m_is_rally = false;
			m_rallypoint = m_vPos;
		}
		else
		{
			m_is_rally = true;
			rallypoint_pathfinding();
		}
	}

	if('L' == nkey)
	{
		m_eOBJ_NAME = OBJ_LAIR;

		m_build_hp = 1250;
		m_build_maxhp = 1750;
		m_unitinfo.fbuildtime = 10.f;
		m_fbuild_tick = float(m_build_maxhp - m_build_hp)/m_unitinfo.fbuildtime;

		m_com_anim->SetOriginal_texname(L"Z_LAIR");

		m_unitinfo.state = BUILD;


	}

	if('H' == nkey)
	{
		m_eOBJ_NAME = OBJ_HIVE;

		m_build_hp = 1750;
		m_build_maxhp = 2500;
		m_unitinfo.fbuildtime = 10.f;
		m_fbuild_tick = float(m_build_maxhp - m_build_hp)/m_unitinfo.fbuildtime;

		m_com_anim->SetOriginal_texname(L"Z_HIVE");

		m_unitinfo.state = BUILD;
	}


}
void CHatchery::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
void CHatchery::Input_cmd(const int& nkey , bool* waitkey)
{
	if('S' == nkey)
	{
		waitkey[nkey] = false;
		CUnitMgr::GetInstance()->discharge_unit();
		m_com_larvahatch->Select_Larva();
	}
}

void CHatchery::Input_cmd(const int& firstkey , const int& secondkey)
{
}
void CHatchery::rallypoint_pathfinding(void)
{
	if(!m_rallypath.empty())
		m_rallypath.clear();

	int tempidx = Getcuridx(32);
	int preidx = tempidx;
	int igoalidx = CMyMath::Pos_to_index(m_rallypoint , 32);

	int icurstepcnt = 0;
	const int	istep = 9;
	D3DXVECTOR2 temppos;
	int	loopcnt = 0;

	int	oriidx = 0;
	BYTE byfloor = 0;

	short*	flowfieldpath = CTileManager::GetInstance()->Get_flowfield_node();

	while(true)
	{
		//일일히 담는것이아니라 n스텝당 한번씩 담자

		loopcnt = 0;

		temppos = CMyMath::index_to_Pos( flowfieldpath[tempidx]  , SQ_TILECNTX , SQ_TILESIZEX);

		if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(tempidx))
		{
			// 그지역이 장애물이라면
			temppos = CMyMath::index_to_Pos( preidx , SQ_TILECNTX , SQ_TILESIZEX);

			m_rallypath.push_back( temppos );

			break;
		}

		if(tempidx == igoalidx)
		{
			//최종지점 도착
			m_rallypath.push_back( m_rallypoint );
			break;
		}

		if( 0 != icurstepcnt &&
			0 == icurstepcnt % istep )
		{
			m_rallypath.push_back( temppos );
		}
		++icurstepcnt;

		preidx = tempidx;
		tempidx = flowfieldpath[tempidx]; //다음 경로로 가는 인덱스를 준다
	}
}
void CHatchery::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pcmd = CIngame_UIMgr::GetInstance()->GetCmd_info();
	if(IDLE == m_unitinfo.state )
	{			
		pcmd->Create_Cmdbtn(0 , L"BTN_SCV" , BTN_SCV , true);
		pcmd->Create_Cmdbtn(8 , L"BTN_TAKE_OFF" , BTN_TAKE_OFF , true);

		if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(T_ACADEMY))
			pcmd->Create_Cmdbtn(6 , L"BTN_COMSET" , BTN_COMSET , true);
		else
			pcmd->Create_Cmdbtn(6 , L"BTN_COMSET" , BTN_COMSET , false);

		if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(T_GHOST_ADDON))
			pcmd->Create_Cmdbtn(7 , L"BTN_NC_PART" , BTN_NC_PART , true);
		else
			pcmd->Create_Cmdbtn(7 , L"BTN_NC_PART" , BTN_NC_PART , false);

	}
	else if( PRODUCTION == m_unitinfo.state)
	{
		pcmd->Create_Cmdbtn(0 , L"BTN_SCV" , BTN_SCV , true);
	}
	else if(AIR_IDLE == m_unitinfo.state ||
		TAKE_OFF == m_unitinfo.state)
	{
		pcmd->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE , true);
		pcmd->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP , true);
		pcmd->Create_Cmdbtn(8 , L"BTN_LANDING" , BTN_LANDING , true);
	}
}
void CHatchery::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_COMMAND" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		if(OBJ_HATCERY == m_eOBJ_NAME)
			CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Hatchery" ,interface_pos.x + 320 , interface_pos.y + 390 );
		else if(OBJ_LAIR == m_eOBJ_NAME)
			CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Lair" ,interface_pos.x + 320 , interface_pos.y + 390 );
		else if(OBJ_HIVE == m_eOBJ_NAME)
			CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Hive" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction" , interface_pos.x + 320 , interface_pos.y + 415);
		}
	}

	//--------------------계속해서 갱신받는 부분

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

	if(BUILD == m_unitinfo.state)
	{		
		if(OBJ_HATCERY == m_eOBJ_NAME)
			CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , m_build_hp / (float)m_build_maxhp );
		else if(OBJ_LAIR == m_eOBJ_NAME)
			CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , (m_build_hp - 1250) / (float)(m_build_maxhp - 1250));
		else if(OBJ_HIVE == m_eOBJ_NAME)
			CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , (m_build_hp - 1750)/ (float)(m_build_maxhp - 1750) );
	}

	//-------------------------------------------
}

void CHatchery::Dead(void)
{

}
void CHatchery::Release(void)
{
	CZerg_building::area_release();
}
