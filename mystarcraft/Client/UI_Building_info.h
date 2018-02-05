#pragma once
#include "ui.h"

//»ý»êÃ¢
class CUI_Building_info :
	public CUI
{
private:
	bool		m_is_active;

	CUI*		m_backform[5];
	CUI*		m_btn_form[5];

	D3DXVECTOR2	m_vnumberfont_pos[5];
	D3DXVECTOR2	m_vbtn_pos[5];
public:
	void	SetActive(bool bactive);
	void	SetBuilding_info(const int& idx , const TCHAR* texkey);
	void	InitNumber(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Building_info(const D3DXVECTOR2& vpos);
	~CUI_Building_info(void);
};
