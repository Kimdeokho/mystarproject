#pragma once
#include "ui.h"

class CCmd_btn :
	public CUI
{
private:
	const TCHAR* m_texkey;
	CMD_BTN		 m_btn_id;
	bool		 m_is_active;
public:
	CMD_BTN		GetCmdbtn_id(void);
	void		Init_btn(const TCHAR* texkey , CMD_BTN ebtn ,const D3DXVECTOR2& vpos , bool is_ative);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
public:
	CCmd_btn(const TCHAR* texkey, CMD_BTN ebtn , const D3DXVECTOR2& vpos ,bool is_active);
	~CCmd_btn(void);
};
