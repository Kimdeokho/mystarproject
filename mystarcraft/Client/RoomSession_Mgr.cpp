#include "StdAfx.h"
#include "RoomSession_Mgr.h"

#include "Session_Mgr.h"
#include "Room_UIMgr.h"
#include "SceneMgr.h"
#include "TestSession.h"
IMPLEMENT_SINGLETON(CRoomSession_Mgr)
CRoomSession_Mgr::CRoomSession_Mgr(void)
{
	memset(m_Roomsession_info , 0 , sizeof(SESSION_INFO*)*8 );
	m_myinfo = NULL;
	m_myUDP_Session = NULL;
}

CRoomSession_Mgr::~CRoomSession_Mgr(void)
{
	Safe_Delete(m_myinfo);
	Release();
}

void CRoomSession_Mgr::Initialize(void)
{
	m_player_cnt = 0;
	SESSION_INFO tempinfo = CSession_Mgr::GetInstance()->GetSession_Info();
	m_master_sessionID = 0;

	if(NULL == m_myinfo)
		m_myinfo = new USER_INFO;

	wcscpy_s( m_myinfo->REAL_ADDRESS , tempinfo.REAL_ADDRESS);
	wcscpy_s( m_myinfo->VIRTUAL_ADDRESS , tempinfo.VIRTUAL_ADDRESS);
	wcscpy_s( m_myinfo->USER_ID , tempinfo.USER_ID);
	wcscpy_s( m_myinfo->TRIBE , L"");
	m_myinfo->SESSION_ID = tempinfo.SESSION_ID;
	m_myinfo->TEAMNUM = 0;
	m_myinfo->VIRTUAL_PORT = tempinfo.VIRTUAL_PORT;
	m_myinfo->REAL_PORT = tempinfo.REAL_PORT;

	if(NULL == m_myUDP_Session && CSession_Mgr::GetInstance()->BeginUDP())
	{
		m_myUDP_Session = CSession_Mgr::GetInstance()->GetUDP_Session();
	}

	
	m_curslot = 0;
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
	Safe_Delete(m_myinfo);
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

			if(SCENE_ROOM == CSceneMgr::GetInstance()->GetScene())
			{
				CRoom_UIMgr::GetInstance()->SetUser( L"" , i);
				CRoom_UIMgr::GetInstance()->RoomExitUser(i);
				--m_player_cnt;
			}
		}
	}
}
void CRoomSession_Mgr::Receive_NewUser(const S_PT_ROOM_USER_ENTRY_M& _info)
{
	USHORT idx =  _info.SLOT_IDX;

	if(NULL == m_Roomsession_info[idx])
	{
		m_Roomsession_info[idx] = new USER_INFO;
		wcscpy_s(m_Roomsession_info[idx]->REAL_ADDRESS , _info.REAL_ADDRESS);
		wcscpy_s(m_Roomsession_info[idx]->VIRTUAL_ADDRESS , _info.VIRTUAL_ADDRESS);
		wcscpy_s(m_Roomsession_info[idx]->USER_ID , _info.USER_ID);
		wcscpy_s(m_Roomsession_info[idx]->TRIBE , _info.TRIBE);
		m_Roomsession_info[idx]->SESSION_ID = _info.SESSION_ID;
		m_Roomsession_info[idx]->REAL_PORT = _info.REAL_PORT;
		m_Roomsession_info[idx]->VIRTUAL_PORT = _info.VIRTUAL_PORT;
		m_Roomsession_info[idx]->TEAMNUM = _info.SLOT_IDX;

		if(SCENE_ROOM == CSceneMgr::GetInstance()->GetScene())
		{
			CRoom_UIMgr::GetInstance()->SetUser(m_Roomsession_info[idx]->USER_ID , idx);
			CRoom_UIMgr::GetInstance()->SetCombobox_ChangeData(_info.TRIBE , idx);
			++m_player_cnt;
		}
	}

	printf("%d \n" , m_player_cnt);
}

void CRoomSession_Mgr::Init_RoomUserInfo(const S_PT_ROOM_USER_RENEWAL_SUCC_U& _info)
{
	//_info.TITLE roomUIMGR에서 가져갈 데이터

	m_master_sessionID = _info.MASTERSESSION_ID;
	for(int i = 0; i < 8; ++i)
	{
		if(0 != _info.ROOM_USER_INFO[i].SESSION_ID &&
			NULL == m_Roomsession_info[i])
		{
			m_Roomsession_info[i] = new USER_INFO;
			wcscpy_s(m_Roomsession_info[i]->REAL_ADDRESS , _info.ROOM_USER_INFO[i].REAL_ADDRESS);
			wcscpy_s(m_Roomsession_info[i]->VIRTUAL_ADDRESS , _info.ROOM_USER_INFO[i].VIRTUAL_ADDRESS);
			wcscpy_s(m_Roomsession_info[i]->USER_ID , _info.ROOM_USER_INFO[i].USER_ID);
			wcscpy_s(m_Roomsession_info[i]->TRIBE , _info.ROOM_USER_INFO[i].TRIBE);
			m_Roomsession_info[i]->SESSION_ID = _info.ROOM_USER_INFO[i].SESSION_ID;
			m_Roomsession_info[i]->REAL_PORT = _info.ROOM_USER_INFO[i].REAL_PORT;
			m_Roomsession_info[i]->VIRTUAL_PORT = _info.ROOM_USER_INFO[i].VIRTUAL_PORT;
			m_Roomsession_info[i]->TEAMNUM = _info.ROOM_USER_INFO[i].TEAMNUM;

			if(SCENE_ROOM == CSceneMgr::GetInstance()->GetScene())
			{
				CRoom_UIMgr::GetInstance()->SetUser(m_Roomsession_info[i]->USER_ID , i);
				CRoom_UIMgr::GetInstance()->SetCombobox_ChangeData(_info.ROOM_USER_INFO[i].TRIBE , i);

				++m_player_cnt;
			}
		}
		if(m_myinfo->SESSION_ID == _info.ROOM_USER_INFO[i].SESSION_ID)
			m_curslot = i;
	}
	//printf("%d \n" , m_player_cnt);
}

void CRoomSession_Mgr::SetPlaceData(PLACE_DATA* _data)
{
	memcpy(m_placedata , _data , sizeof(PLACE_DATA)*8);
}

PLACE_DATA* CRoomSession_Mgr::GetPlaceData(void)
{
	return m_placedata;
}
void CRoomSession_Mgr::Release_sessioninfo(void)
{
	for(int i = 0; i < 8; ++i)
	{
		if(NULL != m_Roomsession_info[i])
			Safe_Delete(m_Roomsession_info[i]);
	}
	ZeroMemory(m_placedata , sizeof(m_placedata));
	m_master_sessionID = 0;
	m_curslot = 0;
}


void CRoomSession_Mgr::WriteTurnData(DWORD _protocol , const BYTE* packet , DWORD _length)
{
	CTestSession* psession = CSession_Mgr::GetInstance()->GetUDP_Session();

	const char* szreal_adress;
	for(int i = 0; i < 8; ++i)
	{
		if(NULL == m_Roomsession_info[i])
			continue;
		if(m_myinfo->SESSION_ID == m_Roomsession_info[i]->SESSION_ID)
			continue;

		_bstr_t b(m_Roomsession_info[i]->REAL_ADDRESS);
		szreal_adress = b;

		psession->WriteToPacket(szreal_adress,
			m_Roomsession_info[i]->REAL_PORT,
			_protocol , packet, _length);
	}
}
void CRoomSession_Mgr::WriteAll(DWORD _protocol , const BYTE* packet , DWORD _length)
{
	CTestSession* psession = CSession_Mgr::GetInstance()->GetUDP_Session();

	const char* szreal_adress;
	for(int i = 0; i < 8; ++i)
	{
		if(NULL == m_Roomsession_info[i])
			continue;

		_bstr_t b(m_Roomsession_info[i]->REAL_ADDRESS);
		szreal_adress = b;

		psession->WriteToPacket(szreal_adress,
			m_Roomsession_info[i]->REAL_PORT,
			_protocol , packet, _length);
	}
}

void CRoomSession_Mgr::SetTribe(const WCHAR* _tribe, USHORT idx)
{
	wcscpy_s(m_Roomsession_info[idx]->TRIBE , _tribe); 

	if(m_Roomsession_info[idx]->SESSION_ID == m_myinfo->SESSION_ID)
		wcscpy_s(m_myinfo->TRIBE , _tribe);
}

DWORD_PTR CRoomSession_Mgr::GetMasterSession_ID(void)
{
	return m_master_sessionID;
}