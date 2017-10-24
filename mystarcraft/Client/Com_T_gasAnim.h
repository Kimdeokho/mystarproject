#pragma once
#include "com_animation.h"

class CCom_T_gasAnim :
	public CCom_Animation
{
private:
	const vector<TEXINFO*>*	m_bigbuild_tex;
	float					m_fbuildtime;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_T_gasAnim(D3DXMATRIX& objmat , TEXINFO*& curtex , float fbuildtime);
	~CCom_T_gasAnim(void);
};
