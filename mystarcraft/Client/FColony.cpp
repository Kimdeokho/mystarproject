#include "StdAfx.h"
#include "FColony.h"

#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Ingame_UIMgr.h"
#include "FontMgr.h"
#include "ObjMgr.h"
#include "UnitMgr.h"
#include "LineMgr.h"

#include "Com_fog.h"
#include "Com_Creep.h"
#include "Com_ZBuildingAnim.h"
#include "Com_Creep.h"
#include "Com_Collision.h"

#include "UI_Wireframe.h"
#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"

#include "Sunken.h"
#include "Spore.h"

#include "Corpse.h"
#include "GeneraEff.h"

CFColony::CFColony(void)
{
}

CFColony::~CFColony(void)
{
	Release();
}

void CFColony::Initialize(void)
{
	m_vertex.left = 24.f;
	m_vertex.right = 24.f;
	m_vertex.top =  24.f;
	m_vertex.bottom = 24.f;

	CZerg_building::building_area_Initialize(2 , 2);
	CZerg_building::building_pos_Initialize(2 , 2);

	m_eOBJ_NAME = OBJ_FCOLONY;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_BUILDING;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;	
	m_unitinfo.maxhp = 400;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 32*10*2;
	m_unitinfo.fbuildtime = 1.f;

	m_com_anim = new CCom_ZBuildingAnim(L"Z_FCOLONY" , m_matWorld);
		
	m_com_anim->SetTextureName(L"Z_COCOON");
	m_com_anim->SetAnimation(L"BUILD_S");

	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) );		
	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select72" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 64);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	m_build_maxhp = m_unitinfo.maxhp;
}

void CFColony::Update(void)
{
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
		m_unitinfo.hp = (int)m_build_hp;

		if(m_build_hp >= m_build_maxhp)
		{
			if(NULL ==GetComponent(COM_CREEP))
			{
				m_com_creep = new CCom_Creep(false);
				m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CREEP , m_com_creep )); //외부에서 넣어야할듯
				m_com_creep->Initialize(this);
			}

			m_com_anim->SetAnimation(L"BIRTH_S");

			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.state = STATE_NONE;

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_FCOLONY , 1 , m_eteamnumber);
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , 111 , 400 , 300);
}

void CFColony::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();

	m_com_anim->Render();

	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CFColony::Inputkey_reaction(const int& nkey)
{
	if('U' == nkey)
	{
		CObj* pobj = new CSunken;
		pobj->SetPos(m_vPos - m_weight);
		pobj->SetTeamNumber(m_eteamnumber);
		pobj->Initialize();

		CObjMgr::GetInstance()->AddObject(pobj , OBJ_SUNKEN);
		SetDestroy(true);

	}

	if('S' == nkey)
	{
		CObj* pobj = new CSpore;
		pobj->SetPos(m_vPos - m_weight);
		pobj->SetTeamNumber(m_eteamnumber);
		pobj->Initialize();

		CObjMgr::GetInstance()->AddObject(pobj , OBJ_SUNKEN);
		SetDestroy(true);

	}
}
void CFColony::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

bool CFColony::Input_cmd(const int& nkey , bool* waitkey)
{
	if('U' == nkey)
		return true;
	else if('S' == nkey)
		return true;

	return false;
}

bool CFColony::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}
void CFColony::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_SPWANING_POOL ,m_eteamnumber))
		pui->Create_Cmdbtn(6 , L"BTN_SUNKEN" , BTN_SUNKEN , true);
	else
		pui->Create_Cmdbtn(6 , L"BTN_SUNKEN" , BTN_SUNKEN, false);

	if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_CHAMBER ,m_eteamnumber))
		pui->Create_Cmdbtn(7 , L"BTN_SPORE" , BTN_SPORE , true);
	else
		pui->Create_Cmdbtn(7 , L"BTN_SPORE" , BTN_SPORE, false);
}

void CFColony::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;	

		pui = new CUI_Wireframe(L"WIRE_COLONY" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Colony" ,interface_pos.x + 320 , interface_pos.y + 390 );

		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

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
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , m_build_hp / (float)m_build_maxhp );
	}
}

void CFColony::Dead(void)
{
	CSoundDevice::GetInstance()->PlayBattleSound(SND_B_ZBUILD_BOOM , m_vPos);

	CObj* pobj = new CGeneraEff(L"BLOOD_BOOM" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"ZBD_S_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

}

void CFColony::Release(void)
{
	CZerg_building::area_release();
}


