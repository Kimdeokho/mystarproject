#include "StdAfx.h"
#include "ComanderMgr.h"
#include "Device.h"

#include "TextureMgr.h"

#include "UI_MainInterface.h"
#include "UI_Building_info.h"
#include "Minimap.h"
#include "ScrollMgr.h"
#include "Building_Preview.h"
#include "Production_bar.h"

#include "Obj.h"
#include "Cmd_btn.h"

#include "UnitMgr.h"
#include "FontMgr.h"

#include "UI_armyunitlist.h"
#include "UI_Boarding_info.h"

IMPLEMENT_SINGLETON(CComanderMgr)
CComanderMgr::CComanderMgr(void)
{
	m_total_mineral = 0;
	m_total_gas = 0;
	
}

CComanderMgr::~CComanderMgr(void)
{
	Safe_Delete(m_main_interface);
	Safe_Delete(m_minimap);
	Safe_Delete(m_production_bar);
	Safe_Delete(m_building_info);
	Safe_Delete(m_armyunitlist_info);
	Safe_Delete(m_boarding_info);
	Release_Cmdbtn();

	list<CUI*>::iterator iter = m_wireframe_ui_list.begin();
	list<CUI*>::iterator iter_end = m_wireframe_ui_list.end();

	for( ; iter != iter_end; ++iter) 
	{
		Safe_Delete( *iter );
	}
	m_wireframe_ui_list.clear();

}

void CComanderMgr::Initialize(RACE erace)
{
	m_erace = erace;

	if(TERRAN == m_erace)
		m_main_interface = new CUI_MainInterface(L"terran_interface");

	m_main_interface->Initialize();

	m_minimap = new CMinimap;
	m_minimap->Initialize();

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			m_vcmdbtn_pos[ i * 3 + j ].x = float(608 + j * 46);
			m_vcmdbtn_pos[ i * 3 + j ].y = float(496 + i * 41);
		}
	}

	m_cmdbtn_list.reserve(9);
	m_cmdbtn_list.resize(9);

	memset(m_terran_build , 0 , sizeof(m_terran_build));

	m_pobj_adress = NULL;
	for(int i = 0; i < 9; ++i)
	{
		m_cmdbtn_list[i] = new CCmd_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , false);
	}

	m_temp_cmd_btn = BTN_NONE;
	m_old_state = STATE_NONE;

	m_production_bar = new CProduction_bar;
	m_production_bar->Initialize();

	m_building_info = new CUI_Building_info(D3DXVECTOR2(280,520));
	m_building_info->Initialize();

	m_armyunitlist_info = new CUI_armyunitlist(D3DXVECTOR2(250,520));
	m_armyunitlist_info->Initialize();
	((CUI_armyunitlist*)m_armyunitlist_info)->set_armylist(CUnitMgr::GetInstance()->Getcur_unitlist());

	m_boarding_info = new CUI_Boarding_info;
	m_boarding_info->SetPos(D3DXVECTOR2(320,520));
	m_boarding_info->Initialize();
}

void CComanderMgr::Update(void)
{
	m_minimap->Update();

	//m_preview_building->Update();
	//m_sub_preview_building->Update();
	CUnitMgr::GetInstance()->Update_UI_Infomation();
	m_production_bar->Update();
	m_building_info->Update();
	m_armyunitlist_info->Update();
}

void CComanderMgr::Render(void)
{
	if(!m_vec_preview.empty())
	{
		for(size_t i = 0; i < m_vec_preview.size(); ++i)
		{
			m_vec_preview[i]->Render();
		}
		m_vec_preview.clear();
	}

	m_main_interface->Render();


	for(int i = 0; i < 9; ++i) 
	{
		if(NULL == m_cmdbtn_list[i])
			continue;

		m_cmdbtn_list[i]->Render();
	}

	list<CUI*>::iterator iter = m_wireframe_ui_list.begin();
	list<CUI*>::iterator iter_end = m_wireframe_ui_list.end();

	for( ; iter != iter_end; ++iter)
		(*iter)->Render();

	m_building_info->Render();
	m_armyunitlist_info->Render();
	m_production_bar->Render();
	m_boarding_info->Render();

	m_minimap->Render();
}

void CComanderMgr::SetMiniUnit_display(CUI* pui)
{
	((CMinimap*)m_minimap)->Setminiunit(pui);
}


void CComanderMgr::SetMinimapCamPos(const D3DXVECTOR2& vmousepos)
{
	((CMinimap*)m_minimap)->SetMinimapCamPos(vmousepos);
}
void CComanderMgr::Minimappos_to_screen(D3DXVECTOR2& vmousepos)
{
	((CMinimap*)m_minimap)->Minimappos_to_screen(vmousepos);
}
bool	CComanderMgr::intersect_minimap_mousept(const D3DXVECTOR2& vmouse)
{

	if( MyPtInrect(vmouse , &(m_minimap->GetMyRect())) )
		return true;

	return false;
}
void CComanderMgr::SetMineral(const int& amount)
{
	m_total_mineral += amount;
}

void CComanderMgr::SetGas(const int& amount)
{
	m_total_gas += amount;
}

void CComanderMgr::SetPreview(CUI* building_preview)
{
	m_vec_preview.push_back(building_preview);
}
void CComanderMgr::ClearPreview(void)
{
	m_vec_preview.clear();
}
void CComanderMgr::Update_Cmdbtn(CObj* pobj)
{
	//pobj 동적 바인딩으로 변경가능
	CUI* pui = NULL;

	if(NULL == pobj)
	{
		for(int i = 5; i < 9; ++i)
		{
			pui = m_cmdbtn_list[i];
			((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
		}

		//m_pobj_adress = NULL;

		//서로다른 유닛 여러마리
		Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
		Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
		Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
		Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
		Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);
	}
	else
	{
		//같은종류

		for(int i = 0; i < 9; ++i)
		{
			pui = m_cmdbtn_list[i];
			((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
		}

		pobj->Update_Cmdbtn();
	}

}
void CComanderMgr::Update_Wireframe(CObj* pobj)
{
	pobj->Update_Wireframe();
}
void CComanderMgr::Create_Cmdbtn(const int& idx ,const TCHAR* texkey, CMD_BTN ebtn , bool is_active)
{
	CUI* pui = NULL;
	if(NULL != m_cmdbtn_list[idx])
	{
		pui = m_cmdbtn_list[idx];
		if(ebtn == ((CCmd_btn*)pui)->GetCmdbtn_id())
			return;
		else
		{
			((CCmd_btn*)pui)->Init_btn(texkey , ebtn , m_vcmdbtn_pos[idx] , is_active);
		}
	}
}
void CComanderMgr::T_Cmdbtn_B_buildsetting(void)
{
	//단축키 b를 누르면 나오는 종족 테크트리

	CUI* pui = NULL;
	for(int i = 0; i < 9; ++i)
	{
		pui = m_cmdbtn_list[i];
		((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
	}

	Create_Cmdbtn(0 , L"BTN_COMMAND_CENTER" , BTN_COMAND_CENTER);
	Create_Cmdbtn(1 , L"BTN_SUPPLY" , BTN_SUPPLY );
	Create_Cmdbtn(2 , L"BTN_T_GAS" , BTN_T_GAS );

	if(0 < m_terran_build[T_COMMANDCENTER])
	{
		Create_Cmdbtn(3 , L"BTN_BARRACK" , BTN_BARRACK , true);
		Create_Cmdbtn(4 , L"BTN_EB" , BTN_EB , true);		
	}
	else
	{
		Create_Cmdbtn(3 , L"BTN_BARRACK" , BTN_BARRACK , false);
		Create_Cmdbtn(4 , L"BTN_EB" , BTN_EB , false);		
	}

	if( 0 < m_terran_build[T_BARRACK])
	{
		Create_Cmdbtn(6 , L"BTN_ACADEMY" , BTN_ACADEMY);
		Create_Cmdbtn(7 , L"BTN_BUNKER" , BTN_BUNKER);
	}
	else
	{
		Create_Cmdbtn(6 , L"BTN_ACADEMY" , BTN_ACADEMY , false);
		Create_Cmdbtn(7 , L"BTN_BUNKER" , BTN_BUNKER , false);
	}

	if( 0 < m_terran_build[T_EB])
	{
		Create_Cmdbtn(5 , L"BTN_TURRET" , BTN_TURRET);
	}
	else
		Create_Cmdbtn(5 , L"BTN_TURRET" , BTN_TURRET , false);

}
void CComanderMgr::T_Cmdbtn_V_buildsetting(void)
{
	//단축키 b를 누르면 나오는 종족 테크트리

	CUI* pui = NULL;
	for(int i = 0; i < 9; ++i)
	{
		pui = m_cmdbtn_list[i];
		((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
	}

	if( 0 < m_terran_build[T_BARRACK])
	{
		Create_Cmdbtn(0 , L"BTN_FACTORY" , BTN_FACTORY);
	}
	else
	{
		Create_Cmdbtn(0 , L"BTN_FACTORY" , BTN_FACTORY , false);
	}

	if( 0 < m_terran_build[T_FACTORY])
	{
		Create_Cmdbtn(1 , L"BTN_STARPORT" , BTN_STARPORT);
		Create_Cmdbtn(3 , L"BTN_ARMORY" , BTN_ARMORY);
	}
	else
	{
		Create_Cmdbtn(1 , L"BTN_STARPORT" , BTN_STARPORT , false);
		Create_Cmdbtn(3 , L"BTN_ARMORY" , BTN_ARMORY , false);
	}

	if( 0 < m_terran_build[T_STARPORT])
	{
		Create_Cmdbtn(2 , L"BTN_SIENCE" , BTN_SIENCE);
	}
	else
	{
		Create_Cmdbtn(2 , L"BTN_SIENCE" , BTN_SIENCE , false);
	}

}
bool CComanderMgr::Active_Cmdbtn(const int& idx, CMD_BTN ebtn)
{
	if(NULL == m_cmdbtn_list[idx])
		return false;
	else
	{
		CUI* pui = m_cmdbtn_list[idx];
		if( ebtn == ((CCmd_btn*)pui)->GetCmdbtn_id() )
		{
			return true;
		}
	}
	return false;
}
void CComanderMgr::Release_Cmdbtn(void)
{
	if(!m_cmdbtn_list.empty())
	{
		for(int i = 0; i < 9; ++i)
		{
			Safe_Delete( m_cmdbtn_list[i] );
		}
		m_cmdbtn_list.clear();
	}
}

void CComanderMgr::T_BuildTech_Update(TERRAN_BUILD_TECH etech , const int& cnt)
{
	m_terran_build[etech] += cnt;
}
int CComanderMgr::Get_T_BuildTech(TERRAN_BUILD_TECH etech )
{
	return m_terran_build[etech];
}
void CComanderMgr::Click_cmdbtn(const D3DXVECTOR2& vpt)
{
	for(size_t i = 0; i < m_cmdbtn_list.size(); ++i)
	{
		if( MyPtInrect( vpt ,  &(m_cmdbtn_list[i]->GetMyRect()) ) )
		{
			m_temp_cmd_btn = ((CCmd_btn*)m_cmdbtn_list[i])->GetCmdbtn_id();

			if(BTN_NONE == m_temp_cmd_btn)
				continue;
			else
				return;
		}
	}

	m_temp_cmd_btn = BTN_NONE;
}
CMD_BTN CComanderMgr::Get_Cmd_btn(void)
{
	return m_temp_cmd_btn;
}

bool CComanderMgr::clear_cmdbtn(void)
{
	if(!m_cmdbtn_list.empty())
	{
		CUI* pui = NULL;
		for(int i = 0; i < 9; ++i)
		{
			pui = m_cmdbtn_list[i];
			if(BTN_NONE != ((CCmd_btn*)pui)->GetCmdbtn_id())
				((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
		}
	}

	return true;
}
bool CComanderMgr::renewal_wireframe_ui(CObj* pobj , STATE estate)
{
	if(m_pobj_adress != pobj ||
		m_old_state != estate)
	{		
		m_pobj_adress = pobj;
		m_old_state = estate;

		if( !m_wireframe_ui_list.empty())
		{
			list<CUI*>::iterator iter = m_wireframe_ui_list.begin();
			list<CUI*>::iterator iter_end = m_wireframe_ui_list.end();

			for( ; iter != iter_end; ++iter) 
				Safe_Delete( *iter );

			m_wireframe_ui_list.clear();
		}

		CFontMgr::GetInstance()->renewal_infomation_font();
		((CProduction_bar*)m_production_bar)->SetActive(false);
		((CUI_Building_info*)m_building_info)->SetActive(false);
		((CUI_Boarding_info*)m_boarding_info)->SetActive(false);

		return true;
	}
	else
	{
		return false;
	}
}
void CComanderMgr::add_wireframe_ui(CUI* pui)
{
	m_wireframe_ui_list.push_back(pui);
}
void CComanderMgr::SetProduction_info(const D3DXVECTOR2& vpos , const float& ratioX)
{
	((CProduction_bar*)m_production_bar)->SetProduction_info(vpos , ratioX);
}
void CComanderMgr::SetBuilding_info(list<PRODUCTION_INFO>& ui_list)
{
	((CUI_Building_info*)m_building_info)->SetActive(true);
	((CUI_Building_info*)m_building_info)->InitNumber();

	int idx = 0;
	list<PRODUCTION_INFO>::iterator iter = ui_list.begin();
	list<PRODUCTION_INFO>::iterator iter_end = ui_list.end();

	for( ; iter != iter_end; ++iter)
	{
		((CUI_Building_info*)m_building_info)->SetBuilding_info(idx , (*iter).texkey);
		++idx;
	}
}
void CComanderMgr::set_boarding_infolist(multimap<int , BOARDING_INFO , greater<int> >& infolist)
{
	((CUI_Boarding_info*)m_boarding_info)->set_boarding_infolist(infolist);
}
const UPG_INFO& CComanderMgr::GetUpginfo(UPGRADE eupg)
{
	return m_upginfo[eupg];
}