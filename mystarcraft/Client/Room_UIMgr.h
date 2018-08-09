#pragma once
#include "Include.h"

class CUI;
class CUIRoot;
class CUI_IDslot;
class CUI_RoomExitBtn;
class CUI_RoomInfo;
class CUI_IME;
class CUI_Chat;
class CUI_Combobox;
class CUI_StartBtn;

class CRoom_UIMgr
{
	DECLARE_SINGLETON(CRoom_UIMgr)
private:
	typedef list<CUI*>::iterator UI_ITER;
	list<CUI*>	m_list;
	list<CUIRoot*>	m_rootlist;
	bool		m_ready_exit;

	CUIRoot*			m_userlist;
	CUIRoot*			m_chat;
	CUIRoot*			m_roominfo;
	CUIRoot*			m_startbtn_root;
	CUIRoot*			m_exitbtn_root;
	CUI_IDslot*			m_slotlist[8];
	CUI_Combobox*		m_combobox[8];
	CUI_RoomExitBtn*	m_roomexit_btn;
	CUI_RoomInfo*		m_roominfo_text;
	CUI_IME*			m_chat_ime;
	CUI_Chat*			m_chatlist;
	CUI_StartBtn*		m_startbtn;

	EXIT_FLAG		m_flag;
public:
	void Exit(EXIT_FLAG eflag);
	void UI_Reaction(const D3DXVECTOR2& vpt);
	void SetUser(const WCHAR* szid , int idx);
	void RoomExitUser(int idx);
	void SetRoomTitle(const TCHAR* sztitle);
	void SendChat(void);
	void Receive_Chat(const S_PT_ROOM_RECEIVE_CHAT_M& pinfo);
	void SetCombobox_ChangeData(const WCHAR* _race, const int& slotidx);
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	CRoom_UIMgr(void);
	~CRoom_UIMgr(void);
};
