#pragma once

#include "Include.h"

class CUI;
class CUIRoot;
class CUI_IME;
class CUI_Connect;
class CLogin_UIMgr
{
	DECLARE_SINGLETON(CLogin_UIMgr)
private:
	typedef list<CUI*>::iterator UI_ITER;
	list<CUI*>	m_list;
	list<CUIRoot*> m_rootlist;

	CUI_Connect*	m_log_connect;
	CUI_IME*		m_logime;
	bool		m_ready_exit;
public:
	void Exit(void);
	void UI_Reaction(const D3DXVECTOR2& vpt);
	void Connect_Server(void);
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	CLogin_UIMgr(void);
	~CLogin_UIMgr(void);
};
