#include "StdAfx.h"
#include "RoomSession_Mgr.h"

#include "Session_Mgr.h"
#include "Room_UIMgr.h"

IMPLEMENT_SINGLETON(CRoomSession_Mgr)
CRoomSession_Mgr::CRoomSession_Mgr(void)
{
	memset(m_Roomsession_info , 0 , sizeof(SESSION_INFO*)*8 );
}

CRoomSession_Mgr::~CRoomSession_Mgr(void)
{
}

void CRoomSession_Mgr::Initialize(void)
{
	
	m_mySession_info = CSession_Mgr::GetInstance()->GetSession_Info();
	if(CSession_Mgr::GetInstance()->BeginUDP())
	{
		m_myUDP_Session = CSession_Mgr::GetInstance()->GetUDP_Session();
	}

	CRoom_UIMgr::GetInstance()->SetUserID(m_mySession_info.USER_ID , 0);
}

void CRoomSession_Mgr::Update(void)
{
}
void CRoomSession_Mgr::Release(void)
{
	for(int i = 0; i < 8; ++i)
	{
		if(NULL != m_Roomsession_info[i])
			Safe_Delete(m_Roomsession_info[i]);
	}
}
void CRoomSession_Mgr::Exit_Others(const S_PT_ROOM_LEAVE_M& _info)
{
	for(int i = 0; i < 8; ++i)
	{
		if(NULL == m_Roomsession_info[i])
			continue;

		if(_info.SESSION_ID == m_Roomsession_info[i]->SESSION_ID)
		{
			Safe_Delete(m_Roomsession_info[i]);
			CRoom_UIMgr::GetInstance()->SetUserID( L"" , i);
		}
	}
}
void CRoomSession_Mgr::Receive_NewUser(const S_PT_ROOM_USER_ENTRY_M& _info)
{
	USHORT idx =  _info.SLOT_DIX;

	if(NULL == m_Roomsession_info[idx])
	{
		m_Roomsession_info[idx] = new SESSION_INFO;
		wcscpy_s(m_Roomsession_info[idx]->REAL_ADDRESS , _info.REAL_ADDRESS);
		wcscpy_s(m_Roomsession_info[idx]->VIRTUAL_ADDRESS , _info.VIRTUAL_ADDRESS);
		wcscpy_s(m_Roomsession_info[idx]->USER_ID , _info.USER_ID);
		m_Roomsession_info[idx]->SESSION_ID = _info.SESSION_ID;
		m_Roomsession_info[idx]->REAL_PORT = _info.REAL_PORT;
		m_Roomsession_info[idx]->VIRTUAL_PORT = _info.VIRTUAL_PORT;

		CRoom_UIMgr::GetInstance()->SetUserID(m_Roomsession_info[idx]->USER_ID , idx);
	}
}

void CRoomSession_Mgr::Init_RoomUserInfo(const S_PT_ROOM_ENTER_SUCC_U& _info)
{
	//_info.TITLE roomUIMGR에서 가져갈 데이터

	for(int i = 0; i < 8; ++i)
	{
		if(0 != _info.ROOM_USER_INFO[i].SESSION_ID &&
			NULL == m_Roomsession_info[i])
		{
			m_Roomsession_info[i] = new SESSION_INFO;
			wcscpy_s(m_Roomsession_info[i]->REAL_ADDRESS , _info.ROOM_USER_INFO[i].REAL_ADDRESS);
			wcscpy_s(m_Roomsession_info[i]->VIRTUAL_ADDRESS , _info.ROOM_USER_INFO[i].VIRTUAL_ADDRESS);
			wcscpy_s(m_Roomsession_info[i]->USER_ID , _info.ROOM_USER_INFO[i].USER_ID);
			m_Roomsession_info[i]->SESSION_ID = _info.ROOM_USER_INFO[i].SESSION_ID;
			m_Roomsession_info[i]->REAL_PORT = _info.ROOM_USER_INFO[i].REAL_PORT;
			m_Roomsession_info[i]->VIRTUAL_PORT = _info.ROOM_USER_INFO[i].VIRTUAL_PORT;

			CRoom_UIMgr::GetInstance()->SetUserID(m_Roomsession_info[i]->USER_ID , i);
		}
	}
}

