#include "StdAfx.h"
#include "TurnData.h"

#include "MyCommandList.h"
CTurnData::CTurnData(CMyCommandList* plist)
{
	m_cmdlist = new CMyCommandList;

	list<CMyCommand*>& src = plist->GetCommandList();
	list<CMyCommand*>& dest = m_cmdlist->GetCommandList();

	dest.assign(src.begin() , src.end());
}

CTurnData::CTurnData(void)
{
	m_cmdlist = new CMyCommandList;
}

CTurnData::~CTurnData(void)
{
	Safe_Delete(m_cmdlist);
}

void CTurnData::Write(CStreamSP& writestream)
{
	//플레이어 팀넘버도 쓰고,
	m_cmdlist->Write(writestream);
}
void CTurnData::Read(CStreamSP& readstream)
{
	m_cmdlist->Read(readstream);
	//CMyCommandList::GetInstance()->Read(readstream);
}