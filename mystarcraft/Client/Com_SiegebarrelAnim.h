#pragma once
#include "com_animation.h"

class CCom_SiegebarrelAnim :
	public CCom_Animation
{
private:
	bool m_bsiegemode;
	bool m_btransforming;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
public:
	CCom_SiegebarrelAnim(D3DXMATRIX& objmat );
	~CCom_SiegebarrelAnim(void);
};
