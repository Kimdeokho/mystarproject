#pragma once
#include "ui.h"

class CUI_form :
	public CUI
{
private:
	bool		m_is_active;
public:
	void set_texture(const TCHAR* texkey , const D3DXVECTOR2& vpos);
	void set_texture(const TCHAR* texkey );
	void set_active(bool is_active);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_form(void);
	CUI_form(const D3DXVECTOR2& vsize);
	CUI_form(const TCHAR* texkey , const D3DXVECTOR2& vpos);
	~CUI_form(void);
};
