#pragma once
#include "terran_building.h"

class CFactory :
	public CTerran_building
{
private:
	CComponent*		m_com_production;
public:
	void Update_Wireframe(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey, bool* waitkey);
	virtual bool Input_cmd(const int& firstkey , const int& secondkey);
	virtual void Update_Cmdbtn(void);
public:
	CFactory(void);
	~CFactory(void);
};
