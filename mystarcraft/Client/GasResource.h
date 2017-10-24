#pragma once
#include "obj.h"

class CGasResource :
	public CObj
{
private:
	const vector<TEXINFO*>*	m_vecTex;
private:
	D3DXVECTOR2		m_vtxpos[4];
	int				m_curvtxidx[4];
	int				m_oldvtxidx[4];
private:
	int				m_gasa_mount;
public:
	void building_area_Initialize(void);
	void building_area_release(void);
public:
	void setgas_amount(const int& iamount);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CGasResource(void);
	~CGasResource(void);
};
