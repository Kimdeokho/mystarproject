#include "StdAfx.h"
#include "MyCmd_InputClick.h"

#include "Obj.h"

#include "Area_Mgr.h"
#include "Session_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "TileManager.h"
#include "SoundDevice.h"

CMyCmd_InputClick::CMyCmd_InputClick(void)
{
	m_unit_numlist.reserve(400);
}

CMyCmd_InputClick::~CMyCmd_InputClick(void)
{
	m_unit_numlist.clear();
	vector<USHORT> temp;
	temp.swap(m_unit_numlist);
}
void CMyCmd_InputClick::Write(CStreamSP& writestream)
{
	CMyCommand::Write(writestream);

	writestream->WriteUSHORT(m_unitsize);
	writestream->WriteUSHORT(m_targetnum);
	writestream->WriteUSHORT(m_nkey);

	for(DWORD i = 0; i < m_unitsize; ++i)
	{
		writestream->WriteUSHORT(m_unit_numlist[i]);
	}
	writestream->WriteInt32(m_x);
	writestream->WriteInt32(m_y);	
}

void CMyCmd_InputClick::Read(CStreamSP& readstream)
{
	readstream->ReadUSHORT(&m_unitsize);
	readstream->ReadUSHORT(&m_targetnum);
	readstream->ReadUSHORT(&m_nkey);

	USHORT objnum = 0;
	for(DWORD i = 0; i < m_unitsize; ++i)
	{
		readstream->ReadUSHORT(&objnum);
		m_unit_numlist.push_back(objnum);
	}
	readstream->ReadInt32(&m_x);
	readstream->ReadInt32(&m_y);	
}
void CMyCmd_InputClick::Progress(void)
{
	CObj* pobj = NULL;
	list<CObj*>	unitlist;
	D3DXVECTOR2	vdest = D3DXVECTOR2((float)m_x , (float)m_y);

	//printf("Å©±â %d , ÁÂÇ¥ %f , %f\n" ,m_unitsize ,vdest.x , vdest.y);

	for(USHORT i = 0; i < m_unitsize; ++i)
	{
		pobj = CObjMgr::GetInstance()->GetObj(m_unit_numlist[i]);

		if(NULL != pobj)
			unitlist.push_back(pobj);		
	}
	pobj = CObjMgr::GetInstance()->GetObj(m_targetnum);
	CArea_Mgr::GetInstance()->SetChoiceTarget(pobj);

	CUnitMgr::GetInstance()->Calculate_UnitCenterPt(unitlist , vdest);
	CTileManager::GetInstance()->Flowfield_Pathfinding(vdest);

	list<CObj*>::iterator iter = unitlist.begin();
	list<CObj*>::iterator iter_end = unitlist.end();
	for(; iter != iter_end; ++iter)
	{
		(*iter)->Inputkey_reaction(m_nkey, VK_LBUTTON);
	}
}
CMyCmd_InputClick* CMyCmd_InputClick::StaticCreate(const D3DXVECTOR2& vdest ,const USHORT nkey)
{
	list<CObj*>* unitlist = CUnitMgr::GetInstance()->Getcur_unitlist();

	if(!unitlist->empty())
	{
		if(unitlist->front()->GetTeamNumber() != CSession_Mgr::GetInstance()->GetTeamNumber())
			return NULL;
		else
			CSoundDevice::GetInstance()->SetUnitVoice(unitlist->front()->GetOBJNAME());
	}

	CMyCmd_InputClick*	pcmd = new CMyCmd_InputClick;
	CObj*			ptarget = NULL;
	
	pcmd->m_unitsize = unitlist->size();

	ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
	if(NULL == ptarget)
		pcmd->m_targetnum = 0;
	else
		pcmd->m_targetnum = ptarget->GetObjNumber();

	pcmd->m_x = int(vdest.x);
	pcmd->m_y = int(vdest.y);

	pcmd->m_ecmdtype = CMDTYPE_INPUTCLICK;
	pcmd->m_eteamnum = CSession_Mgr::GetInstance()->GetTeamNumber();

	if(!unitlist->empty())
	{
		list<CObj*>::iterator iter = unitlist->begin();
		list<CObj*>::iterator iter_end = unitlist->end();

		for( ; iter != iter_end; ++iter)
		{		
			pcmd->m_unit_numlist.push_back((*iter)->GetObjNumber());
		}
	}

	pcmd->m_nkey = nkey;

	return pcmd;
}
