#pragma once

#include "Include.h"

class CMyCommand;
class CMyCommandList
{
	//인풋매니저가 클래스를 갖고있는다.
private:
	list<CMyCommand*>	m_cmds;
public:
	USHORT GetSize(void) {return m_cmds.size();}
public:
	void PushCommand(CMyCommand* pcommand);
	void ClearCommand(void);

	list<CMyCommand*>&	GetCommandList(void);
public:
	void Write(CStreamSP& writestream);
	void Read(CStreamSP& readstream);
	void ProcessCmd(void);
public:
	CMyCommandList(void);
	~CMyCommandList(void);
};
