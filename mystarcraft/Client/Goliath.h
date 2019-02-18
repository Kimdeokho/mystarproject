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
	virtual bool Input_cmd(const int& nkey , bool* waitkey);

	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
	virtual void Dead(void);
public:
	CGoliath(void);
	~CGoliath(void);
};
