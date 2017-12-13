#pragma once
#include "com_animation.h"

class CCom_VultureAnim :
	public CCom_Animation
{
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_VultureAnim(D3DXMATRIX& objmat);
	~CCom_VultureAnim(void);
};
