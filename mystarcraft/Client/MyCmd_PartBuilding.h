#pragma once
#include "mycommand.h"

class CMyCmd_PartBuilding :
	public CMyCommand
{
private:
	PREVIEW_INFO	m_main_previewinfo;
	PREVIEW_INFO	m_sub_previewinfo;
	USHORT			m_objnum;
	USHORT			m_unitsize;
	USHORT			m_nkey;
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_PartBuilding* StaticCreate(const PREVIEW_INFO& maininfo, const PREVIEW_INFO& subinfo , const USHORT& nkey);
public:
	CMyCmd_PartBuilding(void);
	~CMyCmd_PartBuilding(void);
};
