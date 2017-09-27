#pragma once
#include "unit.h"


class CTank :
	public CUnit
{
private:
	bool				m_bsiegemode;
	bool				m_btransform_ready;

	CObj*				m_tankbody;
	CObj*				m_tankbarrel;
	CComponent*			m_com_pathfind;


public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void SetDestroy(bool bdestroy);
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
