#pragma once

#include "Include.h"

class CTurnData;
class CTestSession;
class CSession_Mgr
{
	DECLARE_SINGLETON(CSession_Mgr)
public:
	enum	NETWORK_STATE
	{
		NS_NONE,
		NS_PLAYING,
		NS_DELAY,
	};
private:
	CTestSession*		m_TCPsession;
	CTestSession*		m_UDPsession;

	BYTE				m_Read_TCPBuf[MAX_BUFFER_LENGTH];
	BYTE				m_Read_UDPBuf[MAX_BUFFER_LENGTH];

	SESSION_INFO		m_session_info;

	
	int					m_subTurnNumber;
	int					m_TurnNumber;
	vector<map<DWORD_PTR , CTurnData*>>	m_vecTurnData;
	TEAM_NUMBER			m_eTeamNumber;

	bool				m_startstage;
	NETWORK_STATE		m_estate;
public:
	TEAM_NUMBER	GetTeamNumber(void) {return m_eTeamNumber;}
	void SetTeamNumber(TEAM_NUMBER eteamnum){ m_eTeamNumber = eteamnum; }
public:
	CTestSession* GetTCP_Session(void);
	CTestSession* GetUDP_Session(void);
	SESSION_INFO  GetSession_Info(void);
	bool	BeginTCP(void);
	bool	BeginUDP(void);

	void	Read_UDPPacket(void);
	void	Read_TCPPacket(void);
	void	SendTurnPacket(void);
	void	TryAdvanceTurn(void);
	void	HandlerTurnPacket(CStreamSP& readstream);

	void	Write_TCP_Packet(DWORD PT , BYTE* packet, DWORD Length);
public:
	void	Initialize(void);
	void	Update(void);
	void	Release(void);
public:
	CSession_Mgr(void);
	~CSession_Mgr(void);
};
