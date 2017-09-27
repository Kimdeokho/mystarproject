#pragma once
#include "unit.h"

class CComponent;
class CTankbody :
	public CUnit
{
private:
	//CTankbarrel*	
	CComponent*		m_com_pathfind;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	CTankbody(void);
	~CTankbody(void);
};
