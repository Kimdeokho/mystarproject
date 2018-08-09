#include "StdAfx.h"
#include "Session_Mgr.h"

#include "TestSession.h"
#include "SceneMgr.h"
#include "Login_UIMgr.h"
#include "Loby_UIMgr.h"
#include "Room_UIMgr.h"
#include "RoomSession_Mgr.h"

#include "KeyMgr.h"
#include "Input_Interface.h"
#include "Input_Stage.h"
#include "TurnData.h"
#include "MyCommandList.h"

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

	m_subTurnNumber = 0;
	m_TurnNumber = -2;
	m_startstage = false;

	m_vecTurnData.resize(100000);

	m_estate = NS_NONE;
}
void CSession_Mgr::Update(void)
{
	Read_UDPPacket();
	Read_TCPPacket();
	SendTurnPacket();
}
void CSession_Mgr::SendTurnPacket(void)
{
	//if(!m_startstage)
	//	return;

	if(NS_PLAYING != m_estate)
		return;

	m_subTurnNumber++;
	if ( m_subTurnNumber == 6 )
	{
		CInput_Stage* pinput = ((CInput_Stage*)CKeyMgr::GetInstance()->GetInputDevice());
		CMyCommandList* pcmdlist = pinput->GetCmdList();

		CTurnData* pturndata = new CTurnData(pcmdlist);				

		//TurnNumber | SessionID | CmdSize(커맨드 갯수) | CmdType | 커맨드 데이터들
		CStreamSP packet;
		packet->SetBuffer(WriteBuffer);
		packet->WriteInt32(m_TurnNumber + 2);
		packet->WriteDWORD_PTR(m_session_info.SESSION_ID);

		pturndata->Write(packet);

		CRoomSession_Mgr::GetInstance()->WriteTurnData(PU_TURN_DATA ,WriteBuffer ,packet->GetLength());
		//보내고

		m_vecTurnData[m_TurnNumber + 2].insert(pair<DWORD_PTR , CTurnData*>(m_session_info.SESSION_ID , pturndata));
		pinput->ClearCmdList();

		if(m_TurnNumber >= 0)
		{
			TryAdvanceTurn();
		}
		else
		{
			++m_TurnNumber;
			m_subTurnNumber = 0;
		}
	}
}
void CSession_Mgr::TryAdvanceTurn(void)
{
	int playercnt = CRoomSession_Mgr::GetInstance()->GetPlayerCnt();
	if(m_vecTurnData[m_TurnNumber + 1].size() == playercnt)
	{
		if(NS_DELAY == m_estate)
		{
			m_estate = NS_PLAYING;

			CInput_Stage* pinput = ((CInput_Stage*)CKeyMgr::GetInstance()->GetInputDevice());
			CMyCommandList* pcmdlist = pinput->GetCmdList();
			pcmdlist->ClearCommand();

			Sleep(100);
		}
		++m_TurnNumber;
		m_subTurnNumber = 0;

		map<DWORD_PTR , CTurnData*>::iterator iter = m_vecTurnData[m_TurnNumber].begin();
		map<DWORD_PTR , CTurnData*>::iterator iter_end = m_vecTurnData[m_TurnNumber].end();

		for( ; iter != iter_end; ++iter)
		{
			CTurnData* temp = iter->second;

			iter->second->GetCmdList()->ProcessCmd();
		}
		//동기화 전, 승패가 결정났는지 한번 보자.
	}
	else
		m_estate = NS_DELAY;
}
void CSession_Mgr::Read_UDPPacket(void)
{
	if(NULL == m_UDPsession)
		return;

	// 패킷을 받을때 사용하는 프로토콜, 길이, 패킷 변수
	DWORD dwProtocol = 0, dwPacketLength = 0;
	BYTE Packet[MAX_BUFFER_LENGTH] = {0,};

	char tempaddres[32] = "";
	USHORT tempport = 0;

	if(m_UDPsession->ReadFromPacket(dwProtocol , tempaddres , tempport ,m_Read_UDPBuf , dwPacketLength ))
	{
		if(PU_TEST == dwProtocol)
		{
			int a = 0;
		}
		else if(PU_START_ARRANGEMENT == dwProtocol)
		{
			READ_UDP_PACKET(PU_START_ARRANGEMENT);
			CRoomSession_Mgr::GetInstance()->SetPlaceData(Data.PLACE_INFO);
			CRoom_UIMgr::GetInstance()->Exit(R_STAGE_PRE);
		}
		else if(PU_INIT_GAME == dwProtocol)
		{
			READ_UDP_PACKET(PU_INIT_GAME);
			
			srand(unsigned(Data.SEED));
			CRoomSession_Mgr::GetInstance()->SetPlayerCnt(Data.PLAYER_CNT);
			//방에서 스타트버튼을 눌렀다.
		}
		else if(PU_TURN_DATA == dwProtocol)
		{
			//READ_UDP_PACKET(PU_TURN_DATA);

			CStreamSP readstream;
			readstream->SetBuffer(m_Read_UDPBuf);

			if(NS_DELAY == m_estate)
			{
				HandlerTurnPacket(readstream);

				TryAdvanceTurn();
			}
			else
				HandlerTurnPacket(readstream);

			//m_Read_UDPBuf 여기에 턴데이터 정보가 담겨있다.
		}
	}
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

				CRoomSession_Mgr::GetInstance()->Initialize();
				//마스터세션이랑, 타이틀 받고
				CRoomSession_Mgr::GetInstance()->SetMasterSession_ID(m_session_info.SESSION_ID);
				CRoomSession_Mgr::GetInstance()->SetTitle(Data.TITLE);
				
				CLoby_UIMgr::GetInstance()->Exit(LB_ROOM_CREATE);
				CLoby_UIMgr::GetInstance()->Room_Popup(false);
				//CRoom_UIMgr::GetInstance()->SetRoomTitle(Data.TITLE);
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
			else if(PT_ROOM_GET_MASTER_INFO_SUCC_U == dwProtocol)
			{
				READ_TCP_PACKET(PT_ROOM_GET_MASTER_INFO_SUCC_U);

				CRoomSession_Mgr::GetInstance()->Initialize();
				CRoomSession_Mgr::GetInstance()->SetMasterSession_ID(Data.MASTERSESSION_ID);
				CLoby_UIMgr::GetInstance()->Exit(LB_ROOM_JOIN);
				CLoby_UIMgr::GetInstance()->Room_Popup(false);
			}
			else if(PT_ROOM_ENTER_SUCC_U == dwProtocol)
			{				
				READ_TCP_PACKET(PT_ROOM_ENTER_SUCC_U);
				CRoom_UIMgr::GetInstance()->SetRoomTitle(Data.TITLE);				

			}
			else if(PT_ROOM_USER_RENEWAL_SUCC_U == dwProtocol)
			{
				//방에 이미 들어와있는 유저들의 정보받기
				READ_TCP_PACKET(PT_ROOM_USER_RENEWAL_SUCC_U);
				CRoomSession_Mgr::GetInstance()->Init_RoomUserInfo(Data);				
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
			else if(PT_ROOM_TRIBE_CHANGE_M == dwProtocol)
			{
				READ_TCP_PACKET(PT_ROOM_TRIBE_CHANGE_M);
				//누군가가 콤보박스 변경했다
				CRoom_UIMgr::GetInstance()->SetCombobox_ChangeData(Data.TRIBE , Data.SLOTIDX);
				CRoomSession_Mgr::GetInstance()->SetTribe(Data.TRIBE , Data.SLOTIDX);
			}
			else if(PT_LOAD_COMPLETE_SUCC_M == dwProtocol)
			{
				READ_TCP_PACKET(PT_LOAD_COMPLETE_SUCC_M);
				CSceneMgr::GetInstance()->SetScene(SCENE_STAGE);
				//m_startstage = true;
				m_estate = NS_PLAYING;
			}
		}
		else
			break;
	}
}
void CSession_Mgr::HandlerTurnPacket(CStreamSP& readpacket)
{
	DWORD_PTR session_id = 0;
	int		turn_number = 0;
	CTurnData*	turndata = new CTurnData;
	readpacket->ReadInt32(&turn_number);
	readpacket->ReadDWORD_PTR(&session_id);

	turndata->Read(readpacket);
	m_vecTurnData[turn_number].insert(pair<DWORD_PTR , CTurnData*>(session_id , turndata));
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


void CSession_Mgr::Write_TCP_Packet(DWORD PT , BYTE* packet, DWORD Length)
{
	m_TCPsession->WritePacket(PT , packet , Length);
}

SESSION_INFO CSession_Mgr::GetSession_Info(void)
{
	return m_session_info;
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

	map<DWORD_PTR , CTurnData*>::iterator iter;
	map<DWORD_PTR , CTurnData*>::iterator iter_end;
	DWORD vecsize = m_vecTurnData.size();
	for(DWORD i = 0; i < vecsize; ++i)
	{
		iter = m_vecTurnData[i].begin();
		iter_end = m_vecTurnData[i].end();

		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete(iter->second);
		}
		m_vecTurnData[i].clear();
	}
	m_vecTurnData.clear();
	vector<map<DWORD_PTR , CTurnData*>>().swap(m_vecTurnData);
}