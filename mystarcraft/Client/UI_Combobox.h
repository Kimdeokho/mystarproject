#pragma once
#include "ui.h"

class CUI_Combobox :
	public CUI
{
private:
	TCHAR	m_represent[32];
	vector<TCHAR*>	m_combolist; //0번 인덱스가 선택된 박스대표

	const TEXINFO*			m_texcombo_slot;
	const TEXINFO*			m_texcombo_middle;
	const TEXINFO*			m_texcombo_end;
	const TEXINFO*			m_texcombo_arrow;

	bool					m_active_combolist;
	int						m_cursel;

	UINT					m_texwidth;
	UINT					m_texheight;

	int						m_slotidx;
public:
	void	AddData(const TCHAR* _data);
	void			SetRepresentData(const WCHAR* _data);
	const WCHAR*	GetRepresent_data(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Init_State(void);
	virtual void Release(void);
public:
	CUI_Combobox(void);
	CUI_Combobox(const int& idx);
	~CUI_Combobox(void);
};
