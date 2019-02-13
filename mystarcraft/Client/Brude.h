#pragma once
#include "unit.h"

class CCom_Weapon;
class CBrude :
	public CUnit
{
private:
	CCom_Weapon*	m_com_weapon;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey , bool* waitkey);
	virtual bool Input_cmd(const int& firstkey , const int& secondkey);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);

	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Dead(void);
	virtual void Release(void);
public:
	CBrude(void);
	~CBrude(void);
};
