#pragma once
#include "unit.h"

class CVulture :
	public CUnit
{
private:
	bool		m_is_landmine;
	int			m_landmine_cnt;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);

	virtual void Release(void);
	virtual void Dead(void);
public:
	CVulture(void);
	~CVulture(void);
};
