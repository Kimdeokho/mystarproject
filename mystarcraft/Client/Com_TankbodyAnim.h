#pragma once
#include "com_animation.h"

class CCom_TankbodyAnim :
	public CCom_Animation
{
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_TankbodyAnim(D3DXMATRIX& objmat , TEXINFO*& curtex);
	~CCom_TankbodyAnim(void);
};
