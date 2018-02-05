#pragma once
#include "Include.h"

class CUI
{
protected:
	MYRECT<float>	m_rect;
	const TCHAR*	m_statekey;
	const TEXINFO*	m_texinfo;
	D3DXVECTOR2		m_vpos;
	D3DXVECTOR3		m_vcenter;
	D3DXMATRIX		m_matworld;
	LPD3DXSPRITE	m_pSprite;
	D3DCOLOR		m_color;
protected:
	bool			m_bdestroy;

public:
	void SetDestroy(bool bdestroy);
	void SetPos(const D3DXVECTOR2& vpos);
public:
	bool GetDestroy(void);
	const MYRECT<float>& GetMyRect(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI(void);
	virtual ~CUI(void);
};
