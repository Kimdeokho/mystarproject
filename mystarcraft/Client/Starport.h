#pragma once
#include "terran_building.h"

class CStarport :
	public CTerran_building
{
private:
	CComponent*		m_com_production;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Input_cmd(const int& nkey, bool* waitkey);
	virtual void Input_cmd(const int& firstkey , const int& secondkey);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CStarport(void);
	~CStarport(void);
};
