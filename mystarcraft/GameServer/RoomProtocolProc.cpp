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

VOID CGameIocp::onPT_ROOM_LEAVE(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	connectedUser->SetStatus(US_LOBY_ENTERED);

	READ_PACKET(PT_ROOM_LEAVE);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_LEAVE\n"));

	CRoom* proom = connectedUser->GetEnteredRoom();
	proom->LeaveUser(FALSE , NULL , connectedUser);

	if(proom->GetIsEmpty())
	{
		//룸리스트 제거
		//proom->GetIndex();
		mConnectedUserManager.WriteAll(PT_ROOMLIST_ERASE_M
			, WriteBuffer , WRITE_PT_ROOMLIST_ERASE_M(WriteBuffer
			, (USHORT)(proom->GetIndex()) ));
	}
	//connectedUser->WritePacket()
}
VOID CGameIocp::onPT_ROOM_SENDCHAT(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	READ_PACKET(PT_ROOM_SENDCHAT);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_SENDCHAT %s , %s , %d\n") 
		, Data.MESSAGE , Data.USER_ID , Data.SESSION_ID);

	CRoom* proom = connectedUser->GetEnteredRoom();
	if(NULL != proom)
	{
		proom->WriteAll(PT_ROOM_RECEIVE_CHAT_M , WriteBuffer , 
			WRITE_PT_ROOM_RECEIVE_CHAT_M(WriteBuffer , 
			Data.USER_ID,
			Data.MESSAGE,			
			Data.SESSION_ID,
			Data.CUR_TIME,
			Data.RETAIN_TIME));
	}
}
VOID CGameIocp::onPT_ROOM_TRIBE_CHANGE(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
	READ_PACKET(PT_ROOM_TRIBE_CHANGE);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_RACE_CHANGE %s , %d\n")
		, Data.TRIBE , Data.SLOTIDX);

	connectedUser->SetTribe(Data.TRIBE);
	CRoom* proom = connectedUser->GetEnteredRoom();
	if(NULL != proom)
	{
		proom->WriteAll(PT_ROOM_TRIBE_CHANGE_M , WriteBuffer , 
			WRITE_PT_ROOM_TRIBE_CHANGE_M(WriteBuffer , 
			Data.TRIBE,
			Data.SLOTIDX));
	}
}
VOID CGameIocp::onPT_LOAD_COMPLETE(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
	READ_PACKET(PT_LOAD_COMPLETE);
	CLog::WriteLog(_T("# Read packet : PT_LOAD_COMPLETE \n"));


	CRoom* proom = connectedUser->GetEnteredRoom();
	proom->SetLoadComplete();
	if(TRUE == proom->IsAllLoadComplete())
	{
		proom->WriteAll(PT_LOAD_COMPLETE_SUCC_M , WriteBuffer , 
			WRITE_PT_LOAD_COMPLETE_SUCC_M(WriteBuffer));
	}
}