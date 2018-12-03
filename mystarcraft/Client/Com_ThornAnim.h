#pragma once
#include "Com_Animation.h"

class CCom_ThornAnim :
	public CCom_Animation
{
private:
	D3DXVECTOR3 m_vcenter;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_ThornAnim(D3DXMATRIX& objmat , const TCHAR* objname);
	~CCom_ThornAnim(void);
};
