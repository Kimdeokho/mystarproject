#pragma once

#include "Include.h"

class CTestSession;
class CRoomSession_Mgr
{
	DECLARE_SINGLETON(CRoomSession_Mgr)
private:
	SESSION_INFO*	m_Roomsession_info[8];
	SESSION_INFO	m_mySession_info;
	CTestSession*	m_myUDP_Session;
public:
	void	Receive_NewUser(const S_PT_ROOM_USER_ENTRY_M& _info);
	void	Init_RoomUserInfo(const S_PT_ROOM_ENTER_SUCC_U& _info);
	void	Exit_Others(const S_PT_ROOM_LEAVE_M& _info);
public:
	void	Initialize(void);
	void	Update(void);
	void	Release(void);
public:
	CRoomSession_Mgr(void);
	~CRoomSession_Mgr(void);
};
