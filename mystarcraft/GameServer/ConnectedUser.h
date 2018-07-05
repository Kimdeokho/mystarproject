#pragma once

class CRoom;

class CConnectedUser : public CPacketSession
{
public:
	CConnectedUser(VOID);
	virtual ~CConnectedUser(VOID);

private:
	WCHAR				mUserID[32];
	
	WCHAR				mVirtualAddress[32];
	WCHAR				mRealAddress[32];

	USHORT				mVirtualPort;
	USHORT				mRealPort;

	USER_STATUS			mStatus;

	CRoom				*mEnteredRoom;

	BOOL				mIsConnected;

	TCHAR				mNickName[32];
	INT64				mGameMoney;
	INT64				mExp;

	CCharacter			mCharacters[3];
	USHORT				mSelectedCharacterSlot;
	
	//////////////////////////////////////////////////////////////////////////
	// 한판마다 리셋 변수
	DWORD				mHP;
	DWORD				mAP;
	BOOL				mIsInCourt;

	BOOL				mIsReady;
	BOOL				mIsLoadComplete;
	BOOL				mIsIntroComplete;
	//////////////////////////////////////////////////////////////////////////

public:
	BOOL				Begin(VOID);
	BOOL				End(VOID);

	BOOL				Reload(SOCKET listenSocket);

	inline BOOL			SetIsConnected(BOOL isConnected){CThreadSync Sync;mIsConnected = isConnected;return TRUE;}
	inline BOOL			GetIsConnected(VOID){CThreadSync Sync;return mIsConnected;}
	inline BOOL			SetIsReady(BOOL isReady){CThreadSync Sync;mIsReady = isReady;return TRUE;}
	inline BOOL			GetIsReady(VOID){CThreadSync Sync;return mIsReady;}
	inline BOOL			SetIsLoadComplete(BOOL isLoadComplete){CThreadSync Sync;mIsLoadComplete = isLoadComplete;return TRUE;}
	inline BOOL			GetIsLoadComplete(VOID){CThreadSync Sync;return mIsLoadComplete;}
	inline BOOL			SetIsIntroComplete(BOOL isIntroComplete){CThreadSync Sync;mIsIntroComplete = isIntroComplete;return TRUE;}
	inline BOOL			GetIsIntroComplete(VOID){CThreadSync Sync;return mIsIntroComplete;}

	inline BOOL			SetVirtualInformation(const WCHAR* virtualAddress, USHORT virtualPort){CThreadSync Sync;if (!virtualAddress || virtualPort <= 0)return FALSE;wcsncpy_s(mVirtualAddress, _countof(mVirtualAddress), virtualAddress, _countof(mVirtualAddress)-1);mVirtualPort = virtualPort;return TRUE;}
	inline BOOL			SetRealInformation(const WCHAR* realAddress, USHORT realPort){CThreadSync Sync;if (!realAddress || realPort <= 0)return FALSE;wcsncpy_s(mRealAddress, _countof(mRealAddress), realAddress, _countof(mRealAddress)-1);mRealPort = realPort;return TRUE;}
	inline BOOL			SetUserID(const WCHAR* userID){CThreadSync Sync;if (!userID)return FALSE;wcsncpy_s(mUserID,_countof(mUserID), userID, _countof(mUserID)-1 );return TRUE;}
	inline BOOL			SetStatus(USER_STATUS status){CThreadSync Sync;mStatus = status;return TRUE;}
	inline USER_STATUS	GetStatus(VOID){CThreadSync Sync;return mStatus;}
	inline BOOL			SetEnteredRoom(CRoom *room){CThreadSync Sync;mEnteredRoom = room;return TRUE;}
	inline CRoom*		GetEnteredRoom(VOID){CThreadSync Sync;return mEnteredRoom;}
	inline const WCHAR*	GetUserID(VOID){CThreadSync Sync;return mUserID;}
	inline const WCHAR*	GetRealAddress(VOID){CThreadSync Sync;return mRealAddress;}
	inline const WCHAR*	GetVirtualAddress(VOID){CThreadSync Sync;return mVirtualAddress;}
	inline USHORT		GetRealPort(VOID){CThreadSync Sync;return mRealPort;}
	inline USHORT		GetVirtualPort(VOID){CThreadSync Sync;return mVirtualPort;}

	//////////////////////////////////////////////////////////////////////////
	// 게임관련 함수들
	inline BOOL			InitializeForGameStart(VOID){CThreadSync Sync;mHP=DEFAULT_MAX_HP;mAP=0;mIsInCourt=TRUE;return TRUE;}

	inline DWORD		GetHP(VOID){CThreadSync Sync;return mHP;}
	inline DWORD		GetAP(VOID){CThreadSync Sync;return mAP;}
	inline BOOL			SetHP(DWORD hp){CThreadSync Sync;mHP=hp;return TRUE;}
	inline BOOL			SetAP(DWORD ap){CThreadSync Sync;mAP=ap;return TRUE;}

	inline BOOL			GetIsInCourt(VOID){CThreadSync Sync;return mIsInCourt;}
	inline BOOL			SetIsInCourt(BOOL isInCourt){CThreadSync Sync;mIsInCourt = isInCourt;return TRUE;}

	inline CCharacter*	GetCharacterObject(USHORT slotIndex){return &mCharacters[slotIndex];}

	inline LPTSTR		GetNickName(VOID){CThreadSync Sync;return mNickName;}
	inline BOOL			SetNickName(LPTSTR nickName){CThreadSync Sync;if (!nickName)return FALSE;_tcsncpy_s(mNickName, _countof(mNickName), nickName, _countof(mNickName) - 1);return TRUE;}
	inline INT64		GetGameMoney(VOID){CThreadSync Sync;return mGameMoney;}
	inline VOID			SetGameMoney(INT64 gameMoney){CThreadSync Sync;mGameMoney = gameMoney;}
	inline INT64		GetExp(VOID){CThreadSync Sync;return mExp;}
	inline VOID			SetExp(INT64 exp){CThreadSync Sync;mExp = exp;}

	inline USHORT		GetSelectedCharacterSlot(VOID){CThreadSync Sync;return mSelectedCharacterSlot;}
	inline VOID			SetSelectedCharacterSlot(USHORT slot){CThreadSync Sync;mSelectedCharacterSlot = slot;}
	//////////////////////////////////////////////////////////////////////////
};
