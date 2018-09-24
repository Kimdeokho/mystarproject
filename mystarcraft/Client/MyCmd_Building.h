#pragma once
#include "mycommand.h"

class CMyCmd_Building :
	public CMyCommand
{
protected:
	PREVIEW_INFO	m_preview_info;
	USHORT			m_objnum;
	USHORT			m_unitsize;
	USHORT			m_targetnum;
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_Building* StaticCreate(const PREVIEW_INFO& info);
public:
	CMyCmd_Building(void);
	~CMyCmd_Building(void);
};
