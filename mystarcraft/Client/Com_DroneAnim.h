#pragma once
#include "Com_Animation.h"

class CCom_DroneAnim :
	public CCom_Animation
{
private:

public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_DroneAnim(D3DXMATRIX& objmat , TEXINFO*& curtex);
	~CCom_DroneAnim(void);
};
