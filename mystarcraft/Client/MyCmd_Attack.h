#pragma once
#include "mycommand.h"

class CMyCmd_Attack :
	public CMyCommand
{
	//어택땅 명령 클래스
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCmd_Attack* StaticCreate(const D3DXVECTOR2& vpt_arrive);
public:
	CMyCmd_Attack(void);
	~CMyCmd_Attack(void);
};
