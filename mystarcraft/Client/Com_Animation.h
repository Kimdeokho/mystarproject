#pragma once

#include "Include.h"

#include "Component.h"

class CObj;
class CCom_Animation : public CComponent
{
protected:
	FRAME						m_frame;

	const vector<TEXINFO*>* 	m_animtexture[DIR_CNT];/*[]는 방향(각도)을 뜻한다*/

	const TCHAR*				m_objname;
	const TCHAR*				m_statkey;

	float						m_curdiridx; //회전애니메이션에 쓰일 값
	int							m_texdiridx; //텍스쳐에 쓰일 방향인덱스
	int							m_newdiridx;

	TEXINFO*&					m_curtex;

	CObj*						m_pobj;

	D3DXMATRIX&					m_objmat;

	int							m_rotation_dir;
	float						m_frotation_time;

	float						m_newdgree;

	D3DXVECTOR3					m_newdirvec;
	D3DXVECTOR3					m_olddirvec;
	D3DXVECTOR3					m_vout;

	int							m_olddiridx;
public:
	void DirIdxCalculation(void);
public:
	virtual void Initialize(CObj* pobj) = 0;
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void) = 0;
	virtual void SetAnimation(const TCHAR* statekey);
public:
	//virtual void SetState(void);
public:
	CCom_Animation(D3DXMATRIX& objmat , TEXINFO*& curtex);
	virtual ~CCom_Animation(void);
};
