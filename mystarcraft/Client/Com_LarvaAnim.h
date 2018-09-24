#pragma once
#include "Com_Animation.h"

class CCom_LarvaAnim 
	: public CCom_Animation
{
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_LarvaAnim(D3DXMATRIX& objmat );
	~CCom_LarvaAnim(void);
};
