#pragma once
#include "unit.h"

class CDropship :
	public CUnit
{
private:
	CComponent*		m_com_transport;
public:
	bool setunit(CObj* pobj);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);

	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
public:
public:
	CDropship(void);
	~CDropship(void);
};
