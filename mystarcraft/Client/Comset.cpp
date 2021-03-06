#include "StdAfx.h"
#include "Comset.h"

#include "Component.h"
#include "Com_PartAnim.h"
#include "Com_Collision.h"
#include "Com_fog.h"
#include "Com_AirPathfind.h"
#include "Com_Detect.h"

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
#include "Ingame_UIMgr.h"
#include "Comandcenter.h"

#include "LoopEff.h"
#include "Fog_object.h"
#include "ObjMgr.h"
#include "MouseMgr.h"

#include "UI_Wireframe.h"
#include "UI_Cmd_info.h"

#include "UI_Energy_bar.h"

CComset::CComset(CObj* command)
{
	m_mainbuilding = command;
}

CComset::~CComset(void)
{
	Release();
}

void CComset::Initialize(void)
{
	m_com_pathfind = NULL;

	CTerran_building::building_area_Initialize(2 , 2);
	CTerran_building::building_pos_Initialize(2 , 2);

	m_vertex.left = 32.f;//원래 37이던데 이유는 흠...
	m_vertex.right = 32.f;
	m_vertex.top =  16.f;
	m_vertex.bottom = 26.f;

	m_ebuild_tech = T_COMSET;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_COMSET;
	m_eteamnumber = m_mainbuilding->GetTeamNumber();

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 500;
	m_unitinfo.hp = 1;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 3.f;

	m_com_anim = new CCom_PartAnim(L"T_COMSET" , L"COMSET_LINK", m_matWorld);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select94" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 94 , m_vertex.bottom*1.8f);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();
}

void CComset::Update(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"BUILD");

		m_build_hp += m_fbuild_tick * GETTIME;
		m_unitinfo.hp = (int)m_build_hp;

		if(m_unitinfo.hp >= m_unitinfo.maxhp )
		{
			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.state = IDLE;
			CTerran_building::Build_Complete();
			m_mainbuilding->SetState(IDLE);
		}
	}

	//D3DXVECTOR2 vpos;
	//vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	//vpos.x -= CScrollMgr::m_fScrollX;
	//vpos.y -= CScrollMgr::m_fScrollY;
	//CFontMgr::GetInstance()->Setbatch_Font(L"@" , m_vPos.x - CScrollMgr::m_fScrollX, 
	//	m_vPos.y - CScrollMgr::m_fScrollY);


	CTerran_building::fire_eff_update();

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CComset::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}


void CComset::Inputkey_reaction(const int& nkey)
{
	if(nkey == 'S')
	{
	}
}

void CComset::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('S' == firstkey && VK_LBUTTON == secondkey)
	{		

		D3DXVECTOR2 vmousept;
		vmousept = CUnitMgr::GetInstance()->GetUnitGoalPos();
		CSoundDevice::GetInstance()->PlayBattleSound(SND_B_SCAN , vmousept);

		CObj* pobj = NULL;
		CComponent* pcom = new CCom_Detect;

		pobj = new CFog_object(320*2 , 10.f , vmousept , MOVE_AIR , m_eteamnumber);
		
		pobj->AddComponent(COM_DETECTOR , pcom);

		pobj->Initialize();		
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_FOG);

		CObj* peff = NULL;
		peff = new CGeneraEff( L"SCAN" , vmousept , D3DXVECTOR2(5.f,5.f) , SORT_AIR_EFF);
		peff->Initialize();
		CObjMgr::GetInstance()->AddEffect(peff);
	}
}
bool CComset::Input_cmd(const int& nkey , bool* waitkey)
{
	if('S' == nkey)
		waitkey[nkey] = true;

	return false;
}
void CComset::Setlink(bool blink , CObj* pobj)
{
	if(true == blink)
	{
		m_mainbuilding = pobj;
		((CCom_PartAnim*)m_com_anim)->play_direction(1);
	}
	else
	{
		m_mainbuilding = NULL;
		((CCom_PartAnim*)m_com_anim)->play_direction(-1);
	}
}

void CComset::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
	if(IDLE == m_unitinfo.state )
	{			
		if(NULL != m_mainbuilding)
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_SCAN" , BTN_SCAN , true);
	}
}

void CComset::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_COMSET" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font( L"Terran Comset" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction" , interface_pos.x + 320 , interface_pos.y + 415);
		}
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

	if(BUILD == m_unitinfo.state)
	{		
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , m_build_hp / (float)m_unitinfo.maxhp );
	}
}

void CComset::Dead(void)
{
	CSoundDevice::GetInstance()->PlayBattleSound(SND_B_TBMIDDLE_BOOM , m_vPos);

	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	if(NULL != m_mainbuilding)
		((CTerran_building*)m_mainbuilding)->SetPartBuilding(NULL);

}
void CComset::Release(void)
{
	CTerran_building::area_release();	
}
