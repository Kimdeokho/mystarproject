#include "StdAfx.h"
#include "MyCommand.h"

#include "MyCmd_Move.h"
#include "MyCmd_InputKey.h"
#include "MyCmd_InputClick.h"
#include "MyCmd_Building.h"
#include "MyCmd_PartBuilding.h"
#include "MyCmd_Leave.h"
#include "MyCmd_chat.h"

CMyCommand::CMyCommand(void)
{
}

CMyCommand::~CMyCommand(void)
{
}

void CMyCommand::Write(CStreamSP& writestream)
{
	writestream->WriteUSHORT(m_ecmdtype);
	writestream->WriteUSHORT(m_eteamnum);

	//이 데이터들은 StaticReadAndCreate에서 읽어들인다
}

void CMyCommand::Read(CStreamSP& readstream)
{

}

void CMyCommand::Progress(void)
{

}

CMyCommand* CMyCommand::StaticReadAndCreate(CStreamSP& readstream)
{
	//CommandList::Read에서 호출중..

	CMyCommand*		pcmd = NULL;

	USHORT ecmd = CMDTYPE_NONE;
	USHORT eteamnum = TEAM_NONE;

	readstream->ReadUSHORT(&ecmd);
	readstream->ReadUSHORT(&eteamnum);


	switch(ecmd)
	{
	case CMDTYPE_MOVE:
		{
			pcmd = new CMyCmd_Move;
			
			pcmd->m_eteamnum = eteamnum;
			pcmd->m_ecmdtype = ecmd;
			pcmd->Read(readstream);
			break;
		}
	case CMDTYPE_INPUTKEY:
		{
			pcmd = new CMyCmd_InputKey;

			pcmd->m_eteamnum = eteamnum;
			pcmd->m_ecmdtype = ecmd;
			pcmd->Read(readstream);
			break;
		}
	case CMDTYPE_INPUTCLICK:
		{
			pcmd = new CMyCmd_InputClick;

			pcmd->m_eteamnum = eteamnum;
			pcmd->m_ecmdtype = ecmd;
			pcmd->Read(readstream);
			break;
		}
	case CMDTYPE_BUILDING:
		{
			pcmd = new CMyCmd_Building;

			pcmd->m_eteamnum = eteamnum;
			pcmd->m_ecmdtype = ecmd;
			pcmd->Read(readstream);
			break;
		}
	case CMDTYPE_PART_BUILDING:
		{
			pcmd = new CMyCmd_PartBuilding;

			pcmd->m_eteamnum = eteamnum;
			pcmd->m_ecmdtype = ecmd;
			pcmd->Read(readstream);
			break;
		}
	case CMDTYPE_LEAVE_PLAYER:
		{
			pcmd = new CMyCmd_Leave;

			pcmd->m_eteamnum = eteamnum;
			pcmd->m_ecmdtype = ecmd;
			pcmd->Read(readstream);
			break;
		}
	case CMDTYPE_CHAT:
		{
			pcmd = new CMyCmd_chat;

			pcmd->m_eteamnum = eteamnum;
			pcmd->m_ecmdtype = ecmd;
			pcmd->Read(readstream);
			break;
		}
	}

	return pcmd;
}
