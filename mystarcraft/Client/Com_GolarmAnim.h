#pragma once
#include "com_animation.h"

class CCom_GolarmAnim :
	public CCom_Animation
{
private:
	int		m_attackloop;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_GolarmAnim(D3DXMATRIX& objmat);
	~CCom_GolarmAnim(void);
};
