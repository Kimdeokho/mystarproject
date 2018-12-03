#pragma once
#include "unit.h"

class CLarva :
	public CUnit
{
private:
	D3DXVECTOR2		m_waypoint[4];
	DWORD			m_wayidx;
	
	float			m_idletime;
	DWORD			m_standard;
	
	bool			m_moveswitch;

	int				m_hatch_num;
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

	virtual void Dead(void);
	virtual void Release(void);
public:
	CLarva(int hatch_number);
	~CLarva(void);
};
