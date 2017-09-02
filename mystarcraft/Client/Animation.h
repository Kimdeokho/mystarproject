#pragma once

#include "Include.h"

class CObj;
class CAnimation
{
protected:
	FRAME						m_frame;

	const vector<TEXINFO*>* 	m_animtexture[DIR_CNT];/*[]는 방향(각도)을 뜻한다*/

	const TCHAR*				m_objname;
	const TCHAR*				m_statkey;

	int							m_diridx;

	TEXINFO**					m_curtex;

	CObj*						m_pobj;
public:
	void Setcurtex_adress(TEXINFO** curtex);
	void Setdir(const int& idx);
public:
	virtual void Initialize(void) = 0;
	virtual void Update(void);
	virtual void Render(void) = 0;
	virtual void Release(void) = 0;
	virtual void SetAnimation(const TCHAR* statekey);
public:
	//virtual void SetState(void);
public:
	CAnimation(void);
	virtual ~CAnimation(void);
};
