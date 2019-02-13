#pragma once
#include "unit.h"


class CTank :
	public CUnit
{
private:
	bool				m_bsiegemode;
	bool				m_btransform_ready;

	CObj*				m_tankbarrel;

	CCom_Animation*			m_com_tankanim;
	CCom_Animation*			m_com_siegeanim;

	bool				m_bcheck;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey, bool* waitkey);

	//virtual CComponent*		GetComponent(COMPONENT_LIST ecom_name);
	virtual void Dead(void);

	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	void barrel_sync(void);
	bool GetTransformReady(void);
	bool GetSiegemode(void);

	void SetTransformReady(bool btransform_ready);
	void Transform_Tankbody(void);
	void Transform_Siegebody(void);
	void Transform(void);
	
public:
	CTank(void);
	~CTank(void);
};
