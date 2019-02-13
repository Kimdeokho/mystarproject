#include "StdAfx.h"
#include "Sunken.h"

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
#include "Com_SunkenAnim.h"
#include "Com_Creep.h"
#include "Com_Collision.h"
#include "Com_Guardsearch.h"
#include "Com_WSunken.h"

#include "UI_Wireframe.h"
#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"

#include "Corpse.h"
#include "GeneraEff.h"

CSunken::CSunken(void)
{
}

CSunken::~CSunken(void)
{
	Release();
}

void CSunken::Initialize(void)
{
	m_vertex.left = 24.f;
	m_vertex.right = 24.f;
	m_vertex.top =  24.f;
	m_vertex.bottom = 24.f;

	CZerg_building::building_area_Initialize(2 , 2);
	CZerg_building::building_pos_Initialize(2 , 2);

	m_eOBJ_NAME = OBJ_SUNKEN;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_BUILDING;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;	
	m_unitinfo.maxhp = 300;
	m_unitinfo.hp = m_unitinfo.maxhp;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 7*32;
	m_unitinfo.attack_range = 6*32;
	m_unitinfo.fog_range = 32*10*2;
	m_unitinfo.fbuildtime = 10.f;
	m_unitinfo.armor = 2;

	m_sunken_anim = new CCom_SunkenAnim(L"Z_SUNKEN" , m_matWorld);
	m_com_creep = new CCom_Creep(false);

	m_sunken_anim->SetTextureName(L"Z_COCOON");
	m_sunken_anim->SetAnimation(L"BUILD_S");


	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_sunken_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , new CCom_WSunken() ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CREEP , m_com_creep )); //외부에서 넣어야할듯


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select72" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 62);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	m_build_maxhp = m_unitinfo.maxhp;
}

void CSunken::Update(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		//여기서 성큰애니메이션 추가해야겠네...
		m_sunken_anim->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.state)
	{
		m_sunken_anim->SetTextureName(L"Z_COCOON");

		m_build_hp += m_fbuild_tick * GETTIME;		

		if(m_build_hp >= m_build_maxhp)
		{
			//if(NULL ==GetComponent(COM_CREEP))
			//{
			//	m_com_creep = new CCom_Creep(false);
			//	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CREEP , m_com_creep )); //외부에서 넣어야할듯
			//	m_com_creep->Initialize(this);
			//}

			m_sunken_anim->SetAnimation(L"BIRTH_S");

			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.state = STATE_NONE;

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_SUNKEN , 1);
		}
	}
	else if(ATTACK == m_unitinfo.state)
	{
		float fangle = CMyMath::dir_to_dgree(m_vcurdir);

		if(fangle > 60 && fangle < 180)
			m_sunken_anim->SetAnimation(L"ATTACK1");
		else if(fangle >= 180 && fangle < 300)
			m_sunken_anim->SetAnimation(L"ATTACK2");
		else
			m_sunken_anim->SetAnimation(L"ATTACK3");
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , 111 , 400 , 300);
}

void CSunken::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();

	m_sunken_anim->Render();

	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CSunken::Inputkey_reaction(const int& nkey)
{

}

void CSunken::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CSunken::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	pui->Create_Cmdbtn(1, L"BTN_STOP", BTN_STOP);
	pui->Create_Cmdbtn(2, L"BTN_ATTACK", BTN_ATTACK);
}

void CSunken::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;	

		pui = new CUI_Wireframe(L"WIRE_SUNKEN" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Sunken" ,interface_pos.x + 320 , interface_pos.y + 390 );

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
void CSunken::Build_Complete(void)
{
	CComponent* pcom = new CCom_Guardsearch(SEARCH_ONLY_ENEMY );
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH , pcom ) );
	pcom->Initialize(this);

	m_unitinfo.state = IDLE;
}

void CSunken::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"BLOOD_BOOM" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"ZBD_S_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}

void CSunken::Release(void)
{
	CZerg_building::area_release();
}

