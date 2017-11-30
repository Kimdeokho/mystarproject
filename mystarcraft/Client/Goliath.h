#pragma once
#include "unit.h"

class CGoliath :
	public CUnit
{
private:
	CObj*	m_pgoliath_arm;
public:
	void sync_arm(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);

	virtual void Dead(void);
public:
	CGoliath(void);
	~CGoliath(void);
};
