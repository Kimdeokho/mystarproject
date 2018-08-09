#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"

DWORD WINAPI ReliableUdpThreadCallback(LPVOID parameter)
{
	CNetworkSession *Owner = (CNetworkSession*) parameter;
	Owner->ReliableUdpThreadCallback();

	return 0;
}

CNetworkSession::CNetworkSession(VOID)
{
	memset(&mAcceptOverlapped, 0, sizeof(mAcceptOverlapped));
	memset(&mReadOverlapped, 0, sizeof(mReadOverlapped));
	memset(&mWriteOverlapped, 0, sizeof(mWriteOverlapped));

	memset(mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	mSocket							= NULL;
	mReliableUdpThreadHandle		= NULL;
	mReliableUdpThreadStartupEvent	= NULL;
	mReliableUdpThreadDestroyEvent	= NULL;
	mReliableUdpThreadWakeUpEvent	= NULL;
	mReliableUdpWriteCompleteEvent	= NULL;

	mIsReliableUdpSending			= FALSE;

	mAcceptOverlapped.IoType		= IO_ACCEPT;
	mReadOverlapped.IoType			= IO_READ;
	mWriteOverlapped.IoType			= IO_WRITE;

	mAcceptOverlapped.Object		= this;
	mReadOverlapped.Object			= this;
	mWriteOverlapped.Object			= this;
}

CNetworkSession::~CNetworkSession(VOID)
{

}

VOID CNetworkSession::ReliableUdpThreadCallback(VOID)
{
	DWORD				EventID					= 0;
	HANDLE				ThreadEvents[2]			= {mReliableUdpThreadDestroyEvent, mReliableUdpThreadWakeUpEvent};

	CHAR				RemoteAddress[32]		= {0,};
	USHORT				RemotePort				= 0;
	BYTE				Data[MAX_BUFFER_LENGTH]	= {0,};
	DWORD				DataLength				= 0;
	VOID				*Object					= NULL;

	/*A호스트에서 udp로 writeto를 이용하여 패킷을 보낼경우
	ReliableUdpThreadCallback을 깨운다*/
	while (TRUE)
	{
		SetEvent(mReliableUdpThreadStartupEvent);

        EventID = WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			return;

		case WAIT_OBJECT_0 + 1: //2
NEXT_DATA:
			// mReliableUdpThreadWakeUpEvent ,Write 일 경우 Pop을 해주고
			if (mReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort))
			{
				// 데이터가 있을 경우
				// 실제 Write를 해 주고 WaitForSingleObject를 해 준다.
				// 받았을때 그 SetEvent를 해 주면 풀린다.
RETRY:
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength))
					return;

				/*상대가 ACK9999 = 패킷을 받았다는 응답을 할경우 
				mReliableUdpWriteCompleteEvent가 시그널되며 NEXT_DATA를 호출*/
				DWORD Result = WaitForSingleObject(mReliableUdpWriteCompleteEvent, 10);

				// 받았을 경우 다음 데이터 처리
				if (Result == WAIT_OBJECT_0)
					goto NEXT_DATA;
				else
					goto RETRY;
			}
			else
				mIsReliableUdpSending = FALSE;

			break;
		}
	}

	//A호스트 , B호스트가 있다고하자
	/*
		A호스트 측에서 B호스트 측으로 패킷을 보낼경우
		B호스트는 A호스트가 보낸 패킷을 Read하여 ACK9999를 보내게 되어잇다
		이때 A호스트는 B호스트로부터 ACK9999를 받기전까지 다음 메세지를 보내지 않고 대기한다
		ACK9999를 받게되면 다음메세지를 보낼 준비가 완료 된다.
	*/
}

BOOL CNetworkSession::Begin(VOID)
{
	CThreadSync Sync;

	if (mSocket)
		return FALSE;

	memset(mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	mSocket							= NULL;
	mReliableUdpThreadHandle		= NULL;
	mReliableUdpThreadStartupEvent	= NULL;
	mReliableUdpThreadDestroyEvent	= NULL;
	mReliableUdpThreadWakeUpEvent	= NULL;
	mReliableUdpWriteCompleteEvent	= NULL;

	mIsReliableUdpSending			= FALSE;

	return TRUE;
}

BOOL CNetworkSession::End(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	closesocket(mSocket);

	mSocket					= NULL;

	if (mReliableUdpThreadHandle)
	{
		SetEvent(mReliableUdpThreadDestroyEvent);

		WaitForSingleObject(mReliableUdpThreadHandle, INFINITE);

		CloseHandle(mReliableUdpThreadHandle);
	}

	if (mReliableUdpThreadDestroyEvent)
		CloseHandle(mReliableUdpThreadDestroyEvent);

	if (mReliableUdpThreadStartupEvent)
		CloseHandle(mReliableUdpThreadStartupEvent);

	if (mReliableUdpThreadWakeUpEvent)
		CloseHandle(mReliableUdpThreadWakeUpEvent);

	if (mReliableUdpWriteCompleteEvent)
		CloseHandle(mReliableUdpWriteCompleteEvent);

	mReliableWriteQueue.End();

	return TRUE;
}

BOOL CNetworkSession::Listen(USHORT port, INT backLog)
{
	CThreadSync Sync;

	if (port <= 0 || backLog <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN ListenSocketInfo;

	ListenSocketInfo.sin_family				= AF_INET;
	ListenSocketInfo.sin_port				= htons(port);
	ListenSocketInfo.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);

	if (bind(mSocket, (struct sockaddr*) &ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	if (listen(mSocket, backLog) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	LINGER Linger; //현재 보내고 있거나 받는 데이터가 있을경우 그 데이터를 모두 보낸후 Close하는 옵션
	Linger.l_onoff	= 1;
	Linger.l_linger = 0;

	//acceptEX를 실행하기 위해서 해야하는 함수
	/*이 처리를 수행하지 않을 경우, AcceptEx() 가 동작중이지 않아도 자동적으로 listen() 이 발생되게 된다.*/
	if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*) &Linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkSession::Accept(SOCKET listenSocket)
{
	CThreadSync Sync;

	if (!listenSocket)
		return FALSE;

	if (mSocket)
		return FALSE;

	mSocket	= WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
	{
		End();

		return FALSE;
	}

	//BOOL NoDelay = TRUE;
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));

	//AcceptEx를 사용한 이유
	/*일반적인 accept처럼 while문으로 처리하는게 아니라
	한 acceptEx당 하나의 소켓을 미리 만들어두고 요청이 있을때 만둘어둔 소켓을 활용하는방법
	많은 접속자가 몰릴때를 대비한다*/
	if (!AcceptEx(listenSocket, 
		mSocket, 
		mReadBuffer, 
		0, 
		sizeof(sockaddr_in) + 16, 
		sizeof(sockaddr_in) + 16, 
		NULL, 
		&mAcceptOverlapped.Overlapped))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CNetworkSession::InitializeReadForIocp(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes	= 0;
	DWORD	ReadFlag	= 0;

	WsaBuf.buf			= (CHAR*) mReadBuffer;
	WsaBuf.len			= MAX_BUFFER_LENGTH;

	INT		ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkSession::ReadForIocp(BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	memcpy(data, mReadBuffer, dataLength);

	return TRUE;
}

BOOL CNetworkSession::ReadForEventSelect(BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes	= 0;
	DWORD	ReadFlag	= 0;

	WsaBuf.buf			= (CHAR*) mReadBuffer;
	WsaBuf.len			= MAX_BUFFER_LENGTH;

	INT		ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	memcpy(data, mReadBuffer, ReadBytes);
	dataLength = ReadBytes;

	return TRUE;
}

BOOL CNetworkSession::Write(BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <=0)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	WriteBytes	= 0;
	DWORD	WriteFlag	= 0;

	WsaBuf.buf			= (CHAR*) data;
	WsaBuf.len			= dataLength;

	INT		ReturnValue	= WSASend(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		&mWriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkSession::Connect(LPSTR address, USHORT port)
{
	CThreadSync Sync;

	if (!address || port <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(address);

	if (WSAConnect(mSocket, (LPSOCKADDR) &RemoteAddressInfo, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CNetworkSession::UdpBind(USHORT port)
{
	CThreadSync Sync;

	if (mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);

	mSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	if (bind(mSocket, (struct sockaddr*) &RemoteAddressInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// mReliableUdpThreadDestroyEvent
	mReliableUdpThreadDestroyEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadDestroyEvent == NULL)
	{
		End();

		return FALSE;
	}
	//

	// mReliableUdpThreadStartupEvent
	mReliableUdpThreadStartupEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadStartupEvent == NULL)
	{
		End();

		return FALSE;
	}
	//

	// mReliableUdpThreadWakeUpEvent
	mReliableUdpThreadWakeUpEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadWakeUpEvent == NULL)
	{
		End();

		return FALSE;
	}
	//

	// mReliableUdpWriteCompleteEvent
	mReliableUdpWriteCompleteEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpWriteCompleteEvent == NULL)
	{
		End();

		return FALSE;
	}
	//

	if (!mReliableWriteQueue.Begin())
	{
		End();

		return FALSE;
	}

	DWORD ReliableUdpThreadID	= 0;
	mReliableUdpThreadHandle	= CreateThread(NULL, 0, ::ReliableUdpThreadCallback, this, 0, &ReliableUdpThreadID);

	WaitForSingleObject(mReliableUdpThreadStartupEvent, INFINITE);

	return TRUE;
}

BOOL CNetworkSession::TcpBind(VOID)
{
	CThreadSync Sync;

	if (mSocket)
		return FALSE;

	mSocket	= WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	if (mSocket == INVALID_SOCKET)
	{
		int a = WSAGetLastError();
		return FALSE;
	}


	//BOOL NoDelay = TRUE;
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));
	return TRUE;
}

BOOL CNetworkSession::GetLocalIP(WCHAR* pIP)
{
	CThreadSync Sync;

	if(!mSocket)
		return FALSE;

	//SOCKADDR_IN addr;
	//ZeroMemory(&addr, sizeof(addr));

	//int addrLength = sizeof(addr);
	//if(getsockname(mSocket, (sockaddr*)&addr, &addrLength ) != SOCKET_ERROR)
	//{
	//	if(MultiByteToWideChar(CP_ACP, 0, inet_ntoa(addr.sin_addr), 32, pIP, 32) > 0)
	//		return TRUE;
	//}

	CHAR	Name[256]	= {0,};

	gethostname(Name, sizeof(Name));

	PHOSTENT host = gethostbyname(Name);
	if (host)
	{
		if(MultiByteToWideChar(CP_ACP, 0, inet_ntoa(*(struct in_addr*)*host->h_addr_list), -1, pIP, 32) > 0)
			return TRUE;
	}

	return FALSE;
}

USHORT CNetworkSession::GetLocalPort(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return 0;

	SOCKADDR_IN Addr;
	ZeroMemory(&Addr, sizeof(Addr));

	INT AddrLength = sizeof(Addr);
	if (getsockname(mSocket, (sockaddr*) &Addr, &AddrLength) != SOCKET_ERROR)
		return ntohs(Addr.sin_port);

	return 0;
}

BOOL CNetworkSession::InitializeReadFromForIocp(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF		WsaBuf;
	DWORD		ReadBytes				= 0;
	DWORD		ReadFlag				= 0;
	INT			RemoteAddressInfoSize	= sizeof(mUdpRemoteInfo);

	WsaBuf.buf			= (CHAR*) mReadBuffer;
	WsaBuf.len			= MAX_BUFFER_LENGTH;

	INT		ReturnValue = WSARecvFrom(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*) &mUdpRemoteInfo,
		&RemoteAddressInfoSize,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkSession::ReadFromForIocp(LPSTR remoteAddress, USHORT &remotePort, BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	memcpy(data, mReadBuffer, dataLength);

	//memcpy(remoteAddress, inet_ntoa(mUdpRemoteInfo.sin_addr), 32);
	strcpy_s(remoteAddress, 32, inet_ntoa(mUdpRemoteInfo.sin_addr));
	remotePort	= ntohs(mUdpRemoteInfo.sin_port);

	USHORT Ack = 0;
	memcpy(&Ack, mReadBuffer, sizeof(USHORT));

	if (Ack == 9999)
	{
		SetEvent(mReliableUdpWriteCompleteEvent);

		return FALSE;
	}
	else
	{
		Ack = 9999;
		WriteTo2(remoteAddress, remotePort, (BYTE*) &Ack, sizeof(USHORT));
	}

	return TRUE;
}

BOOL CNetworkSession::ReadFromForEventSelect(LPSTR remoteAddress, USHORT &remotePort, BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!mSocket)
		return FALSE;

	WSABUF		WsaBuf;
	DWORD		ReadBytes				= 0;
	DWORD		ReadFlag				= 0;
	INT			RemoteAddressInfoSize	= sizeof(mUdpRemoteInfo);

	WsaBuf.buf			= (CHAR*) mReadBuffer;
	WsaBuf.len			= MAX_BUFFER_LENGTH;

	INT		ReturnValue = WSARecvFrom(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*) &mUdpRemoteInfo,
		&RemoteAddressInfoSize,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	memcpy(data, mReadBuffer, ReadBytes);
	dataLength	= ReadBytes;

	//memcpy(remoteAddress, inet_ntoa(mUdpRemoteInfo.sin_addr), 32);
	//strcpy_s(remoteAddress, inet_ntoa(mUdpRemoteInfo.sin_addr));
	strcpy_s(remoteAddress,  32, inet_ntoa(mUdpRemoteInfo.sin_addr));
	remotePort	= ntohs(mUdpRemoteInfo.sin_port);

	USHORT Ack = 0;
	memcpy(&Ack, mReadBuffer, sizeof(USHORT));

	if (Ack == 9999)
	{
		//Ack를 받았으면 쓰기완료
		SetEvent(mReliableUdpWriteCompleteEvent);

		return FALSE;
	}
	else
	{
		//패킷을 받았을때 Ack를 못받았으면 Ack 문자를보낸다
		Ack = 9999;
		WriteTo2(remoteAddress, remotePort, (BYTE*) &Ack, sizeof(USHORT));
	}

	return TRUE;
}

BOOL CNetworkSession::WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE *data, DWORD dataLength)
{
	// 1
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <=0)
		return FALSE;

	if (!mReliableWriteQueue.Push(this, data, dataLength, remoteAddress, remotePort))
		return FALSE;

	if (!mIsReliableUdpSending)
	{
		mIsReliableUdpSending = TRUE;
		SetEvent(mReliableUdpThreadWakeUpEvent);
	}

	return TRUE;
}

BOOL CNetworkSession::WriteTo2(LPSTR remoteAddress, USHORT remotePort, BYTE *data, DWORD dataLength)
{
	//3
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <=0)
		return FALSE;

	WSABUF		WsaBuf;
	DWORD		WriteBytes					= 0;
	DWORD		WriteFlag					= 0;

	SOCKADDR_IN	RemoteAddressInfo;
	INT			RemoteAddressInfoSize		= sizeof(RemoteAddressInfo);

	WsaBuf.buf								= (CHAR*) data;
	WsaBuf.len								= dataLength;

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(remoteAddress);
	RemoteAddressInfo.sin_port				= htons(remotePort);

	INT		ReturnValue	= WSASendTo(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		(SOCKADDR*) &RemoteAddressInfo,
		RemoteAddressInfoSize,
		&mWriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkSession::GetRemoteAddressAfterAccept(WCHAR* remoteAddress, USHORT &remotePort)
{
	CThreadSync Sync;

	if (!remoteAddress)
		return FALSE;

	sockaddr_in		*Local			= NULL;
	INT				LocalLength		= 0;

	sockaddr_in		*Remote			= NULL;
	INT				RemoteLength	= 0;

	GetAcceptExSockaddrs(mReadBuffer, 
		0, 
		sizeof(sockaddr_in) + 16, 
		sizeof(sockaddr_in) + 16,
		(sockaddr **) &Local,
		&LocalLength,
		(sockaddr **) &Remote,
		&RemoteLength);

	//_tcscpy(remoteAddress, (LPTSTR)(inet_ntoa(Remote->sin_addr)));
	CHAR	TempRemoteAddress[32] = {0,};
	//strcpy(TempRemoteAddress, inet_ntoa(Remote->sin_addr));
	strcpy_s(TempRemoteAddress, inet_ntoa(Remote->sin_addr));

	MultiByteToWideChar(CP_ACP,
		0,
		TempRemoteAddress,
		-1,
		remoteAddress,
		32);

	remotePort = ntohs(Remote->sin_port);

	return TRUE;
}

SOCKET CNetworkSession::GetSocket(VOID)
{
	CThreadSync Sync;

	return mSocket;
}