#pragma once
#include "component.h"

class CAnimation;
class CCom_multitexture :
	public CComponent
{
private:
	LPD3DXSPRITE				m_pSprite;
	TEXINFO*					m_curtex;

	CAnimation*					m_anim;
private:
	const D3DXVECTOR2&				m_vobjpos;
	D3DXMATRIX&				m_objmat;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	//CCom_multitexture(void);
	CCom_multitexture(const D3DXVECTOR2& vpos , D3DXMATRIX& matworld , CAnimation* panim);
	~CCom_multitexture(void);
};
