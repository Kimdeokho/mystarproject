#pragma once
#include "unit.h"

CComponent;
class CTankbarrel :
	public CUnit
{
private:
	CObj*	m_tankbody;
	CComponent*	m_com_targetsearch;
	CComponent* m_com_weapon;
	bool	m_bsiegemode;
	bool	m_btransform_ready;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	bool	GetTransformReady(void);
	void	SetTransformReady(bool btransform);
	void	TransformSiegebarrel(void);
	void	TransformTankbarrel(void);
public:
	CTankbarrel(void);
	CTankbarrel(CObj* tankbody);
	~CTankbarrel(void);
};
