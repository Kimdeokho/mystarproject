#include "StdAfx.h"
#include "MyCommand.h"

#include "MyCmd_Move.h"
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

	//�� �����͵��� StaticReadAndCreate���� �о���δ�
}

void CMyCommand::Read(CStreamSP& readstream)
{

}

void CMyCommand::Progress(void)
{

}

CMyCommand* CMyCommand::StaticReadAndCreate(CStreamSP& readstream)
{
	//CommandList::Read���� ȣ����..

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
	}

	return pcmd;
}
