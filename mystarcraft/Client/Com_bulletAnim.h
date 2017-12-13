#pragma once
#include "com_animation.h"

class CCom_BulletAnim :
	public CCom_Animation
{
private:
	D3DXVECTOR3 m_vcenter;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_BulletAnim(D3DXMATRIX& objmat , const TCHAR* objname);
	~CCom_BulletAnim(void);
};
