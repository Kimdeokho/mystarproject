#pragma once
#include "Include.h"

class CObj;

class CSkill
{
protected:
	CObj*			m_pobj;
	CObj*			m_ptarget;
	const TEXINFO*	m_curtex;

	FRAME			m_frame;
	D3DXMATRIX		m_matWorld;
	D3DXVECTOR2		m_vPos;
	D3DXVECTOR2		m_vtarget_pos;


	SKILL_NAME		m_skillname;	

	int				m_targetid;

	bool			m_bdestroy;
public:
	SKILL_NAME GetSkillName(void);
	bool GetDestroy(void);
	void SetDestroy(bool bdestroy);
public:
	virtual void Use(const int& objid , const D3DXVECTOR2& vpt);
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CSkill(void);
	virtual ~CSkill(void);
};
