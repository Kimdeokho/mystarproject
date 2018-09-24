#pragma once
#include "com_animation.h"

class CCom_LarvaEggAnim :
	public CCom_Animation
{
private:
	const TCHAR*	m_birthkey;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_LarvaEggAnim(D3DXMATRIX& objmat , const TCHAR* birthkey);
	~CCom_LarvaEggAnim(void);
};
