#pragma once

class CObj;
class CSkill
{
protected:
	CObj*	m_pobj;
public:
	virtual void use(void) = 0;
	virtual void Initialize(void);
	virtual void Update(void);
public:
	CSkill(void);
	virtual ~CSkill(void);
};
