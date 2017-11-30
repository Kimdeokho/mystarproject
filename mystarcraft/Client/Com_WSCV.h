#pragma once
#include "com_weapon.h"

class CCom_WSCV :
	public CCom_Weapon
{
private:
	bool	m_bmineral_gather;
	float	m_gathertime;
	float	m_gatherdelay;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	virtual void fire(CObj*& ptarget);
public:
	CCom_WSCV();
	~CCom_WSCV(void);
};
