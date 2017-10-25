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
	//CUI*		m_preview_building;
	//CUI*		m_sub_preview_building;
	vector<CUI*>	m_vec_preview;
private:
	int			m_total_mineral;
	int			m_total_gas;
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
public:
	void	SetMiniUnit_display(CUI* pui);
	void	SetMinimapCamPos(const D3DXVECTOR2& vmousepos);
	void    SetMineral(const int& amount);
	void    SetGas(const int& amount);



	void	Minimappos_to_screen(D3DXVECTOR2& vmousepos);
	bool	intersect_minimap_mousept(const D3DXVECTOR2& vmouse);

	void	SetPreview(CUI* building_preview);
public:
	CComanderMgr(void);
	~CComanderMgr(void);
};
