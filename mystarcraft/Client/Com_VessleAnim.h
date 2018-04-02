#pragma once
#include "com_animation.h"

class CCom_VessleAnim :
	public CCom_Animation
{
	const vector<TEXINFO*>* tex_list;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_VessleAnim(D3DXMATRIX& objmat);
	~CCom_VessleAnim(void);
};
