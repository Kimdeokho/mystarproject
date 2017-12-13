#pragma once

#include "Include.h"

#include "Component.h"

class CObj;
class CCom_Animation : public CComponent
{
protected:
	LPD3DXSPRITE				m_pSprite;
	FRAME						m_frame;

	const vector<TEXINFO*>* 	m_animtexture;/*[]�� ����(����)�� ���Ѵ�*/
	const vector<TEXINFO*>* 	m_statetexture;/*���⼺�� ������ ���°� �ִ� �ؽ��ĵ�*/
	const vector<TEXINFO*>*		m_generaltex;

	const TCHAR*				m_objname;
	const TCHAR*				m_statkey;
	const TCHAR*				m_generalkey;

	float						m_curdiridx; //ȸ���ִϸ��̼ǿ� ���� ��
	int							m_texdiridx; //�ؽ��Ŀ� ���� �����ε���
	int							m_newdiridx;

	TEXINFO*					m_curtex;


	D3DXMATRIX&					m_objmat;
	D3DXMATRIX					m_objshadow_mat;

	float						m_frotation_time;
	float						m_rotation_speed;
	float						m_newdgree;
	int							m_rotation_dir;

	D3DXVECTOR3					m_newdirvec;
	D3DXVECTOR3					m_olddirvec;
	D3DXVECTOR3					m_vout;

	int							m_olddiridx;
	

	bool						m_brotationcomplete;
	bool						m_banim_end;
	bool						m_battack_sync;
	bool						m_battack_end;
public:
	void Dir_Initialize(void);
	void DirIdxCalculation(void);
	void DirIdxCalculation32(void);
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
public:
	//virtual void SetState(void);
public:
	CCom_Animation(D3DXMATRIX& objmat);
	virtual ~CCom_Animation(void);
};
