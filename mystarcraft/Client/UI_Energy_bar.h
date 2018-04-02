#pragma once
#include "ui.h"

class CObj;
class CUI_Energy_bar :
	public CUI
{
private:
	CObj*					m_pobj;
	const D3DXVECTOR2*		m_uipos;

	float					m_fwidth_size;
	float					m_fhp;
	float					m_fmaxhp;
	float					m_fmp;
	float					m_fmaxmp;
	float					m_fheight;

	D3DXMATRIX				m_matback;
	D3DXMATRIX				m_matmp;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Energy_bar(CObj* pobj , float fwidth_size);
	CUI_Energy_bar(CObj* pobj , float fwidth_size , float fheight);
	~CUI_Energy_bar(void);
};
