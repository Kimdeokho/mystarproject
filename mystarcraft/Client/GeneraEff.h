#pragma once
#include "effect.h"

class CGeneraEff :
	public CEffect
{
private:
	const vector<TEXINFO*>*	m_generaltex;
	int			m_maxsize;
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	CGeneraEff(void);
	CGeneraEff(const TCHAR* texkey ,const D3DXVECTOR2& vpos ,const D3DXVECTOR2& vsize, SORT_ID esortid, const float& ftimespeed = 1);
	~CGeneraEff(void);
};
