#pragma once
#include "ui.h"

class CUI_Boarding_info :
	public CUI
{
private:
	D3DXVECTOR2	m_uiform_pos[2][4];
	bool		m_occupy[2][4];

	CUI*		m_backform[2][4];
	CUI*		m_wireform[2][4];

	bool		m_is_active;
public:
	void SetActive(bool is_active);
	void set_boarding_infolist(multimap< int ,BOARDING_INFO , greater<int>>& infolist , OBJID eid);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Boarding_info(void);
	~CUI_Boarding_info(void);
};
