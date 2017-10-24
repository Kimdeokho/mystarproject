#pragma once
#include "com_animation.h"

class CCom_SCVAnim :
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
	CCom_SCVAnim(D3DXMATRIX& objmat , TEXINFO*& curtex);
	~CCom_SCVAnim(void);
};
