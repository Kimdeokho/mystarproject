#pragma once
#include "com_animation.h"

class CCom_MarineAnim :
	public CCom_Animation
{
private:
	bool	m_banimplay;
	int		m_attackloop;
	const TCHAR*	m_tempkey;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_MarineAnim(D3DXMATRIX& objmat , TEXINFO*& curtex);
	~CCom_MarineAnim(void);
};
