#pragma once
#include "com_animation.h"

class CCom_MutalAnim :
	public CCom_Animation
{
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void SetAnimation(const TCHAR* statekey);
	virtual void Release(void);	
public:
	CCom_MutalAnim(D3DXMATRIX& objmat);
	~CCom_MutalAnim(void);
};
