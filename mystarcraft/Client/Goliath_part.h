#pragma once
#include "unit.h"

class CGoliath_part :
	public CUnit
{
private:
	CComponent* m_com_weapon;

	MOVE_TYPE	m_etarget_movetype;

	CObj*		m_goliath_leg;

public:
	void setarm_pos(const D3DXVECTOR2& vpos);
	void settarget_movetype(MOVE_TYPE emovetype);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);

	virtual void Dead(void);
public:
	CGoliath_part(CObj*	goliath_leg);
	~CGoliath_part(void);
};
