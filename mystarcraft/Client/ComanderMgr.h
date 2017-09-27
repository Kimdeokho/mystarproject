#pragma once

#include "Include.h"

class CUI;
class CComanderMgr
{
	DECLARE_SINGLETON(CComanderMgr)
private:
	CUI*		m_main_interface;
	CUI*		m_minimap;
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
public:
	void SetMiniUnit_display(CUI* pui);
	void SetMinimapCamPos(const D3DXVECTOR2& vmousepos);
public:
	CComanderMgr(void);
	~CComanderMgr(void);
};
