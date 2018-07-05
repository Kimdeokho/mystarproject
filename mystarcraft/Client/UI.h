#pragma once
#include "Include.h"

class CUI
{
protected:
	D3DXMATRIX*		m_parentmat;
	D3DXMATRIX		m_matworld;
	D3DXVECTOR2		m_vpos;
	D3DXVECTOR3		m_vcenter;
	

	MYRECT<float>	m_rect;
	const TCHAR*	m_statekey;
	const TEXINFO*	m_texinfo;

	LPD3DXSPRITE	m_pSprite;
	D3DCOLOR		m_color;
protected:
	bool			m_active;
	bool			m_bdestroy;
public:
	void SetDestroy(bool bdestroy);
	void SetPos(const D3DXVECTOR2& vpos);
	void SetActive(bool _active);
	void SetColor(D3DCOLOR ecolor);
	void SetParentMat(D3DXMATRIX* parmat);
public:
	const D3DXVECTOR2& GetPos(void);
public:
	bool GetDestroy(void);
	const MYRECT<float>& GetMyRect(void);
	D3DXMATRIX*	GetMatrix_Adress(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Init_State(void){};
	virtual void Release(void);
public:
	CUI(void);
	virtual ~CUI(void);
};
