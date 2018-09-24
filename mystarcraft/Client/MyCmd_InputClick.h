#pragma once
#include "mycommand.h"

class CMyCmd_InputClick :
	public CMyCommand
{
private:
	USHORT		m_unitsize;
	USHORT		m_targetnum;
	int			m_x;
	int			m_y;
	USHORT		m_nkey;
	vector<USHORT>	m_unit_numlist;
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_InputClick* StaticCreate(D3DXVECTOR2& vdest , const USHORT& nkey);
public:
	CMyCmd_InputClick(void);
	~CMyCmd_InputClick(void);
};
