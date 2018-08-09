#pragma once

#include "Include.h"

class CMyCommandList;
class CTurnData
{
private:
	CMyCommandList*		m_cmdlist;
public:
	void Write(CStreamSP& writestream);
	void Read(CStreamSP& readstream);
	CMyCommandList*		GetCmdList(void) {return m_cmdlist;}
public:
	CTurnData(void);
	CTurnData(CMyCommandList* plist);
	~CTurnData(void);
};
