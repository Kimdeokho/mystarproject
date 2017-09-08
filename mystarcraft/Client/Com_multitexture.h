#pragma once
#include "component.h"


class CCom_multitexture :
	public CComponent
{
private:
	LPD3DXSPRITE					m_pSprite;
	TEXINFO*						m_curtex;

	CComponent*						m_anim;
private:
	const D3DXVECTOR2&				m_vobjpos;
	
	D3DXMATRIX&				m_objmat;
	bool					m_bshadowrender;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void SetAnimation(const TCHAR* statekey);
public:
	//CCom_multitexture(void);
	CCom_multitexture(const D3DXVECTOR2& vpos , D3DXMATRIX& matworld , CComponent* panim );
	~CCom_multitexture(void);
};
