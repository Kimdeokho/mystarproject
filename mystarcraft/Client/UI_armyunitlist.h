#pragma once
#include "ui.h"

class CObj;
class CUI_armyunitlist :
	public CUI
{
private:
	list<CObj*>*	m_armyunit_list;
	CUI*			m_backform[2][6];
	CUI*			m_unitform[2][6];
public:
	void set_armylist(list<CObj*>* armylist);
	void set_wireframe(OBJID eid , const int& icol , const int& irow);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_armyunitlist(const D3DXVECTOR2& vpos);
	~CUI_armyunitlist(void);
};
