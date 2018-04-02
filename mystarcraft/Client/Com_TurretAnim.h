#pragma once
#include "com_animation.h"

//필요없는 클래스
class CCom_TurretAnim :
	public CCom_Animation
{
private:
	float		m_fbuildtime;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_TurretAnim(D3DXMATRIX& objmat , float fbuildtime);
	~CCom_TurretAnim(void);
};
