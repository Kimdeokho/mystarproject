#pragma once

#include "Include.h"

#include "Component.h"

class CObj;
class CCom_Animation : public CComponent
{
protected:
	LPD3DXSPRITE				m_pSprite;
	FRAME						m_frame;

	const vector<TEXINFO*>* 	m_animtexture;/*[]는 방향(각도)을 뜻한다*/
	const vector<TEXINFO*>* 	m_statetexture;/*방향성은 없지만 상태가 있는 텍스쳐들*/
	const vector<TEXINFO*>*		m_generaltex; // 그냥 한장한장 넘기는 텍스쳐들

	const TCHAR*				m_objname;
	const TCHAR*				m_statkey;
	const TCHAR*				m_generalkey;

	float						m_curdiridx; //회전애니메이션에 쓰일 값
	int							m_texdiridx; //텍스쳐에 쓰일 방향인덱스
	int							m_newdiridx;


	TEXINFO*					m_curtex;


	D3DXMATRIX&					m_objmat;
	D3DXMATRIX					m_objshadow_mat;

	float						m_frotation_time;
	float						m_rotation_speed;
	float						m_newdgree;
	float						m_rotation_tick;
	int							m_rotation_dir;

	D3DXVECTOR3					m_newdirvec;
	D3DXVECTOR3					m_olddirvec;
	D3DXVECTOR3					m_vout;

	D3DXVECTOR3					m_vcenter;

	int							m_olddiridx;
	

//	bool						m_brotationcomplete;
	bool						m_banim_end;
	bool						m_battack_sync;
	bool						m_battack_end;
public:
	void Dir_Initialize(void);
	void DirIdxCalculation(void);
public:
	virtual void Initialize(CObj* pobj) = 0;
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void) = 0;
	virtual void SetAnimation(const TCHAR* statekey);
public:
	void			InitTexidx(void);
	const TCHAR*	GetAnimation(void);
	bool			GetRotationComplete(void);
	bool			GetAnimation_end(void);
	int				GetCurDirIdx(void);
	bool			GetAttackSync(void);
	bool			GetAttack_end(void);
	void			SetTextureName(const TCHAR* name);
public:
	//virtual void SetState(void);
public:
	CCom_Animation(D3DXMATRIX& objmat);
	virtual ~CCom_Animation(void);
};
