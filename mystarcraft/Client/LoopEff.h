#pragma once
#include "effect.h"

class CLoopEff :
	public CEffect
{
private:
	const vector<TEXINFO*>*	m_generaltex;

public:
	void SetTexname(const TCHAR* texkey);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CLoopEff(const TCHAR* texkey ,const D3DXVECTOR2& vpos ,const D3DXVECTOR2& vsize, SORT_ID esortid, const float& ftimespeed = 1.f);
	~CLoopEff(void);
};
