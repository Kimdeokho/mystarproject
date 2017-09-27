#pragma once
#include "com_animation.h"

class CCom_SiegebodyAnim :
	public CCom_Animation
{
private:
	bool	m_bsiegemode;
	bool	m_btransforming;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	//bool	GetTransform(void);
public:
	CCom_SiegebodyAnim(D3DXMATRIX& objmat , TEXINFO*& curtex);
	~CCom_SiegebodyAnim(void);
};
