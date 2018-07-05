#pragma once
#include "ui.h"

class CUI_RCreateOK;
class CUI_RCreateC;
class CUI_IME;

class CUI_RoomPopup :
	public CUI
{
private:
	typedef list<CUI*>::iterator UI_ITER;
	list<CUI*>			m_list;
	CUI_RCreateOK*		m_create_ok;
	CUI_RCreateC*		m_create_cancel;
	CUI_IME*			m_roomtitle_ime;
public:
	const TCHAR* GetRoomTitle(void);
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	bool UI_ptinrect(const D3DXVECTOR2 vpos);
	void Init_State(void);
	void Release(void);
public:
	CUI_RoomPopup(void);
	~CUI_RoomPopup(void);
};
