#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"

#include "../Packet/ErrorCode.h"
#include "../Packet/TcpProtocol.h"
#include "../Packet/Tcp_Structure.h"
#include "../Packet/Tcp_ReadPacket.h"
#include "../Packet/Tcp_WritePacket.h"

#include "Character.h"
#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

VOID CGameIocp::onPT_LOBY_ENTER(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	//CHECK_USER_STATUS(!= US_NONE, PT_CHANNEL_ENTER_FAIL_U);

	connectedUser->SetStatus(US_LOBY_ENTERING);

	READ_PACKET(PT_LOBY_ENTER);
	CLog::WriteLog(_T("# Read packet : PT_LOBY_ENTER %s/%s/%d\n"), Data.USER_ID,
		Data.VIRTUAL_ADDRESS, Data.VIRTUAL_PORT);


	Data.USER_ID[sizeof(Data.USER_ID)-1]					= NULL;
	Data.VIRTUAL_ADDRESS[sizeof(Data.VIRTUAL_ADDRESS)-1]	= NULL;

	if (!mConnectedUserManager.IsAlreadyLogined(Data.USER_ID))
	{
		connectedUser->SetUserID(Data.USER_ID);
		connectedUser->SetVirtualInformation(Data.VIRTUAL_ADDRESS, Data.VIRTUAL_PORT);

		DWORD_PTR	SessionID	= (DWORD_PTR)(connectedUser);

		connectedUser->WritePacket(PT_LOBY_ENTER_SUCC_U,
			WriteBuffer,
			WRITE_PT_LOBY_ENTER_SUCC_U(WriteBuffer, 
			connectedUser->GetUserID(),
			SessionID,
			connectedUser->GetVirtualAddress(),
			connectedUser->GetVirtualPort(),
			connectedUser->GetRealAddress(),
			connectedUser->GetRealPort()
			));

		connectedUser->SetStatus(US_LOBY_ENTERED);

		CLog::WriteLog(_T("# Write packet : PT_LOBY_ENTER_SUCC_U 0x%x/%s/%d/%s/%d\n"), SessionID,
			connectedUser->GetVirtualAddress(),
			connectedUser->GetVirtualPort(),
			connectedUser->GetRealAddress(),
			connectedUser->GetRealPort());
	}
	else
	{
		//WRITE_FAIL_U_PACKET(PT_CHANNEL_ENTER_FAIL_U, EC_USER_ALREADY_LOGINED);

		CLog::WriteLog(_T("# ALREADYLOG \n"));
		connectedUser->SetStatus(US_NONE);
	}
}
VOID CGameIocp::onPT_ROOMLIST_RENEWAL(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	READ_PACKET(PT_ROOMLIST_RENEWAL);
	CLog::WriteLog(_T("# Read packet : PT_ROOMLIST_RENEWAL\n"));

	std::vector<CRoom*>	roomvec = mRoomManager.GetRoomVector();
	for(DWORD i = 0; i < roomvec.size(); ++i)
	{			
		if(!roomvec[i]->GetIsEmpty())
		{
			connectedUser->WritePacket(PT_ROOMLIST_RENEWAL_M , WriteBuffer 
				, WRITE_PT_ROOMLIST_RENEWAL_M(WriteBuffer , roomvec[i]->GetTitle() , 
				(DWORD_PTR)roomvec[i]->GetRootUser() , (USHORT)i));
		}
	}
}
VOID CGameIocp::onPT_CREATE_ROOM(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	READ_PACKET(PT_CREATE_ROOM);
	CLog::WriteLog(_T("# Read packet : PT_CREATE_ROOM %s\n"), Data.TITLE);
	connectedUser->SetStatus(US_ROOM_ENTERED);

	USHORT	slotidx = 0;
	USHORT	room_idx = 0;
	CRoom*	proom;
	std::vector<CRoom*>	roomvec = mRoomManager.GetRoomVector();
	for(DWORD i = 0; i < roomvec.size(); ++i)
	{
		if( TRUE == roomvec[i]->GetIsEmpty() )
		{
			proom = roomvec[i];
			proom->SetTitle(Data.TITLE);
			proom->JoinUser(connectedUser , slotidx);
			room_idx = (USHORT)i;
			break;
		}
	}

	connectedUser->WritePacket(PT_CREATE_ROOM_SUCC_U , WriteBuffer,
		WRITE_PT_CREATE_ROOM_SUCC_U(WriteBuffer, slotidx , Data.TITLE));

	mConnectedUserManager.WriteAll(PT_ROOMLIST_RENEWAL_M , WriteBuffer 
		, WRITE_PT_ROOMLIST_RENEWAL_M(WriteBuffer , Data.TITLE , (DWORD_PTR)connectedUser , room_idx));
}
VOID CGameIocp::onPT_ROOM_ENTER(CConnectedUser* connectedUser, BYTE *packet)
{

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	READ_PACKET(PT_ROOM_ENTER);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_ENTER %d\n"), Data.ROOM_IDX);
	connectedUser->SetStatus(US_ROOM_ENTERED);

	std::vector<CRoom*> proomvec = mRoomManager.GetRoomVector();

	CRoom* proom = proomvec[Data.ROOM_IDX];
	USHORT	slotidx = 0;
	proom->JoinUser(connectedUser , slotidx);
	connectedUser->SetSelectedCharacterSlot(slotidx);

	connectedUser->WritePacket(PT_ROOM_ENTER_SUCC_U , WriteBuffer ,
		WRITE_PT_ROOM_ENTER_SUCC_U(WriteBuffer , 
		proom->GetTitle() , DWORD_PTR(proom->GetRootUser()) ));

	CLog::WriteLog(_T("# Write packet : PT_ROOM_ENTER_SUCC_U \n"));
}
VOID CGameIocp::onPT_ROOM_USER_RENEWAL(CConnectedUser* connectedUser, BYTE *packet)
{
	//콤보박스에서 종족을 받아온다.
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
	READ_PACKET(PT_ROOM_USER_RENEWAL);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_USER_RENWAL %s\n") , Data.TRIBE);

	connectedUser->SetTribe(Data.TRIBE);
	CRoom* proom = connectedUser->GetEnteredRoom();
	USHORT	slotidx = connectedUser->GetSelectedCharacterSlot();

	//방에 들어와 있는 사람에게 들어온 유저 정보 뿌려주기
	proom->WriteAll(PT_ROOM_USER_ENTRY_M , WriteBuffer,
		WRITE_PT_ROOM_USER_ENTRY_M(WriteBuffer ,
		connectedUser->GetUserID(),
		(DWORD_PTR)connectedUser,
		connectedUser->GetVirtualAddress(),
		connectedUser->GetVirtualPort(),
		connectedUser->GetRealAddress(),
		connectedUser->GetRealPort(),
		slotidx,
		connectedUser->GetTribe()));

	CLog::WriteLog(_T("# Write packet : PT_ROOM_USER_ENTRY %s , %d , %s , %d , %s , %d , %d, %s\n"), 
		connectedUser->GetUserID(),
		(DWORD_PTR)connectedUser,
		connectedUser->GetVirtualAddress(),
		connectedUser->GetVirtualPort(),
		connectedUser->GetRealAddress(),
		connectedUser->GetRealPort(),
		slotidx,
		connectedUser->GetTribe());

	//방에 들어와있는 사람들의 정보 받기
	CConnectedUser** userlist = proom->GetRoomUser();
	SLOT_USER_DATA temp[8];

	for(int i = 0; i < 8; ++i)
	{
		if(NULL == userlist[i])
			continue;

		wcscpy_s(temp[i].REAL_ADDRESS , userlist[i]->GetRealAddress());
		wcscpy_s(temp[i].VIRTUAL_ADDRESS , userlist[i]->GetVirtualAddress());
		wcscpy_s(temp[i].USER_ID , userlist[i]->GetUserID());
		wcscpy_s(temp[i].TRIBE , userlist[i]->GetTribe());
		temp[i].REAL_PORT = userlist[i]->GetRealPort();
		temp[i].VIRTUAL_PORT = userlist[i]->GetVirtualPort();
		temp[i].SESSION_ID = DWORD_PTR(userlist[i]);
		temp[i].TEAMNUM = i;

	}

	connectedUser->WritePacket(PT_ROOM_USER_RENEWAL_SUCC_U , WriteBuffer ,
		WRITE_PT_ROOM_USER_RENEWAL_SUCC_U(WriteBuffer , 
		temp , DWORD_PTR(proom->GetRootUser()) ));
}
VOID CGameIocp::onPT_ROOM_GET_MASTER_INFO(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
	READ_PACKET(PT_ROOM_GET_MASTER_INFO);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_GET_MASTER_INFO roomidx = %d\n") , Data.ROOM_IDX);

	std::vector<CRoom*> proomvec = mRoomManager.GetRoomVector();
	CRoom* proom = proomvec[Data.ROOM_IDX];

	connectedUser->WritePacket(PT_ROOM_GET_MASTER_INFO_SUCC_U , WriteBuffer ,
		WRITE_PT_ROOM_GET_MASTER_INFO_SUCC_U(WriteBuffer , 
		DWORD_PTR(proom->GetRootUser()) ));
}