#pragma once
#include "com_animation.h"

class CCom_GollegAnim :
	public CCom_Animation
{
private:
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_GollegAnim(D3DXMATRIX& objmat);
	~CCom_GollegAnim(void);
};
