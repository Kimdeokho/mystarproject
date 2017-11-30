#pragma once
#include "com_animation.h"

class CCom_LaserAnim :
	public CCom_Animation
{
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_LaserAnim(D3DXMATRIX& objmat , const TCHAR* objname);
	~CCom_LaserAnim(void);
};
