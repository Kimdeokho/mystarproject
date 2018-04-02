#pragma once
#include "obj.h"

class CAstar;


class CSkill;
typedef boost::unordered_map<SKILL_NAME , CSkill*> SKILL_PAIR;

class CUI;
class CUnit :
	public CObj
{
protected:
	CComponent*		m_com_anim;
	CComponent*		m_com_targetsearch;
	CComponent*		m_com_pathfind;
	CComponent*		m_com_collision;
	CComponent*		m_com_usingskill;
	CComponent*		m_com_cc;
protected:
	UPG_INFO*		m_upg_info;
	bool			m_upg_feedback[UPG_END];
protected:
	D3DXMATRIX		m_matshadow;
protected:
	CUI*			m_miniunit_display;

	TEXINFO*						m_curtex;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Dead(void);
public:
	CUnit(void);
	virtual ~CUnit(void);
};
