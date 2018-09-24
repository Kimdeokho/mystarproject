#pragma once

#include "Include.h"

class CUI;
class CObj;
class CUI_Cmd_info;
class CUI_Building_info;
class CIngame_UIMgr
{
	DECLARE_SINGLETON(CIngame_UIMgr)
private:
	CUI*			m_main_interface;
	CUI*			m_minimap;

	vector<CUI*>	m_vec_preview;	
	list<CUI*>		m_wireframe_ui_list;

	CUI*						m_production_bar;
	CUI_Building_info*			m_building_info;
	CUI*						m_armyunitlist_info;
	CUI*						m_boarding_info;
	CUI*						m_cmd_info;
private:
	int				m_total_mineral;
	int				m_total_gas;
private:
	RACE			m_erace;
	int				m_build_state[BUILD_TECH_END];
	UPG_INFO		m_upginfo[UPG_END];
private:
	
private:
	CObj*		m_pobj_adress;
	STATE		m_old_state;
public:
	void Initialize(RACE erace);
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
	void	SetBuilding_info(list<PRODUCTION_INFO>& ui_list);
	void	SetProduction_info(const D3DXVECTOR2& vpos , const float& ratioX);
	void	set_boarding_infolist(multimap<int , BOARDING_INFO , greater<int> >& infolist , OBJID eid);
public:
	CUI_Cmd_info*		GetCmd_info(void);
	UPG_INFO*			GetUpginfo();
	int					Get_BuildTech(TERRAN_BUILD_TECH etech );
public:
	void	BuildTech_Update(TERRAN_BUILD_TECH etech , const int& cnt);
public:
	void	SetMiniUnit_display(CUI* pui);
	void	SetMinimapCamPos(const D3DXVECTOR2& vmousepos);
	void    SetMineral(const int& amount);
	void    SetGas(const int& amount);
	void	SetPreview(CUI* building_preview);	

	void	Minimappos_to_screen(D3DXVECTOR2& vmousepos);
	bool	intersect_minimap_mousept(const D3DXVECTOR2& vmouse);
	
	void	ClearPreview(void);
public:
	CIngame_UIMgr(void);
	~CIngame_UIMgr(void);
};
