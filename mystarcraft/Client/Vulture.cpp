#include "StdAfx.h"
#include "Vulture.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_Pathfind.h"
#include "Com_VultureAnim.h"
#include "Com_Distancesearch.h"
#include "Com_WVulture.h"
#include "Com_UsingSkill.h"
#include "Com_CC.h"

#include "ObjMgr.h"
#include "LineMgr.h"
#include "TileManager.h"
#include "Com_Collision.h"
#include "FontMgr.h"
#include "UI_Select.h"

#include "UnitMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"
#include "GeneraEff.h"

#include "Ingame_UIMgr.h"

#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"

#include "Spidermine.h"
#include "MyMath.h"

#include "UI_Energy_bar.h"
#include "Skill_Defensive.h"
CVulture::CVulture(void)
{
}

CVulture::~CVulture(void)
{
		Release();
}

void CVulture::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize(); //맵 디스플레이

	m_curtex = NULL;
	m_com_pathfind = NULL;
	m_com_targetsearch = NULL;


	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_UNIT;
	m_eOBJ_NAME = OBJ_VULTURE;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_MECHANIC;
	m_unitinfo.eArmorType = ARMOR_MEDIUM;
	m_unitinfo.hp = 80;
	m_unitinfo.maxhp = m_unitinfo.hp;
	m_unitinfo.mp = 0;
	m_unitinfo.maxmp = 0;
	m_unitinfo.fspeed = 155;
	m_unitinfo.attack_range = 5*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 255;
	m_unitinfo.fog_range = 512;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;


	m_vertex.left = 16;
	m_vertex.right = 16;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,16);
	m_com_targetsearch = new CCom_Distancesearch(SEARCH_ONLY_ENEMY);
	m_com_anim = new CCom_VultureAnim(m_matWorld);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex);
	m_com_weapon = new CCom_WVulture();
	m_com_usingskill = new CCom_UsingSkill();
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc )) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon) );
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_USINGSKILL, m_com_usingskill) );


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_select_ui = new CUI_Select(L"Select32" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 32);
	m_energybar_ui->Initialize();

	m_landmine_cnt = 3;
	m_is_mine_install = false;

}

void CVulture::Update(void)
{
	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(MOVE == m_unitinfo.state || COLLISION == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}


	//if(true == m_is_mine_install)
	//{
	//	if(CMyMath::pos_distance(m_install_pos , m_vPos) <= 2)
	//	{
	//		//마인설치
	//		CObj* pObj = NULL;
	//		D3DXVECTOR2 vpos;
	//		pObj = new CSpidermine;

	//		pObj->SetPos(m_install_pos);
	//		pObj->Initialize();
	//		CObjMgr::GetInstance()->AddObject(pObj , OBJ_MINE);

	//		m_unitinfo.order = ORDER_NONE;
	//		m_unitinfo.state = IDLE;
	//		m_is_mine_install = false;
	//	}
	//}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CVulture::Render(void)
{
	if( false == m_unitinfo.is_active ||
		ORDER_BUNKER_BOARDING == m_unitinfo.order)
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

void CVulture::Inputkey_reaction(const int& nkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			if(NULL != ptarget)
				m_bmagicbox = false;

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}

		((CCom_UsingSkill*)m_com_usingskill)->SetUsingSkill(SO_NONE, NULL , m_install_pos);
	}

	m_is_landmine = false;
	if('Q' == nkey)
	{
		//m_fspeed = 30;
		//m_eteamnumber = TEAM_0;
	}
	if('W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		//m_skill_sp->use();
	}
}

void CVulture::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_MOVE_ATTACK;
		m_unitinfo.state = MOVE;
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}

		((CCom_UsingSkill*)m_com_usingskill)->SetUsingSkill(SO_NONE, NULL , m_install_pos);
	}

	if('I' == firstkey && VK_LBUTTON == secondkey)
	{
		//if( m_landmine_cnt >= 0 && m_upg_info[UPG_T_VFC1].upg_cnt >= 1)
		//if( m_landmine_cnt >= 0)
		{
			m_unitinfo.order = ORDER_USINGSKILL;
			m_unitinfo.state = MOVE;		
			m_is_mine_install = true;

			if(NULL != m_com_pathfind)
			{
				D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

				((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
				((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
				((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();

				//매직박스 가중치를 받은 Pos
				m_install_pos = ((CCom_Pathfind*)m_com_pathfind)->GetGoalPos();
				m_bmagicbox = false;
			}
			CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
			((CCom_UsingSkill*)m_com_usingskill)->SetUsingSkill(SO_INSTALL_MINE, ptarget , m_install_pos);
		}
	}
}
void CVulture::Input_cmd(const int& nkey , bool* waitkey)
{
	if('I' == nkey)
	{
		waitkey[nkey] = true;
	}
}
void CVulture::Release(void)
{
	CObj::area_release();

	m_com_pathfind = NULL;
}

void CVulture::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND ,1.4f);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}

void CVulture::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
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
		//킬수 + 1 하면 되는디..
		m_unitinfo.hp = 0;
		m_bdestroy = true;
		Dead();
	}
	if(m_unitinfo.hp >= m_unitinfo.maxhp)
		m_unitinfo.hp = m_unitinfo.maxhp;
}
void CVulture::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

	if(m_upg_info[UPG_T_VFC1].upg_cnt == 0)
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(6 , L"BTN_SPIDERMINE" , BTN_SPIDERMINE , false);
	else
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(6 , L"BTN_SPIDERMINE" , BTN_SPIDERMINE , true);
}

void CVulture::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_VULTURE" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Vulture" ,interface_pos.x + 320 , interface_pos.y + 390 );
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
	CFontMgr::GetInstance()->Setbatch_Font(L"공격력:%d + %d" ,temp_info.damage , m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt*2,
		interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));
	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}
