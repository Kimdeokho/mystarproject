#pragma once
#include "Include.h"

class CMyCommand
{
public:
	enum	COMMANDTYPE
	{
		CMDTYPE_NONE,
		CMDTYPE_ATTACK_MOVE,
		CMDTYPE_MOVE,
		CMDTYPE_INPUTKEY,
		CMDTYPE_INPUTCLICK,
		CMDTYPE_BUILDING,
		CMDTYPE_PART_BUILDING,
		CMDTYPE_LEAVE_PLAYER,
		CMDTYPE_CHAT,
	};

protected:
	USHORT		m_eteamnum;
	USHORT		m_ecmdtype;
public:
	virtual void Progress(void);
	virtual void Write(CStreamSP& writestream);
	virtual void Read(CStreamSP& readstream);
public:
	static CMyCommand*	StaticReadAndCreate(CStreamSP& readstream);
public:
	CMyCommand(void);
	virtual ~CMyCommand(void);
};
