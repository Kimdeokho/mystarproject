#include "StdAfx.h"
#include "Tank.h"

#include "Tankbody.h"
#include "Tankbarrel.h"

#include "Siegebody.h"
#include "Siegebarrel.h"


#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_Pathfind.h"
#include "Com_TankbodyAnim.h"
#include "Com_SiegebodyAnim.h"
#include "Com_Weapon.h"
#include "Com_CC.h"

#include "TileManager.h"

#include "ScrollMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "MyMath.h"
#include "UnitMgr.h"
#include "GeneraEff.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "UI_Select.h"

#include "ComanderMgr.h"

#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_Energy_bar.h"
#include "Skill_Defensive.h"
CTank::CTank(void)
{
	m_tankbarrel = NULL;

}

CTank::~CTank(void)
{
	Release();
}

void CTank::Initialize(void)
{
	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	CObj::unit_area_Initialize();
	CUnit::Initialize();

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_UNIT;
	m_eteamnumber = TEAM_0;
	m_eOBJ_NAME = OBJ_TANK;


	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.esize = SIZE_MEDIUM;
	m_unitinfo.erace = OBJRACE_MECHANIC;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 150;
	m_unitinfo.maxhp = 150;
	m_unitinfo.mp = 0;
	m_unitinfo.armor = 1;
	m_unitinfo.fspeed = 68;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 16;
	m_vertex.right = 16;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_bsiegemode = false;
	m_btransform_ready = false;


	m_tankbarrel = new CTankbarrel(this);
	m_tankbarrel->SetPos(m_vPos);
	m_tankbarrel->SetObjID(m_obj_id);



	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 , 16);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex , true);

	m_com_tankanim = new CCom_TankbodyAnim(m_matWorld);
	m_com_siegeanim = new CCom_SiegebodyAnim(m_matWorld);
	m_com_cc = new CCom_CC();
	m_com_anim = m_com_tankanim;
	

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc )) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_tankbarrel->Initialize();//포신은 제일 마지막에 초기화

	m_select_ui = new CUI_Select(L"Select62" , m_vPos , 8);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 62 , m_vertex.bottom*2.f);
	m_energybar_ui->Initialize();

	m_bcheck = false;

}

void CTank::Update(void)
{
	CObj::area_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
		//m_tankbarrel->SetState(IDLE); //아 이거 문젠데....
	}
	else if(ATTACK == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(MOVE == m_unitinfo.state || COLLISION == m_unitinfo.state)
	{
		if(true == m_bsiegemode )
		{
		}
		else
		{
			//((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
			m_tankbarrel->Setdir(m_vcurdir);			
		}
		//m_tankbarrel->SetState( m_unitinfo.state );
	}
	else if(TRANSFORMING == m_unitinfo.state)
	{
		if(true == m_btransform_ready)
		{
			if(true == ((CCom_Animation*)m_com_anim)->GetAnimation_end())
			{
				m_unitinfo.state = IDLE;

				if(false == m_bsiegemode)
				{
					m_bsiegemode = true;
					((CTankbarrel*)m_tankbarrel)->TransformSiegebarrel();					
				}
				else
				{
					//시즈모드 상태였다면
					m_bsiegemode = false;
					Transform_Tankbody();
					((CTankbarrel*)m_tankbarrel)->TransformTankbarrel();
				}
				m_btransform_ready = false;
			}			
		}
		else
		{
			if(false == m_bsiegemode)
			{				
				if( true == ((CCom_Animation*)m_com_anim)->GetRotationComplete() /*&&
				//	true == ((CTankbarrel*)m_tankbarrel)->GetTransformReady()*/)
				{				
					m_btransform_ready = true; // 변신 준비완료
					Transform_Siegebody();
				}
			}
		}
	}
	if(ORDER_NONE == m_unitinfo.order)
	{
		m_tankbarrel->SetOrder(ORDER_NONE);
	}

	barrel_sync();
	m_tankbarrel->SetActive(m_unitinfo.is_active);
	m_tankbarrel->Update();

	//if(m_bcheck)
	//{
	//	CFontMgr::GetInstance()->Setbatch_Font(L"@" , m_vPos.x - CScrollMgr::m_fScrollX - 15, 
	//		m_vPos.y - CScrollMgr::m_fScrollY);
	//}

	//int dirdix = ((CCom_Animation*)m_com_anim)->GetCurDirIdx();
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" ,dirdix , m_vPos.x - CScrollMgr::m_fScrollX, 
	//	m_vPos.y - CScrollMgr::m_fScrollY);

	//dirdix = ((CCom_Animation*)m_com_anim)->GetCurnewtestIdx();
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" ,dirdix , m_vPos.x - CScrollMgr::m_fScrollX + 13, 
	//	m_vPos.y - CScrollMgr::m_fScrollY);

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CTank::Render(void)
{
	if( false == m_unitinfo.is_active )
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();
	m_tankbarrel->Render();
	m_com_cc->Render();

	m_energybar_ui->Render();

	if(NULL != m_com_pathfind)
		m_com_pathfind->Render();
}
void CTank::Transform(void)
{
	((CCom_Animation*)m_com_anim)->SetAnimation(L"SIEGEBODY_TRANS");
}
void CTank::Transform_Tankbody(void)
{
	//역변신
	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_ANIMATION);
	m_componentlist.erase(iter);


	m_com_anim = m_com_tankanim;
	m_com_anim->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind));

	((CCom_Collision*)m_com_collision)->SetCollsearch(true);

	//변신 끝
	m_btransform_ready = false;

	m_eOBJ_NAME = OBJ_TANK;
	m_tankbarrel->SetObjName(OBJ_TANK);
	
}

void CTank::Transform_Siegebody(void)
{		

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_ANIMATION);
	m_componentlist.erase(iter);

	m_com_anim = m_com_siegeanim;
	m_com_anim->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));

	((CCom_Collision*)m_com_collision)->SetCollsearch(false);

	m_eOBJ_NAME = OBJ_SIEGETANK;
	m_tankbarrel->SetObjName(OBJ_SIEGETANK);
}

void CTank::barrel_sync(void)
{
	D3DXVECTOR2 vbarrelpos;
	
	vbarrelpos = m_vPos - m_vcurdir*8;
	vbarrelpos.y -= 5;
	((CTankbarrel*)m_tankbarrel)->SetbarrelPos(m_vPos , vbarrelpos);

	m_tankbarrel->SetActive(m_unitinfo.is_active);
}
void CTank::Inputkey_reaction(const int& nkey)
{
	m_tankbarrel->Inputkey_reaction(nkey);

	if(TRANSFORMING == m_unitinfo.state )
		return;

	if( 'O' == nkey)
	{
		//시즈모드 업글이 되어있따면

		m_bcheck = true;
		//if(TRANSFORMING != m_unitinfo.state)
		{
			m_unitinfo.state = TRANSFORMING;
			if(false == m_bsiegemode)
			{				
				m_vcurdir = CMyMath::dgree_to_dir(5*11.25f);
			}
		}

		((CCom_Pathfind*)m_com_pathfind)->SetMultithread(false);
		((CCom_Pathfind*)m_com_pathfind)->ClearPath();
		COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
		if( iter != m_componentlist.end())
			m_componentlist.erase(iter);

	}
	if( 'W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		m_tankbarrel->SetTeamNumber(TEAM_1);
	}
	if(VK_RBUTTON == nkey)
	{
		if(true == m_bsiegemode)
		{
		}
		else
		{
			if(TRANSFORMING != m_unitinfo.state)
			{
				//m_unitinfo.state = MOVE;
				m_unitinfo.order = ORDER_MOVE;
				m_tankbarrel->SetOrder(ORDER_MOVE);
				//m_tankbarrel->SetState(MOVE);

				if(NULL != m_com_pathfind)
				{
					CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
					if(NULL != ptarget)
						m_bmagicbox = false;

					D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

					((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
					((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
					((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
					m_bmagicbox = false;
				}
			}

		}
	}


}

void CTank::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	m_tankbarrel->Inputkey_reaction(firstkey , secondkey);

	if(TRANSFORMING == m_unitinfo.state)
		return;

	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		if(true == m_bsiegemode)
		{

		}
		else
		{
			if(TRANSFORMING != m_unitinfo.state)
			{
				m_unitinfo.order = ORDER_MOVE_ATTACK;
				//m_unitinfo.state = MOVE;
				m_tankbarrel->SetOrder(ORDER_MOVE_ATTACK);
				//m_tankbarrel->SetState(MOVE);				

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

	}
}
void CTank::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
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
		tempdamage = (float)idamage - (m_unitinfo.armor + m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt);

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
bool CTank::GetSiegemode(void)
{
	return m_bsiegemode;
}
bool CTank::GetTransformReady(void)
{
	return m_btransform_ready;
}
void CTank::SetTransformReady(bool btransform_ready)
{
	m_btransform_ready = btransform_ready;
}
void CTank::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	Safe_Delete(m_tankbarrel);

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);

	if(iter != m_componentlist.end())
	{
		m_componentlist.erase(iter);		
	}
	Safe_Delete(m_com_pathfind);

	iter = m_componentlist.find(COM_ANIMATION);

	if(iter != m_componentlist.end())
	{
		m_componentlist.erase(iter);		
	}
	Safe_Delete(m_com_tankanim);
	Safe_Delete(m_com_siegeanim);
	m_com_anim = NULL;
}

void CTank::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"LARGEBANG" , m_vPos , D3DXVECTOR2(0.85f,0.85f) , SORT_GROUND );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}
void CTank::Update_Cmdbtn(void)
{
	const CUI* pui = CComanderMgr::GetInstance()->GetCmd_info();

	((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

	if(true == m_bsiegemode)
	{
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(6 , L"BTN_SIEGETANK" , BTN_SIEGETANK);
	}
	else
	{
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(6 , L"BTN_SIEGEMODE" , BTN_SIEGEMODE);
	}
}
void CTank::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CComanderMgr::GetInstance()->GetMainInterface_pos();

	if(true == CComanderMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;

		if(false == m_bsiegemode)
			pui = new CUI_Wireframe(L"WIRE_TANK" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		else
			pui = new CUI_Wireframe(L"WIRE_SIEGETANK" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));

		pui->Initialize();
		CComanderMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Tank" ,interface_pos.x + 320 , interface_pos.y + 390 );
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

	if(false == m_bsiegemode)
	{
		CFontMgr::GetInstance()->Setbatch_Font(L"공격력:%d + %d" ,30 , m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt*3,
			interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));
	}
	else
	{
		CFontMgr::GetInstance()->Setbatch_Font(L"공격력:%d + %d" ,70 , m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt*5,
			interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));
	}
	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}
