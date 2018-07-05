#include "StdAfx.h"
#include "Session_Mgr.h"

#include "TestSession.h"
#include "SceneMgr.h"
#include "Login_UIMgr.h"
#include "Loby_UIMgr.h"
#include "Room_UIMgr.h"
#include "RoomSession_Mgr.h"

IMPLEMENT_SINGLETON(CSession_Mgr)

CSession_Mgr::CSession_Mgr(void)
{
}

CSession_Mgr::~CSession_Mgr(void)
{
	Release();
}

void CSession_Mgr::Initialize(void)
{
	m_TCPsession = NULL;
	m_UDPsession = NULL;
	m_TCPsession = new CTestSession;
	m_UDPsession = new CTestSession;

	memset(m_Read_TCPBuf , 0 , sizeof(MAX_BUFFER_LENGTH)*sizeof(BYTE));
	memset(m_Read_UDPBuf , 0 , sizeof(MAX_BUFFER_LENGTH)*sizeof(BYTE));

}
void CSession_Mgr::Update(void)
{
	Read_UDPPacket();
	Read_TCPPacket();
}
void CSession_Mgr::Read_UDPPacket(void)
{
	//if(m_UDPsession->ReadFromPacket(dwProtocol , tempaddres , tempport ,Packet , dwPacketLength ))
	//{
	//	if(dwProtocol == PT_TEST)
	//	{
	//		int a = 0;
	//	}
	//}
}
void CSession_Mgr::Read_TCPPacket(void)
{
	if(NULL == m_TCPsession)
		return;

	// 패킷을 받을때 사용하는 프로토콜, 길이, 패킷 변수
	DWORD dwProtocol = 0, dwPacketLength = 0;

	while (TRUE)
	{
		if (m_TCPsession->ReadPacket(dwProtocol, m_Read_TCPBuf, dwPacketLength))
		{
			if(PT_LOBY_ENTER_SUCC_U == dwProtocol)
			{
				READ_TCP_PACKET(PT_LOBY_ENTER_SUCC_U);

				memcpy(&m_session_info , &Data , sizeof(SESSION_INFO));

			}
			else if(PT_CREATE_ROOM_SUCC_U == dwProtocol)
			{
				READ_TCP_PACKET(PT_CREATE_ROOM_SUCC_U);

				//CLoby_UIMgr::GetInstance()->Exit(LB_ROOM_CREATE);
				//CLoby_UIMgr::GetInstance()->Room_Popup(false);

				CRoom_UIMgr::GetInstance()->SetRoomTitle(Data.TITLE);
			}
			else if(PT_ROOMLIST_ERASE_M == dwProtocol)
			{
				READ_TCP_PACKET(PT_ROOMLIST_ERASE_M);

				if(SCENE_LOBY == CSceneMgr::GetInstance()->GetScene())
					CLoby_UIMgr::GetInstance()->Erase_Room(Data);
			}
			else if(PT_ROOMLIST_RENEWAL_M == dwProtocol)
			{
				READ_TCP_PACKET(PT_ROOMLIST_RENEWAL_M);

				if(m_session_info.SESSION_ID != Data.MASTERSESSION_ID &&
					SCENE_LOBY == CSceneMgr::GetInstance()->GetScene())
				{
					CLoby_UIMgr::GetInstance()->Add_RoomTitle(Data);
					//리뉴얼 받음
				}
			}
			else if(PT_ROOM_USER_ENTRY_M == dwProtocol)
			{
				//누군가 방에 들어왔다
				READ_TCP_PACKET(PT_ROOM_USER_ENTRY_M);

				CRoomSession_Mgr::GetInstance()->Receive_NewUser(Data);
					//룸매니저에서 새로 들어온 유저정보 습득
			}
			else if(PT_ROOM_ENTER_SUCC_U == dwProtocol)
			{
				//방에 이미 들어와있는 유저들의 정보받기
				READ_TCP_PACKET(PT_ROOM_ENTER_SUCC_U);
				CRoomSession_Mgr::GetInstance()->Init_RoomUserInfo(Data);
				CRoom_UIMgr::GetInstance()->SetRoomTitle(Data.TITLE);
			}
			else if(PT_ROOM_LEAVE_SUCC_U == dwProtocol)
			{
				READ_TCP_PACKET(PT_ROOM_LEAVE_SUCC_U);
				//방탈출 성공
				CRoom_UIMgr::GetInstance()->Exit(R_ROOM_EXIT);
			}
			else if(PT_ROOM_LEAVE_M == dwProtocol)
			{
				READ_TCP_PACKET(PT_ROOM_LEAVE_M);
				//방에서 누군가 나갔다
				CRoomSession_Mgr::GetInstance()->Exit_Others(Data);
			}
			else if(PT_ROOM_RECEIVE_CHAT_M == dwProtocol)
			{
				READ_TCP_PACKET(PT_ROOM_RECEIVE_CHAT_M);
				//채팅메시지 입수
				CRoom_UIMgr::GetInstance()->Receive_Chat(Data);
			}
		}
		else
			break;
	}
}

CTestSession* CSession_Mgr::GetTCP_Session(void)
{
	return m_TCPsession;
}
CTestSession* CSession_Mgr::GetUDP_Session(void)
{
	return m_UDPsession;
}

bool CSession_Mgr::BeginTCP(void)
{
	if(m_TCPsession->BeginTcp("127.0.0.1" , DEFAULT_PORT))
		return true;

	return false;//로그인 실패
}

bool CSession_Mgr::BeginUDP(void)
{
	if(m_UDPsession->BeginUdp(m_session_info.REAL_PORT))
		return true;

	return false;
}
void CSession_Mgr::Release(void)
{
	if(NULL != m_TCPsession)
	{
		m_TCPsession->End();
		Safe_Delete(m_TCPsession);
	}
	if(NULL != m_UDPsession)
	{
		m_UDPsession->End();
		Safe_Delete(m_UDPsession);
	}
}

void CSession_Mgr::Write_TCP_Packet(DWORD PT , BYTE* packet, DWORD Length)
{
	m_TCPsession->WritePacket(PT , packet , Length);
}

SESSION_INFO CSession_Mgr::GetSession_Info(void)
{
	return m_session_info;
}

