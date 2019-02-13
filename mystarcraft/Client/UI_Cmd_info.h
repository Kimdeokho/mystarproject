#pragma once
#include "ui.h"

class CObj;
class CUI_Cmd_info :
	public CUI
{
private:
	vector<CUI*>	m_cmdbtn_list;
	D3DXVECTOR2		m_vcmdbtn_pos[9];
	CMD_BTN			m_clicked_cmd_btn;
public:
	void Create_Cmdbtn(const int& idx ,const TCHAR* texkey, CMD_BTN ebtn , bool is_active = true);
	void Click_cmdbtn(const D3DXVECTOR2& vpt);
	void Update_Cmdbtn(CObj* pobj);
	void clear_btn(void);
	void T_Cmdbtn_B_buildsetting(void);
	void T_Cmdbtn_V_buildsetting(void);
	void Z_Cmdbtn_B_buildsetting(void);
	void Z_Cmdbtn_V_buildsetting(void);
	bool active_cmdbtn(const int& idx, CMD_BTN ebtn);
	CMD_BTN Get_clicked_btn(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Cmd_info(void);
	~CUI_Cmd_info(void);
};
