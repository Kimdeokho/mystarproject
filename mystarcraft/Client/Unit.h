#pragma once
#include "obj.h"

class CAstar;


class CUI;
class CUnit :
	public CObj
{
protected:
	//CComponent*					m_panimation;
	CComponent*		m_com_anim;
	CComponent*		m_com_targetsearch;
	CComponent*		m_com_pathfind;
	CComponent*		m_com_collision;
protected:
	D3DXMATRIX	m_matshadow;
protected:
	//const vector<TEXINFO*>* 	m_unittexture[DIR_CNT];/*[]는 방향(각도)을 뜻한다*/
protected:
	//int					m_diridx;
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
