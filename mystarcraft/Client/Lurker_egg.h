#pragma once
#include "unit.h"

class CLurker_egg :
	public CUnit
{
private:
	float				m_fbuild_tick;
	float				m_build_hp;
	int					m_build_maxhp;
	int					m_create_cnt;

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
	CLurker_egg(void);
	~CLurker_egg(void);
};
