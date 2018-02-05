#pragma once

#include "Include.h"

class CUI;
class CObj;
class CComanderMgr
{
	DECLARE_SINGLETON(CComanderMgr)
private:
	CUI*		m_main_interface;
	CUI*		m_minimap;

	vector<CUI*>	m_vec_preview;
	vector<CUI*>	m_cmdbtn_list;
	list<CUI*>		m_wireframe_ui_list;
	CUI*			m_production_bar;
	CUI*			m_building_info;
	CUI*			m_armyunitlist_info;
	CUI*			m_boarding_info;
private:
	int			m_total_mineral;
	int			m_total_gas;
private:
	CMD_BTN		m_temp_cmd_btn;
	RACE		m_erace;
	int			m_terran_build[T_BUILD_END];
	UPG_INFO	m_upginfo[UPG_END];
private:
	D3DXVECTOR2	m_vcmdbtn_pos[9];
private:
	CObj*		m_pobj_adress;
	STATE		m_old_state;
public:
	void Initialize(RACE erace);
	void Update(void);
	void Render(void);
public:
	void	Update_Cmdbtn(CObj* pobj);

	void	Create_Cmdbtn(const int& idx ,const TCHAR* texkey, CMD_BTN ebtn , bool is_active = true);
	bool	Active_Cmdbtn(const int& idx, CMD_BTN ebtn);

	void	T_Cmdbtn_B_buildsetting(void);
	void	T_Cmdbtn_V_buildsetting(void);
	void	Click_cmdbtn(const D3DXVECTOR2& vpt);
	CMD_BTN	Get_Cmd_btn(void);
	bool	clear_cmdbtn(void);
	void	Release_Cmdbtn(void);
public:
	void	Update_Wireframe(CObj* pobj);
	bool	renewal_wireframe_ui(CObj* pobj , STATE estate);
	void	add_wireframe_ui(CUI* pui);
public:
	void	SetBuilding_info(list<PRODUCTION_INFO>& ui_list);
	void	SetProduction_info(const D3DXVECTOR2& vpos , const float& ratioX);
	void	set_boarding_infolist(multimap<int , BOARDING_INFO , greater<int> >& infolist);
public:
	const UPG_INFO& GetUpginfo(UPGRADE eupg);
public:
	
public:
	void	T_BuildTech_Update(TERRAN_BUILD_TECH etech , const int& cnt);
public:
	void	SetMiniUnit_display(CUI* pui);
	void	SetMinimapCamPos(const D3DXVECTOR2& vmousepos);
	void    SetMineral(const int& amount);
	void    SetGas(const int& amount);
	void	SetPreview(CUI* building_preview);

	int		Get_T_BuildTech(TERRAN_BUILD_TECH etech );

	void	Minimappos_to_screen(D3DXVECTOR2& vmousepos);
	bool	intersect_minimap_mousept(const D3DXVECTOR2& vmouse);
	
	void	ClearPreview(void);	
public:
	CComanderMgr(void);
	~CComanderMgr(void);
};
