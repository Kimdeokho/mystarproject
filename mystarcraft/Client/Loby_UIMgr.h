#pragma once

#include "Include.h"

class CUI;
class CUIRoot;
class CUI_CreateRoom;
class CUI_Join;
class CUI_RoomPopup;
class CUI_RCreateOK;
class CUI_RCreateC;
class CUI_IME;
class CUI_RoomList;

class CLoby_UIMgr
{
	DECLARE_SINGLETON(CLoby_UIMgr)
private:
	typedef list<CUI*>::iterator UI_ITER;
	list<CUI*>	m_list;
	list<CUIRoot*> m_rootlist;
	bool		m_ready_exit;

	CUI_RoomPopup*		m_roompopup;
	CUI_Join*			m_join;
	CUI_CreateRoom*		m_createroom;
	CUI_RoomList*		m_RoomList;
	//CUI_RCreateOK*		m_CreateOK;
	//CUI_RCreateC*		m_CreateCancel;
	//CUI_IME*			m_TitleIME;
	
	EXIT_FLAG				m_flag;
public:
	void Exit(EXIT_FLAG eflag);
	void UI_Reaction(const D3DXVECTOR2& vpt);
	void Room_Popup(bool is_popup);
	void Create_Room(void);
	void Add_RoomTitle(const S_PT_ROOMLIST_RENEWAL_M& roominfo);
	void Erase_Room(const S_PT_ROOMLIST_ERASE_M& roominfo);
	int	 GetRoomIdx(void);
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	CLoby_UIMgr(void);
	~CLoby_UIMgr(void);
};
