#pragma once
#include "effect.h"

class CGeneraEff :
	public CEffect
{
private:
	const vector<TEXINFO*>*	m_generaltex;
	int			m_maxsize;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CGeneraEff(void);
	CGeneraEff(const TCHAR* texkey ,const D3DXVECTOR2& vpos ,const D3DXVECTOR2& vsize, SORT_ID esortid, const float ftimespeed = 1.f , D3DCOLOR	_color = D3DCOLOR_ARGB(255,255,255,255));
	~CGeneraEff(void);
};
