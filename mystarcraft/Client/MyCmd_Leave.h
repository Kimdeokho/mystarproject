#pragma once
#include "mycommand.h"

class CMyCmd_Leave :
	public CMyCommand
{
private:
	DWORD	m_session_id;
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_Leave* StaticCreate(const DWORD session_id);
public:
	CMyCmd_Leave(void);
	~CMyCmd_Leave(void);
};
