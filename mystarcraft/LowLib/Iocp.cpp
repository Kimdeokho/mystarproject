#include "stdafx.h"
#include "Iocp.h"

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	CIocp *Owner = (CIocp*) parameter;
	Owner->WorkerThreadCallback();

	return 0;
}

CIocp::CIocp(VOID)
{
	mIocpHandle			= NULL;
	mWorkerThreadCount	= 0;

	mStartupEventHandle	= NULL;
}

CIocp::~CIocp(VOID)
{
}

BOOL CIocp::Begin(VOID)
{
	mIocpHandle			= NULL;

	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	mWorkerThreadCount	= SystemInfo.dwNumberOfProcessors * 2;
	mIocpHandle			= CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0); //오브젝트 생성

	if (!mIocpHandle)
		return FALSE;

	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0); //nonsignaled 상태, auto-reset모드
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	for (DWORD i=0;i<mWorkerThreadCount;i++)
	{
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mWorkerThreadVector.push_back(WorkerThread);

		WaitForSingleObject(mStartupEventHandle, INFINITE);
	}

	return TRUE;
}

BOOL CIocp::End(VOID)
{
	for (DWORD i=0;i<mWorkerThreadVector.size();i++)
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);

	for (DWORD i=0;i<mWorkerThreadVector.size();i++)
	{
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);

		CloseHandle(mWorkerThreadVector[i]);
	}

	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	mWorkerThreadVector.clear();

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

BOOL CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	if (!socket || !completionKey)
		return FALSE;

	mIocpHandle = CreateIoCompletionPort((HANDLE) socket, mIocpHandle, completionKey, 0); //socket과 iocphandle연결

	if (!mIocpHandle)
		return FALSE;

	return TRUE;
}

VOID CIocp::WorkerThreadCallback(VOID)
{
	BOOL			Successed				= FALSE;
	DWORD			NumberOfByteTransfered	= 0;
	VOID			*CompletionKey			= NULL;
	OVERLAPPED		*Overlapped				= NULL;
	OVERLAPPED_EX	*OverlappedEx			= NULL;
	VOID			*Object					= NULL;

	while (TRUE)
	{
		SetEvent(mStartupEventHandle); //event 오브젝트를 signaled 상태로 변경

		Successed = GetQueuedCompletionStatus(
			mIocpHandle,
			&NumberOfByteTransfered,
			(LPDWORD) &CompletionKey,
			&Overlapped,
			INFINITE); //입출력이 완료될때까지 블로킹상태에 놓인다.

		if (!CompletionKey)
			return;

		OverlappedEx	= (OVERLAPPED_EX*) Overlapped;
		Object			= OverlappedEx->Object;

		if (!Successed || (Successed && !NumberOfByteTransfered))
		{
			//connect 메시지가 발생하는 key는 CGameIocp::Begin에서 등록한다
			if (OverlappedEx->IoType == IO_ACCEPT)
				OnIoConnected(Object);
			else
				OnIoDisconnected(Object);

			continue;
		}

		switch (OverlappedEx->IoType)
		{
		case IO_READ:
			OnIoRead(Object, NumberOfByteTransfered);
			break;

		case IO_WRITE:
			OnIoWrote(Object, NumberOfByteTransfered);
			break;
		}
	}
}