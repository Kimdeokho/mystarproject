#pragma once
#include "terran_building.h"

class CNuclear_part :
	public CTerran_building
{
private:
	
	vector<PRODUCTION_INFO>	m_nuclear;
	int						m_nuclear_cnt;
public:
	int	 GetNuclear_cnt(void);
	void SetNuclear_cnt(const int& cnt);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey, bool* waitkey);
	virtual void Setlink(bool blink , CObj* pobj);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CNuclear_part(CObj* pobj);
	~CNuclear_part(void);
};
