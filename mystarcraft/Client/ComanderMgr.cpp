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
#include "TimeMgr.h"

#include "UI_armyunitlist.h"
#include "UI_Boarding_info.h"

#include "UI_Cmd_info.h"

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
	Safe_Delete(m_cmd_info);

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


	memset(m_terran_build , 0 , sizeof(m_terran_build));

	D3DXVECTOR2 interface_pos = m_main_interface->GetPos();

	m_pobj_adress = NULL;
	m_old_state = STATE_NONE;

	m_minimap = new CMinimap;
	m_minimap ->SetPos( D3DXVECTOR2(interface_pos.x + 6 , interface_pos.y + 348) );
	m_minimap->Initialize();

	m_production_bar = new CProduction_bar;
	m_production_bar->Initialize();

	m_building_info = new CUI_Building_info(D3DXVECTOR2(interface_pos.x + 243 , interface_pos.y + 403));
	m_building_info->Initialize();

	m_armyunitlist_info = new CUI_armyunitlist(D3DXVECTOR2(interface_pos.x + 165 , interface_pos.y + 395));
	m_armyunitlist_info->Initialize();
	((CUI_armyunitlist*)m_armyunitlist_info)->set_armylist(CUnitMgr::GetInstance()->Getcur_unitlist());

	m_boarding_info = new CUI_Boarding_info;
	m_boarding_info->SetPos(D3DXVECTOR2(interface_pos.x + 245 , interface_pos.y + 402));
	m_boarding_info->Initialize();

	m_cmd_info = new CUI_Cmd_info;
	m_cmd_info->SetPos( D3DXVECTOR2(interface_pos.x + 510 , interface_pos.y + 360));
	m_cmd_info->Initialize();
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
	m_cmd_info->Update();
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
	
	list<CUI*>::iterator iter = m_wireframe_ui_list.begin();
	list<CUI*>::iterator iter_end = m_wireframe_ui_list.end();

	for( ; iter != iter_end; ++iter)
		(*iter)->Render();

	m_building_info->Render();
	m_armyunitlist_info->Render();
	m_production_bar->Render();
	m_boarding_info->Render();
	m_cmd_info->Render();

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
void CComanderMgr::Update_Wireframe(CObj* pobj)
{
	pobj->Update_Wireframe();
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
	((CUI_Cmd_info*)m_cmd_info)->Click_cmdbtn(vpt);
}

bool CComanderMgr::renewal_wireframe_ui(CObj* pobj , STATE state)
{
	if(m_pobj_adress != pobj ||
		m_old_state != state)
	{		
		m_pobj_adress = pobj;
		m_old_state = state;

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
void CComanderMgr::set_boarding_infolist(multimap<int , BOARDING_INFO , greater<int> >& infolist , OBJID eid)
{
	((CUI_Boarding_info*)m_boarding_info)->set_boarding_infolist(infolist , eid);
}
UPG_INFO* CComanderMgr::GetUpginfo()
{
	return m_upginfo;
}
const CUI* CComanderMgr::GetCmd_info(void)	
{
	return m_cmd_info;
}

const D3DXVECTOR2& CComanderMgr::GetMainInterface_pos(void)
{
	return m_main_interface->GetPos();
}
