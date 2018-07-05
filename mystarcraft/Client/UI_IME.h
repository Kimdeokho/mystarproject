#pragma once
#include "ui.h"

class CUI_IME :
	public CUI
{
public:
private:
	D3DXMATRIX		m_matfont;
	HWND			m_InputEdit;
	HWND			m_InputBufferEdit;
	TCHAR			m_text[256];

	D3DXVECTOR2		m_vsize;
public:
	HWND	GetHandle(void){return m_InputEdit;}
	void	TextClear(void);
	const TCHAR*	GetMessage(void);
public:
	virtual void Initialize( void );
	virtual void Update( void );
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Init_State(void);
	virtual void Render( void );
	virtual void Release( void );
public:
	CUI_IME(D3DXVECTOR2 vpos , D3DXVECTOR2 vsize );
	~CUI_IME(void);
};
