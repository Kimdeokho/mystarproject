#pragma once

#include "Include.h"

class CTestSession;
class CSession_Mgr
{
	DECLARE_SINGLETON(CSession_Mgr)
private:
	CTestSession*		m_TCPsession;
	CTestSession*		m_UDPsession;

	BYTE				m_Read_TCPBuf[MAX_BUFFER_LENGTH];
	BYTE				m_Read_UDPBuf[MAX_BUFFER_LENGTH];

	SESSION_INFO		m_session_info;
public:
	CTestSession* GetTCP_Session(void);
	CTestSession* GetUDP_Session(void);
	SESSION_INFO  GetSession_Info(void);
	bool	BeginTCP(void);
	bool	BeginUDP(void);

	void	Read_UDPPacket(void);
	void	Read_TCPPacket(void);

	void	Write_TCP_Packet(DWORD PT , BYTE* packet, DWORD Length);
public:
	void	Initialize(void);
	void	Update(void);
	void	Release(void);
public:
	CSession_Mgr(void);
	~CSession_Mgr(void);
};
