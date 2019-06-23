#include "StdAfx.h"
#include "MyCmd_InputKey.h"

#include "Obj.h"

#include "Area_Mgr.h"
#include "Session_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"

CMyCmd_InputKey::CMyCmd_InputKey(void)
{
	m_unit_numlist.reserve(100);
	m_nkey1 = 0;
	m_nkey2 = 0;
}

CMyCmd_InputKey::~CMyCmd_InputKey(void)
{
	m_unit_numlist.clear();
	vector<USHORT> temp;
	temp.swap(m_unit_numlist);
}

void CMyCmd_InputKey::Progress(void)
{
	CObj* pobj = NULL;

	for(USHORT i = 0; i < m_unitsize; ++i)
	{
		pobj = CObjMgr::GetInstance()->GetObj(m_unit_numlist[i]);
		if(NULL == pobj)
			continue;

		if(0 == m_nkey2)
			pobj->Inputkey_reaction(m_nkey1);
		else
			pobj->Inputkey_reaction(m_nkey1 , m_nkey2);
	}
}

void CMyCmd_InputKey::Write(CStreamSP& writestream)
{
	CMyCommand::Write(writestream);

	writestream->WriteUSHORT(m_unitsize);

	for(DWORD i = 0; i < m_unitsize; ++i)
		writestream->WriteUSHORT(m_unit_numlist[i]);

	writestream->WriteUSHORT(m_nkey1);
	writestream->WriteUSHORT(m_nkey2);
}

void CMyCmd_InputKey::Read(CStreamSP& readstream)
{
	readstream->ReadUSHORT(&m_unitsize);

	USHORT objnum = 0;
	for(DWORD i = 0; i < m_unitsize; ++i)
	{
		readstream->ReadUSHORT(&objnum);
		m_unit_numlist.push_back(objnum);
	}

	readstream->ReadUSHORT(&m_nkey1);
	readstream->ReadUSHORT(&m_nkey2);
}

CMyCmd_InputKey* CMyCmd_InputKey::StaticCreate(const USHORT& nkey1 , const USHORT nkey2)
{
	list<CObj*>* unitlist = CUnitMgr::GetInstance()->Getcur_unitlist();

	if(unitlist->front()->GetTeamNumber() != 
		CSession_Mgr::GetInstance()->GetTeamNumber())
		return NULL;

	CMyCmd_InputKey* pcmd = new CMyCmd_InputKey;

	pcmd->m_unitsize = unitlist->size();

	pcmd->m_ecmdtype = CMDTYPE_INPUTKEY;
	pcmd->m_nkey1 = nkey1;
	pcmd->m_nkey2 = nkey2;

	if(!unitlist->empty())
	{
		list<CObj*>::iterator iter = unitlist->begin();
		list<CObj*>::iterator iter_end = unitlist->end();

		for( ; iter != iter_end; ++iter)	
			pcmd->m_unit_numlist.push_back((*iter)->GetObjNumber());
	}

	return pcmd;
}
