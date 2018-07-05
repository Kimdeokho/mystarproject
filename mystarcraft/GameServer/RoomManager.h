#pragma once

class CRoomManager : public CMultiThreadSync<CRoomManager>
{
public:
	CRoomManager(VOID);
	virtual ~CRoomManager(VOID);

private:
	DWORD				mMaxRoomCount;
	DWORD				mCurrentRoomCount;

	std::vector<CRoom*>	mRoomVector;
public:
	std::vector<CRoom*>	GetRoomVector(VOID);
public:
	BOOL	Begin(DWORD maxRoomCount);
	BOOL	End(VOID);

	CRoom*	QuickJoin(CConnectedUser *connectedUser, USHORT &slotIndex);
	CRoom*	GetRemaining_Room(VOID);

	BOOL	CheckGameTime(CGameIocp *iocp);
};
