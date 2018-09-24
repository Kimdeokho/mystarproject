#pragma once
#include "mycommand.h"

class CMyCmd_InputKey :
	public CMyCommand
{
protected:
	USHORT			m_unitsize;
	USHORT			m_targetnum;
	USHORT			m_nkey1;
	USHORT			m_nkey2;
	vector<USHORT>	m_unit_numlist;
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_InputKey* StaticCreate(const USHORT& nkey1 , const USHORT nkey2 = 0);
public:
	CMyCmd_InputKey(void);
	~CMyCmd_InputKey(void);
};
