#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"

//#include "../Packet/ErrorCode.h"
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

#include "../Packet/ErrorCode.h"

CRoom::CRoom(VOID)
{
	mIndex						= 0;
	mIsRoomStarted				= FALSE;
	mMapIndex					= 0;
	mCurrentUserCount			= 0;
	mRootUser					= NULL;
	mIsGameStarted				= FALSE;

	mRemainGameTime				= DEFAULT_GAMETIME;

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	ZeroMemory(mTitle, sizeof(mTitle));
	ZeroMemory(mRoomUser, sizeof(mRoomUser));
}

CRoom::~CRoom(VOID)
{
}

BOOL CRoom::Begin(DWORD index)
{
	CThreadSync Sync;

	mIndex						= index;
	mIsRoomStarted				= FALSE;
	mMapIndex					= 0;
	mCurrentUserCount			= 0;
	mRootUser					= NULL;
	mIsGameStarted				= FALSE;

	mRemainGameTime				= DEFAULT_GAMETIME;

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	ZeroMemory(mTitle, sizeof(mTitle));
	ZeroMemory(mRoomUser, sizeof(mRoomUser));

	return TRUE;
}

BOOL CRoom::End(VOID)
{
	CThreadSync Sync;

	mIndex						= 0;
	mIsRoomStarted				= FALSE;
	mMapIndex					= 0;
	mCurrentUserCount			= 0;
	mRootUser					= NULL;
	mIsGameStarted				= FALSE;

	mRemainGameTime				= DEFAULT_GAMETIME;

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;	
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	ZeroMemory(mTitle, sizeof(mTitle));
	ZeroMemory(mRoomUser, sizeof(mRoomUser));

	return TRUE;
}

BOOL CRoom::JoinUser(CConnectedUser *connectedUser, USHORT &slotIndex)
{
	CThreadSync Sync;

	if (!connectedUser)
		return FALSE;

	for (USHORT i= 0;i<8;i++)
	{
		// 편에 따른 위치 선정이 필요하다.
		// 빈자리를 찾아준다.

		if (mRoomUser[i] == NULL)
		{
			mRoomUser[i] = connectedUser;
			connectedUser->SetEnteredRoom(this);

			mCurrentUserCount = min(SHORT(mCurrentUserCount++), 8);

			slotIndex = i;

			// 방 처음 생성
			if (mCurrentUserCount == 1)
			{
				mRootUser = connectedUser;
				mMapIndex = 0;
			}

			return TRUE;
		}
	}

	//// 편이 맞는지 확인한다.
	//USHORT	RedTeamCount	= 0;
	//USHORT	BlueTeamCount	= 0;

	//for (USHORT i=0;i<8;i++)
	//{
	//	// 해당 사용자를 찾는다.
	//	if (mRoomUser[i] && i < 4)
	//		RedTeamCount++;
	//	else if (mRoomUser[i] && i >= 4)
	//		BlueTeamCount++;
	//}

	//// FULL
	//if (RedTeamCount + BlueTeamCount == 8)
	//	return FALSE;

	//USHORT StartPos = RedTeamCount <= BlueTeamCount ? 0 : 4;

	//for (USHORT i=StartPos;i<8;i++)
	//{
	//	// 편에 따른 위치 선정이 필요하다.
	//	// 빈자리를 찾아준다.

	//	if (mRoomUser[i] == NULL)
	//	{
	//		mRoomUser[i] = connectedUser;
	//		connectedUser->SetEnteredRoom(this);

	//		mCurrentUserCount = min(SHORT(mCurrentUserCount++), 8);

	//		slotIndex = i;

	//		// 방 처음 생성
	//		if (mCurrentUserCount == 1)
	//		{
	//			mRootUser = connectedUser;
	//			mMapIndex = 0;
	//		}

	//		return TRUE;
	//	}
	//}

	return FALSE;
}

BOOL CRoom::LeaveUser(BOOL isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	if (!connectedUser)
		return FALSE;

	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i] == connectedUser)
		{
			mRoomUser[i] = NULL;
			connectedUser->SetEnteredRoom(NULL);

			mCurrentUserCount	= max(SHORT(mCurrentUserCount--), 0);

			if (connectedUser == mRootUser)
			{
				mRootUser			= NULL;

				for (USHORT j=0;j<8;j++)
				{
					if (mRoomUser[j])
					{
						mRootUser = mRoomUser[j];

						break;
					}
				}
			}

			if (!isDisconnected)
			{
				// 방 나가기 성공을 보내고
				connectedUser->WritePacket(PT_ROOM_LEAVE_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_ROOM_LEAVE_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_ROOM_LEAVE_SUCC_U\n"));
			}

			//방안에 있는 유저들에게 유저가 나갔다고 메시지를 보낸다
			WriteAll(PT_ROOM_LEAVE_M, 
				WriteBuffer, 
				WRITE_PT_ROOM_LEAVE_M(WriteBuffer, 
				(DWORD_PTR) connectedUser, 
				(DWORD_PTR) mRootUser));

			CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_LEAVE_M 0x%x/0x%x\n"), 
				(DWORD_PTR) connectedUser, 
				(DWORD_PTR) mRootUser);

			if (mIsRoomStarted)
			{
				// Game End를 시켜준다.
				GameEnd(iocp);

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_END_M\n"));
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRoom::WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (protocol <= 0 || !packet)
		return FALSE;

	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i])
			mRoomUser[i]->WritePacket(protocol, packet, packetLength);
	}

	return TRUE;
}

BOOL CRoom::WriteAllInitHPAP(VOID)
{
	CThreadSync Sync;

	BYTE		WriteBuffer[MAX_BUFFER_LENGTH] = {0,};


	return TRUE;
}

DWORD CRoom::RoomStart(VOID)
{
	CThreadSync Sync;
#ifdef _CHECK_MODULE
	// 사람들이 전부 Ready인지 확인한다.
	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i])
		{
			if (!mRoomUser[i]->GetIsReady())
				return EC_ROOM_START_FAIL_ALL_READY;
		}
	}

	// 사용자가 짝수인지 확인한다.
	if (mCurrentUserCount % 2 != 0)
		return EC_ROOM_START_FAIL_TEAM_INCORRECT;

	// 편이 맞는지 확인한다.
	USHORT	RedTeamCount	= 0;
	USHORT	BlueTeamCount	= 0;

	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i] && i < 4)
			RedTeamCount++;
		else if (mRoomUser[i] && i >= 4)
			BlueTeamCount++;
	}

	if (RedTeamCount != BlueTeamCount)
		return EC_ROOM_START_FAIL_TEAM_INCORRECT;
#endif
	mIsRoomStarted = TRUE;

	// 모든 사람의 상태를 변화해 준다.
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			mRoomUser[i]->SetStatus(US_GAME_STARTING);
			mRoomUser[i]->SetIsReady(FALSE);
		}
	}

	return 0;
}

BOOL CRoom::GameStart(VOID)
{
	CThreadSync Sync;

	// 모두 IntroComplete인지 확인한다.
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (!mRoomUser[i]->GetIsIntroComplete())
				return FALSE;
		}
	}

	mIsGameStarted		= TRUE;
	mRemainGameTime		= DEFAULT_GAMETIME;

	// 모두일 경우 TRUE를 리턴 해 주고 LoadComplete 초기화
	// 모든 사용자 게임상태로 변경
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			mRoomUser[i]->SetStatus(US_GAME_STARTED);
			mRoomUser[i]->SetIsLoadComplete(FALSE);
			mRoomUser[i]->SetIsIntroComplete(FALSE);
			mRoomUser[i]->InitializeForGameStart();
		}
	}

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	return TRUE;
}

BOOL CRoom::GameEnd(CGameIocp *iocp)
{
	CThreadSync Sync;

	// 승패 관련 처리

	// 모든 룸에 사용자의 HP를 더해서 많은쪽이 이긴거다
	// 혹시나 같으면 무승부

	return TRUE;
}

BOOL CRoom::IsSameTeam(CConnectedUser *player1, CConnectedUser *player2)
{
	CThreadSync Sync;


	return FALSE;
}

DWORD CRoom::GetTeam(CConnectedUser *player)
{
	CThreadSync Sync;


	return 0;
}

BOOL CRoom::IsAllOut(CConnectedUser *player)
{
	CThreadSync Sync;


	return TRUE;
}

VOID CRoom::PassInfo(CConnectedUser *shooter, CConnectedUser *target)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

}

VOID CRoom::PassReceiveInfo(CConnectedUser *shooter, CConnectedUser *target)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 마지막 패서 확인
	//if (GetLastPassUser() == shooter)
}

VOID CRoom::ShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};


}

VOID CRoom::ShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

}

VOID CRoom::ShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power, CGameIocp *gameIocp)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 슈터를 확인해 주고, 맞는 사람도 확인해 준다.(혹시나) 코트안에 있는지 확인
	//if (shooter == GetLastShootUser())
	//{
	
}

VOID CRoom::SpecialShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	
}

VOID CRoom::SpecialShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 마지막 슈터 확인
	//if (GetLastShootUser() == shooter)
	//{

}

VOID CRoom::SpecialShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType, CGameIocp *gameIocp)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 슈터를 확인해 주고, 맞는 사람도 확인해 준다.(혹시나) 코트안에 있는지 확인
	//if (shooter == GetLastShootUser())
	//{
	//if (GetBallUser())
	//{
	
}

VOID CRoom::FieldBall(CConnectedUser *user)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};


}

VOID CRoom::GetFieldBall(CConnectedUser *user, DWORD parameter)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 현재 소유권이 없는지 확인
	
}

BOOL CRoom::IsAllLoadComplete(VOID)
{
	CThreadSync Sync;


	return TRUE;
}

BOOL CRoom::IsAllIntroComplete(VOID)
{
	CThreadSync Sync;

	return TRUE;
}

FLOAT CRoom::GetAdVantage(CConnectedUser *user)
{
	CThreadSync Sync;

	if (!user)
		return FALSE;

	return 1.0F;
}

VOID CRoom::SkillInfo(CConnectedUser *actor, CConnectedUser *target, USHORT skillType)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 슛 상대가 코트밖인지 안인지 확인

}

VOID CRoom::CheckExplosion(CGameIocp *gameIocp)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
}