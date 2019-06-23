#pragma once
#include "com_animation.h"

class CCom_TurretheadAnim :
	public CCom_Animation
{
private:
	float		m_fdir;

	bool					m_bsighton;
	bool					m_isescape;

	TEXINFO*				m_updateTex;
	TEXINFO*				m_staticTex;

	D3DXVECTOR2				m_staticPos;
	D3DXMATRIX				m_curMat;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_TurretheadAnim(D3DXMATRIX& objmat);
	~CCom_TurretheadAnim(void);
};
