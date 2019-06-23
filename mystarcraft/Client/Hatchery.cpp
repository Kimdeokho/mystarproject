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
#include "ObjMgr.h"
#include "LineMgr.h"
#include "Session_Mgr.h"

#include "Com_fog.h"
#include "Com_Creep.h"
#include "Com_ZBuildingAnim.h"
#include "Com_Collision.h"
#include "Com_Larvahatch.h"

#include "UI_Wireframe.h"
#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"
#include "UI_Resource.h"
#include "UI_form.h"

#include "Corpse.h"
#include "GeneraEff.h"

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
	m_unitinfo.fog_range = 32*19;

	m_is_rally = false;

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

	m_energybar_ui = new CUI_Energy_bar(this , 146 , m_vertex.bottom*1.7f);
	m_energybar_ui->Initialize();

	m_build_maxhp = m_unitinfo.maxhp;

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetMaxPopvalue(1 , m_eteamnumber);
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

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_HATCHERY , 1 , m_eteamnumber);
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();

	for(int i = UPG_Z_BH3; i <= UPG_Z_BH5; ++i)
		CZerg_building::upginfo_update((UPGRADE)i);
}
void CHatchery::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);

	if(CSession_Mgr::GetInstance()->GetTeamNumber() == m_eteamnumber)
	{
		if(m_isSelect && m_is_rally)
			CLineMgr::GetInstance()->PathLineRender(m_vPos , m_rallypoint , 2.0f);
	}
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


	if('Z' == nkey)
	{
		if( !m_upg_info[UPG_Z_BH3].proceeding &&
			m_upg_info[UPG_Z_BH3].upg_cnt < 1)
		{
			m_upg_info[UPG_Z_BH3].proceeding = true;
			m_upg_info[UPG_Z_BH3].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;

		}
	}
	if('X' == nkey)
	{
		if( !m_upg_info[UPG_Z_BH4].proceeding &&
			m_upg_info[UPG_Z_BH4].upg_cnt < 1)
		{
			m_upg_info[UPG_Z_BH4].proceeding = true;
			m_upg_info[UPG_Z_BH4].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if('P' == nkey)
	{
		if( !m_upg_info[UPG_Z_BH5].proceeding &&
			m_upg_info[UPG_Z_BH5].upg_cnt < 1)
		{
			m_upg_info[UPG_Z_BH5].proceeding = true;
			m_upg_info[UPG_Z_BH5].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
}
void CHatchery::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
}
bool CHatchery::Input_cmd(const int& nkey , bool* waitkey)
{
	if('S' == nkey)
	{
		CUnitMgr::GetInstance()->discharge_unit();
		m_com_larvahatch->Select_Larva();
	}

	if('L' == nkey)
		return true;
	if('H' == nkey)
		return true;

	if('Z' == nkey)
		return true;
	if('X' == nkey)
		return true;
	if('P' == nkey)
		return true;

	return false;
}

bool CHatchery::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
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
		pcmd->Create_Cmdbtn(0 , L"BTN_LARVA" , BTN_LARVA , true);
		pcmd->set_shortkey(0 , L"S");

		if( !m_upg_info[UPG_Z_BH3].proceeding && m_upg_info[UPG_Z_BH3].upg_cnt == 0)
			pcmd->Create_Cmdbtn(3 , L"BTN_Z_BH3" , BTN_Z_BH3 , true , L"Z");
		if( !m_upg_info[UPG_Z_BH4].proceeding && m_upg_info[UPG_Z_BH4].upg_cnt == 0)
			pcmd->Create_Cmdbtn(4 , L"BTN_Z_BH4" , BTN_Z_BH4 , true , L"X");
		if(!m_upg_info[UPG_Z_BH5].proceeding && m_upg_info[UPG_Z_BH5].upg_cnt == 0)
			pcmd->Create_Cmdbtn(5 , L"BTN_Z_BH5" , BTN_Z_BH5 , true , L"P");

		if(OBJ_HATCERY == m_eOBJ_NAME)
		{
			if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_SPWANING_POOL ,m_eteamnumber))
				pcmd->Create_Cmdbtn(6 , L"BTN_LAIR" , BTN_LAIR , true);				
			else
				pcmd->Create_Cmdbtn(6 , L"BTN_LAIR" , BTN_LAIR , false);

			pcmd->set_shortkey(6 , L"L");
		}
		else if(OBJ_LAIR == m_eOBJ_NAME)
		{
			if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(Z_QUEEN_NEST ,m_eteamnumber))
				pcmd->Create_Cmdbtn(6 , L"BTN_HIVE" , BTN_HIVE , true);
			else
				pcmd->Create_Cmdbtn(6 , L"BTN_HIVE" , BTN_HIVE, false);

			pcmd->set_shortkey(6 , L"H");
		}
		else if(OBJ_HIVE == m_eOBJ_NAME)
		{
		}
	}
	else if(DEVELOPING == m_unitinfo.state)
		pcmd->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);

}
void CHatchery::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;	

		if(OBJ_HATCERY == m_eOBJ_NAME)
		{
			pui = new CUI_Wireframe(L"WIRE_HATCHERY" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
			CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Hatchery" ,interface_pos.x + 320 , interface_pos.y + 390 );
		}
		else if(OBJ_LAIR == m_eOBJ_NAME)
		{
			pui = new CUI_Wireframe(L"WIRE_LAIR" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
			CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Lair" ,interface_pos.x + 320 , interface_pos.y + 390 );
		}
		else if(OBJ_HIVE == m_eOBJ_NAME)
		{
			pui = new CUI_Wireframe(L"WIRE_HIVE" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
			CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Hive" ,interface_pos.x + 320 , interface_pos.y + 390 );
		}
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction" , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

			if(true == m_upg_info[UPG_Z_BH3].proceeding && 
				m_upg_info[UPG_Z_BH3].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_Z_BH3" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_Z_BH4].proceeding && 
				m_upg_info[UPG_Z_BH4].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_Z_BH4" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_Z_BH5].proceeding && 
				m_upg_info[UPG_Z_BH5].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_Z_BH5" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
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

	if(true == m_upg_info[UPG_Z_BH3].proceeding && m_upg_info[UPG_Z_BH3].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_Z_BH3].curtime / m_upg_info[UPG_Z_BH3].maxtime );
	else if(true == m_upg_info[UPG_Z_BH4].proceeding && m_upg_info[UPG_Z_BH4].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_Z_BH4].curtime / m_upg_info[UPG_Z_BH4].maxtime );
	else if(true == m_upg_info[UPG_Z_BH5].proceeding && m_upg_info[UPG_Z_BH5].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_Z_BH5].curtime / m_upg_info[UPG_Z_BH5].maxtime );
}

void CHatchery::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"BLOOD_BOOM" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"ZBD_L_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);


	for(int i = 0; i < UPG_END; ++i)
	{
		if( true == m_upg_info[i].proceeding &&
			m_obj_id == m_upg_info[i].obj_num)
		{
			m_upg_info[i].proceeding = false;
			m_upg_info[i].obj_num = 0;
			m_upg_info[i].curtime = 0;
			break;
		}
	}

}
void CHatchery::Release(void)
{
	CZerg_building::area_release();

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetMaxPopvalue(-1 , m_eteamnumber);
}
