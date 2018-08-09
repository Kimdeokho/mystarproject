#pragma once
#include "mycommand.h"

class CMyCmd_Move :
	public CMyCommand
{
protected:
	USHORT		m_unitsize;
	USHORT		m_targetnum;
	int			m_x;
	int			m_y;
	vector<USHORT>	m_unit_numlist;

public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_Move* StaticCreate(const D3DXVECTOR2& vpt_arrive);
public:
	CMyCmd_Move(void);
	~CMyCmd_Move(void);
};
