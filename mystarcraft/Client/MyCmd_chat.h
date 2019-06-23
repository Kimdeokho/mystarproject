#pragma once
#include "mycommand.h"

class CMyCmd_chat :
	public CMyCommand
{
private:
	WCHAR	m_user_id[32];
	WCHAR	m_message[128];
	DWORD	m_session_id;
	//FLOAT	m_retain_time;
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_chat* StaticCreate(const S_PT_ROOM_RECEIVE_CHAT_M& _msg);
public:
	CMyCmd_chat(void);
	~CMyCmd_chat(void);
};
