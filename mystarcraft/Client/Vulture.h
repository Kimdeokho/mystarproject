#pragma once
#include "unit.h"

class CVulture :
	public CUnit
{
private:
	CComponent*	m_com_weapon;
	bool		m_is_landmine;
	int			m_landmine_cnt;

	D3DXVECTOR2		m_install_pos;
	bool			m_is_mine_install;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey , bool* waitkey);


	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);

	virtual void Release(void);
	virtual void Dead(void);
public:
	CVulture(void);
	~CVulture(void);
};
