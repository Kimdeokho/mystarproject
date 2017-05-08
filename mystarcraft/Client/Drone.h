#pragma once
#include "unit.h"

class CDrone :
	public CUnit
{
private:
	const vector<TEXINFO*>*					m_moveTex[DIR_CNT];/*[]�� ����(����)�� ���Ѵ�*/
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Release(void);
public:
	CDrone(void);
	virtual ~CDrone(void);
};
