#pragma once
#include "com_animation.h"

class CCom_BrudeAnim :
	public CCom_Animation
{
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_BrudeAnim(D3DXMATRIX& objmat );
	~CCom_BrudeAnim(void);
};
