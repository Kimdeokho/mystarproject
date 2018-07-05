#pragma once
//#include "c:\users\ykm\documents\mystarproject\mystarcraft\lowlib\clientsession.h"
#include "ClientSession.h"

class CTestSession :
	public CClientSession
{
public:
	virtual VOID	OnIoConnected(VOID);
	virtual VOID	OnIoDisconnected(VOID);
public:
	CTestSession(void);
	virtual ~CTestSession(void);
};
