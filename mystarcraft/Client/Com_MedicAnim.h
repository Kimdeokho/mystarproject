#pragma once
#include "com_animation.h"

class CCom_MedicAnim :
	public CCom_Animation
{
private:
	const TCHAR*	m_tempstate;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_MedicAnim(D3DXMATRIX& objmat);
	~CCom_MedicAnim(void);
};
