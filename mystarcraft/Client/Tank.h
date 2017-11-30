#pragma once
#include "unit.h"


class CTank :
	public CUnit
{
private:
	bool				m_bsiegemode;
	bool				m_btransform_ready;

	CObj*				m_tankbarrel;

public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);

	//virtual CComponent*		GetComponent(COMPONENT_LIST ecom_name);
	virtual void Dead(void);
public:
	void barrel_sync(void);
	bool GetTransformReady(void);
	void SetTransformReady(bool btransform_ready);
	void Transform_Tankbody(void);
	void Transform_Siegebody(void);
	void Transform(void);
public:
	CTank(void);
	~CTank(void);
};
