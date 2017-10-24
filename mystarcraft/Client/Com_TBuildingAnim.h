#pragma once
#include "com_animation.h"

class CCom_TBuildingAnim :
	public CCom_Animation
{
private:
	const vector<TEXINFO*>*	m_bigbuild_tex;
	float					m_fbuildtime;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_TBuildingAnim(const TCHAR* objname , D3DXMATRIX& objmat , TEXINFO*& curtex);
	~CCom_TBuildingAnim(void);
};
