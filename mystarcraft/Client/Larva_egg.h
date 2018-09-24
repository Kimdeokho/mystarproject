#pragma once
#include "unit.h"

class CLarva_egg :
	public CUnit
{
private:
	PRODUCTION_INFO		m_production_info;
	float				m_fbuild_tick;
	float				m_build_hp;
	int					m_build_maxhp;
	int					m_hatch_num;
	int					m_create_cnt;

	const TCHAR*		m_birthkey;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Input_cmd(const int& nkey , bool* waitkey);
	virtual void Input_cmd(const int& firstkey , const int& secondkey);

	virtual void Update_Cmdbtn(void);

	virtual void Dead(void);
	virtual void Release(void);
public:
	CLarva_egg(const float maxtime ,const OBJID id, const int hatch_num , const int createcnt , const TCHAR* birthkey);
	~CLarva_egg(void);
};
