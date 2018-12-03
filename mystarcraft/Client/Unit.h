#pragma once
#include "obj.h"



class CAstar;
class CSkill;
class CUI;
class CCom_Animation;
class CCom_Pathfind;
class CCom_AirPathfind;
class CCom_Collision;
class CCom_CC;
class CCom_UsingSkill;

typedef boost::unordered_map<SKILL_NAME , CSkill*> SKILL_PAIR;
class CUnit :
	public CObj
{
protected:
	CCom_Animation*		m_com_anim;
	CCom_Pathfind*		m_com_pathfind;
	CCom_AirPathfind*	m_com_air_pathfind;
	CCom_Collision*		m_com_collision;
	CComponent*			m_com_targetsearch;		
	CCom_UsingSkill*	m_com_usingskill;
	CCom_CC*			m_com_cc;
protected:
	UPG_INFO*		m_upg_info;
	bool			m_upg_state[UPG_END];
protected:
	//D3DXMATRIX		m_matshadow;
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
