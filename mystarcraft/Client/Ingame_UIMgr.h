#pragma once

#include "Include.h"

class CUI;
class CObj;
class CUI_Cmd_info;
class CUI_Building_info;
class CUI_Resource;
class CUI_armyunitlist;
class CUI_Resource;
class CUI_Boarding_info;
class CProduction_bar;
class CUI_IME;
class CUI_Chat;
class CMinimap;
class CUI_Notice;

class CIngame_UIMgr
{
	DECLARE_SINGLETON(CIngame_UIMgr)
private:
	CUI*			m_main_interface;
	

	vector<CUI*>	m_vec_preview;	
	list<CUI*>		m_wireframe_ui_list;

	CMinimap*					m_minimap;
	CProduction_bar*			m_production_bar;
	CUI_Building_info*			m_building_info;
	CUI_armyunitlist*			m_armyunitlist_info;
	CUI_Boarding_info*			m_boarding_info;
	CUI_Cmd_info*				m_cmd_info;
	CUI_Resource*				m_resource_info;
	CUI_IME*					m_chat_ime;
	CUI_Chat*				    m_chatlist;
	CUI_Notice*					m_notice;
private:
	TRIBE			m_erace;
	int				m_build_state[TEAM_END][BUILD_TECH_END];
	UPG_INFO		m_upginfo[UPG_END];
private:
	
private:
	CObj*		m_pobj_adress;
	STATE		m_old_state;
	bool		m_is_chat;
public:
	void Initialize(const TRIBE erace);
	void Update(void);
	void Render(void);
public:
	const D3DXVECTOR2& GetMainInterface_pos(void);
public:
	void	T_Cmdbtn_B_buildsetting(void);
	void	T_Cmdbtn_V_buildsetting(void);
	void	Click_cmdbtn(const D3DXVECTOR2& vpt);
public:
	void	Update_Wireframe(CObj* pobj);
	bool	renewal_wireframe_ui(CObj* pobj , STATE state);
	void	add_wireframe_ui(CUI* pui);
public:
	CUI_Cmd_info*		GetCmd_info(void);
	UPG_INFO*			GetUpginfo();
	CUI_Resource*		GetResource_UI(void);
	CUI_Chat*			GetUI_Chat(void);
	CUI_Notice*			GetUI_Notice(void);
	int					Get_BuildTech(TERRAN_BUILD_TECH etech , const TEAM_NUMBER teamnum);
public:
	void	BuildTech_Update(TERRAN_BUILD_TECH etech , const int cnt , const TEAM_NUMBER teamnum);
	void	Minimappos_to_screen(D3DXVECTOR2& vmousepos);
	bool	intersect_minimap_mousept(const D3DXVECTOR2& vmouse);
	bool	is_active_chat_ime(void);

	void	ClearWireFrame(void);
	void	ClearPreview(void);
	bool	ToggleChat(S_PT_ROOM_RECEIVE_CHAT_M& temp_data);
	void	UI_Reaction(const D3DXVECTOR2 vpt);
public:
	void	SetMiniUnit_display(CUI* pui);
	void	SetMinimapCamPos(const D3DXVECTOR2& vmousepos);
	void    SetMineral(const int& amount);
	void    SetGas(const int& amount);
	void	SetPreview(CUI* building_preview);	
	void	SetBuilding_info(list<PRODUCTION_INFO>& ui_list);
	void	SetProduction_info(const D3DXVECTOR2& vpos , const float& ratioX);
	void	set_boarding_infolist(multimap<int , BOARDING_INFO , greater<int> >& infolist , OBJID eid);

public:
	CIngame_UIMgr(void);
	~CIngame_UIMgr(void);
};
