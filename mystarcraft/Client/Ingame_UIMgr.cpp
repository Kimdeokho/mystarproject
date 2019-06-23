#include "StdAfx.h"
#include "Ingame_UIMgr.h"
#include "Device.h"
#include "Obj.h"

#include "TextureMgr.h"
#include "Session_Mgr.h"

#include "UI_MainInterface.h"
#include "UI_Building_info.h"
#include "Minimap.h"
#include "ScrollMgr.h"
#include "Building_Preview.h"
#include "Production_bar.h"


#include "Cmd_btn.h"

#include "UnitMgr.h"
#include "FontMgr.h"
#include "TimeMgr.h"

#include "UI_armyunitlist.h"
#include "UI_Boarding_info.h"
#include "UI_Resource.h"
#include "UI_IME.h"
#include "UI_Chat.h"
#include "UI_Cmd_info.h"
#include "UI_Notice.h"

IMPLEMENT_SINGLETON(CIngame_UIMgr)
CIngame_UIMgr::CIngame_UIMgr(void)
{	
	m_chat_ime = NULL;
}

CIngame_UIMgr::~CIngame_UIMgr(void)
{
	Safe_Delete(m_main_interface);
	Safe_Delete(m_minimap);
	Safe_Delete(m_production_bar);
	Safe_Delete(m_building_info);
	Safe_Delete(m_armyunitlist_info);
	Safe_Delete(m_boarding_info);
	Safe_Delete(m_cmd_info);
	Safe_Delete(m_resource_info);
	Safe_Delete(m_chat_ime);
	Safe_Delete(m_chatlist);
	Safe_Delete(m_notice);

	list<CUI*>::iterator iter = m_wireframe_ui_list.begin();
	list<CUI*>::iterator iter_end = m_wireframe_ui_list.end();

	for( ; iter != iter_end; ++iter) 
	{
		Safe_Delete( *iter );
	}
	m_wireframe_ui_list.clear();

}
void CIngame_UIMgr::Initialize(const TRIBE erace)
{
	m_erace = erace;

	if(TRIBE_TERRAN == m_erace)
		m_main_interface = new CUI_MainInterface(L"terran_interface");
	else if(TRIBE_ZERG == m_erace)
		m_main_interface = new CUI_MainInterface(L"zerg_interface");
	m_main_interface->Initialize();

	D3DXVECTOR2 interface_pos = m_main_interface->GetPos();

	memset(m_build_state , 0 , sizeof(m_build_state));	

	m_is_chat = false;
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
	m_armyunitlist_info->set_armylist(CUnitMgr::GetInstance()->Getcur_unitlist());

	m_boarding_info = new CUI_Boarding_info;
	m_boarding_info->SetPos(D3DXVECTOR2(interface_pos.x + 245 , interface_pos.y + 402));
	m_boarding_info->Initialize();

	m_cmd_info = new CUI_Cmd_info;
	if(TRIBE_TERRAN == m_erace)
		m_cmd_info->SetPos( D3DXVECTOR2(interface_pos.x + 510 , interface_pos.y + 360));
	else if(TRIBE_ZERG == m_erace)
		m_cmd_info->SetPos( D3DXVECTOR2(interface_pos.x + 500 , interface_pos.y + 360));
	m_cmd_info->Initialize();

	m_resource_info = new CUI_Resource; //자원상태 나타내는 UI
	m_resource_info->Initialize();

	m_chat_ime = new CUI_IME(D3DXVECTOR2(200 , 350) , D3DXVECTOR2(200 , 25));
	m_chat_ime->Initialize();

	m_chatlist = new CUI_Chat;
	m_chatlist->SetPos(D3DXVECTOR2(15,270));
	m_chatlist->SetChatSize(13);
	m_chatlist->Initialize();

	m_notice = new CUI_Notice;
	m_notice->Initialize();
}

void CIngame_UIMgr::Update(void)
{
	m_minimap->Update();

	CUnitMgr::GetInstance()->Update_UI_Infomation();
	m_production_bar->Update();
	m_building_info->Update();
	m_armyunitlist_info->Update();
	m_cmd_info->Update();
	m_resource_info->Update();
	m_chat_ime->Update();
	m_chatlist->Update();
	m_notice->Update();
}

void CIngame_UIMgr::Render(void)
{
	if(!m_vec_preview.empty())
	{
		for(size_t i = 0; i < m_vec_preview.size(); ++i)
			m_vec_preview[i]->Render();

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
	m_resource_info->Render();

	m_minimap->Render();
	m_chat_ime->Render();
	m_notice->Render();
}

void CIngame_UIMgr::SetMiniUnit_display(CUI* pui)
{
	m_minimap->Setminiunit(pui);
}
void CIngame_UIMgr::SetMinimapCamPos(const D3DXVECTOR2& vmousepos)
{
	m_minimap->SetMinimapCamPos(vmousepos);
}
void CIngame_UIMgr::Minimappos_to_screen(D3DXVECTOR2& vmousepos)
{
	m_minimap->Minimappos_to_screen(vmousepos);
}
bool	CIngame_UIMgr::intersect_minimap_mousept(const D3DXVECTOR2& vmouse)
{

	if( MyPtInrect(vmouse , &(m_minimap->GetMyRect())) )
		return true;

	return false;
}

void CIngame_UIMgr::SetPreview(CUI* building_preview)
{
	m_vec_preview.push_back(building_preview);
}
void CIngame_UIMgr::ClearPreview(void)
{
	m_vec_preview.clear();
}
void CIngame_UIMgr::Update_Wireframe(CObj* pobj)
{
	pobj->Update_Wireframe();
}

void CIngame_UIMgr::BuildTech_Update(TERRAN_BUILD_TECH etech , const int cnt , const TEAM_NUMBER teamnum)
{
	m_build_state[teamnum][etech] += cnt;
}

void CIngame_UIMgr::Click_cmdbtn(const D3DXVECTOR2& vpt)
{
	m_cmd_info->Click_cmdbtn(vpt);
}

bool CIngame_UIMgr::renewal_wireframe_ui(CObj* pobj , STATE state)
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
		m_production_bar->SetActive(false);
		m_building_info->SetActive(false);
		m_boarding_info->SetActive(false);

		return true;
	}
	else
	{
		return false;
	}
}
void CIngame_UIMgr::add_wireframe_ui(CUI* pui)
{
	m_wireframe_ui_list.push_back(pui);
}
void CIngame_UIMgr::SetProduction_info(const D3DXVECTOR2& vpos , const float& ratioX)
{
	m_production_bar->SetProduction_info(vpos , ratioX);
}
void CIngame_UIMgr::SetBuilding_info(list<PRODUCTION_INFO>& ui_list)
{
	m_building_info->SetActive(true);
	m_building_info->InitNumber();

	int idx = 0;
	list<PRODUCTION_INFO>::iterator iter = ui_list.begin();
	list<PRODUCTION_INFO>::iterator iter_end = ui_list.end();

	for( ; iter != iter_end; ++iter)
	{
		m_building_info->SetBuilding_info(idx , (*iter).texkey);
		++idx;
	}
}
void CIngame_UIMgr::set_boarding_infolist(multimap<int , BOARDING_INFO , greater<int> >& infolist , OBJID eid)
{
	m_boarding_info->set_boarding_infolist(infolist , eid);
}
UPG_INFO* CIngame_UIMgr::GetUpginfo()
{
	return m_upginfo;
}
CUI_Cmd_info* CIngame_UIMgr::GetCmd_info(void)	
{
	return m_cmd_info;
}
CUI_Resource* CIngame_UIMgr::GetResource_UI(void)
{
	return m_resource_info;
}

CUI_Notice* CIngame_UIMgr::GetUI_Notice(void)
{
	return m_notice;
}
const D3DXVECTOR2& CIngame_UIMgr::GetMainInterface_pos(void)
{
	return m_main_interface->GetPos();
}
int CIngame_UIMgr::Get_BuildTech(const TERRAN_BUILD_TECH etech , const TEAM_NUMBER teamnum )
{
	return m_build_state[teamnum][etech];
}
CUI_Chat* CIngame_UIMgr::GetUI_Chat(void)
{
	return m_chatlist;
}

void CIngame_UIMgr::ClearWireFrame(void)
{
	if( !m_wireframe_ui_list.empty())
	{
		list<CUI*>::iterator iter = m_wireframe_ui_list.begin();
		list<CUI*>::iterator iter_end = m_wireframe_ui_list.end();

		for( ; iter != iter_end; ++iter) 
			Safe_Delete( *iter );

		m_wireframe_ui_list.clear();
	}
}

bool CIngame_UIMgr::ToggleChat(S_PT_ROOM_RECEIVE_CHAT_M& temp_data)
{
	if(m_is_chat)
	{
		if(GetFocus() == m_chat_ime->GetHandle())
		{
			m_is_chat = false;
			m_chat_ime->Init_State();

			WCHAR szmessage[32] = L"";
			wcscpy_s(szmessage , m_chat_ime->GetMessage());


			if( 0 != lstrlen(szmessage))
			{
				SESSION_INFO psession_info = CSession_Mgr::GetInstance()->GetSession_Info();

				wcscpy_s(temp_data.MESSAGE , szmessage);
				wcscpy_s(temp_data.USER_ID , psession_info.USER_ID);
				temp_data.RETAIN_TIME = 7.f;
				temp_data.CUR_TIME = 0.f;
				temp_data.SESSION_ID = psession_info.SESSION_ID;				

				m_chat_ime->TextClear();			
				return true;
			}
			else
				return false;
		}
		else
			return false;

	}
	else
	{		
		m_chat_ime->TextClear();
		m_chat_ime->UI_Reaction();
		m_is_chat = true;
		return false;
	}
}
bool CIngame_UIMgr::is_active_chat_ime(void)
{
	if(NULL == m_chat_ime)
		return false;

	if(GetFocus() == m_chat_ime->GetHandle())
		return true;
	else
		return false;
}

void CIngame_UIMgr::UI_Reaction(const D3DXVECTOR2 vpt)
{
	if(m_notice->UI_ptinrect(vpt))
		m_notice->UI_Reaction();
}
