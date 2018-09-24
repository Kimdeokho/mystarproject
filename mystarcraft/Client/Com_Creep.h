#pragma once
#include "component.h"

enum CREEP_LINE{CREEP_UP , CREEP_RIGHT , CREEP_DOWN, CREEP_LEFT , CREEP_LINEEND};

class CCom_Creep :
	public CComponent
{
private:
	D3DXVECTOR2	m_obj_pos;
	bool		m_bcreep_complete;
	float		m_creeptimer;

	int				m_eline;
	list<int>		m_creepoff_list;
	list<list<int>> m_creep_list[CREEP_LINEEND];
public:
	void Creep_expansion(void);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Creep(bool _complete);
	~CCom_Creep(void);
};
