#pragma once

#include "Include.h"

class CTestSession;
class CRoomSession_Mgr
{
	DECLARE_SINGLETON(CRoomSession_Mgr)
private:
	USER_INFO*		m_Roomsession_info[8];
	USER_INFO*		m_myinfo;
	CTestSession*	m_myUDP_Session;

	DWORD_PTR		m_master_sessionID;
	WCHAR			m_title[32];
	int				m_curslot;
	int				m_player_cnt;

	PLACE_DATA		m_placedata[8];
public:
	void	quit_play(const DWORD sessionid);
	void	Receive_NewUser(const S_PT_ROOM_USER_ENTRY_M& _info);
	void	Init_RoomUserInfo(const S_PT_ROOM_USER_RENEWAL_SUCC_U& _info);
	void	Exit_Others(const S_PT_ROOM_LEAVE_M& _info);
	int		GetMyRoomslot(void){return m_curslot;}
	void						SetPlaceData(PLACE_DATA* _data);
	void						SetMasterSession_ID(DWORD_PTR _sessionid){ m_master_sessionID = _sessionid;}
	void						SetTribe(const WCHAR* _tribe, USHORT idx);
	void						SetTitle(const WCHAR* _title) {wcscpy_s(m_title , _title);}
	void						SetPlayerCnt(const int& playercnt) { m_player_cnt = playercnt;}


	PLACE_DATA*					GetPlaceData(void);	
	DWORD_PTR					GetMasterSession_ID(void);
	DWORD_PTR					GetMySession_ID(void){return m_myinfo->SESSION_ID;}
	USER_INFO**					GetUserinfolist(void){return m_Roomsession_info;}
	const WCHAR*				GetTitle(void){return m_title;}
	int							GetPlayerCnt(void){return m_player_cnt;}
	USER_INFO*					GetMyinfo(void){return m_myinfo;}

	void	Release_sessioninfo(void);
public:
	void WriteTurnData(DWORD _protocol , const BYTE* packet , DWORD _length);
	void WriteAll(DWORD _protocol , const BYTE* packet , DWORD _length);
public:
	void	Initialize(void);
	void	Update(void);
	void	Release(void);
public:
	CRoomSession_Mgr(void);
	~CRoomSession_Mgr(void);
};
