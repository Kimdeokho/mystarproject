#include "StdAfx.h"
#include "MyCommandList.h"

#include "MyCommand.h"

CMyCommandList::CMyCommandList(void)
{
}

CMyCommandList::~CMyCommandList(void)
{
	list<CMyCommand*>::iterator iter = m_cmds.begin();
	list<CMyCommand*>::iterator iter_end = m_cmds.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete((*iter));

	m_cmds.clear();
}

void CMyCommandList::PushCommand(CMyCommand* pcommand)
{
	m_cmds.push_back(pcommand);
}

void CMyCommandList::ClearCommand(void)
{
	//list<CMyCommand*>::iterator iter = m_cmds.begin();
	//list<CMyCommand*>::iterator iter_end = m_cmds.end();

	//for( ; iter != iter_end; ++iter)
	//	Safe_Delete((*iter));

	m_cmds.clear();
}

list<CMyCommand*>& CMyCommandList::GetCommandList(void)
{
	return m_cmds;
}

void CMyCommandList::Write(CStreamSP& writestream)
{
	//호출시점에서 writestream->SetBuffer(WriteBuffer); 했다고 가정
	USHORT cmdsize = m_cmds.size();
	writestream->WriteUSHORT(cmdsize);

	list<CMyCommand*>::iterator iter = m_cmds.begin();
	list<CMyCommand*>::iterator iter_end = m_cmds.end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Write(writestream);
	}
}

void CMyCommandList::Read(CStreamSP& readstream)
{
	USHORT cmdsize = 0;
	readstream->ReadUSHORT(&cmdsize);


	for(int i = 0; i < cmdsize; ++i)
	{
		m_cmds.push_back(CMyCommand::StaticReadAndCreate(readstream));
	}
}

void CMyCommandList::ProcessCmd(void)
{
	list<CMyCommand*>::iterator iter = m_cmds.begin();
	list<CMyCommand*>::iterator iter_end = m_cmds.end();


	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Progress();
	}
}
