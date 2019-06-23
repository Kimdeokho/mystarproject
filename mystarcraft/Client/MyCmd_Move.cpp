#include "StdAfx.h"
#include "MyCmd_Move.h"

#include "Obj.h"

#include "Area_Mgr.h"
#include "Session_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "TileManager.h"
#include "SoundDevice.h"

CMyCmd_Move::CMyCmd_Move(void)
{
	m_unit_numlist.reserve(100);
}

CMyCmd_Move::~CMyCmd_Move(void)
{
	m_unit_numlist.clear();
	vector<USHORT> temp;
	temp.swap(m_unit_numlist);
}

void CMyCmd_Move::Write(CStreamSP& writestream)
{
	CMyCommand::Write(writestream);

	writestream->WriteUSHORT(m_unitsize);
	writestream->WriteUSHORT(m_targetnum);

	for(DWORD i = 0; i < m_unitsize; ++i)
	{
		writestream->WriteUSHORT(m_unit_numlist[i]);
	}
	writestream->WriteInt32(m_x);
	writestream->WriteInt32(m_y);
}

void CMyCmd_Move::Read(CStreamSP& readstream)
{
	readstream->ReadUSHORT(&m_unitsize);
	readstream->ReadUSHORT(&m_targetnum);

	USHORT objnum = 0;
	for(DWORD i = 0; i < m_unitsize; ++i)
	{
		readstream->ReadUSHORT(&objnum);
		m_unit_numlist.push_back(objnum);
	}
	readstream->ReadInt32(&m_x);
	readstream->ReadInt32(&m_y);
}

void CMyCmd_Move::Progress(void)
{
	CObj* pobj = NULL;
	list<CObj*>	unitlist;
	D3DXVECTOR2	vdest = D3DXVECTOR2((float)m_x , (float)m_y);

	//printf("%f , %f\n" , vdest.x , vdest.y);

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

	if(!unitlist.empty())
	{
		list<CObj*>::iterator iter = unitlist.begin();
		list<CObj*>::iterator iter_end = unitlist.end();
		for(; iter != iter_end; ++iter)
		{
			(*iter)->Inputkey_reaction(VK_RBUTTON);
		}
	}
}

CMyCmd_Move* CMyCmd_Move::StaticCreate(const D3DXVECTOR2& vpt_arrive)
{
	//명령을 입력한 시점이다.
	list<CObj*>* unitlist = CUnitMgr::GetInstance()->Getcur_unitlist();

	if(!unitlist->empty())
	{
		if(unitlist->front()->GetTeamNumber() != 
			CSession_Mgr::GetInstance()->GetTeamNumber())
			return NULL;
		else
			CSoundDevice::GetInstance()->SetUnitVoice(unitlist->front()->GetOBJNAME());
	}


	CMyCmd_Move*	pcmd = new CMyCmd_Move;
	CObj*			ptarget = NULL;

	pcmd->m_unitsize = unitlist->size();

	ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
	if(NULL == ptarget)
		pcmd->m_targetnum = 0;
	else
		pcmd->m_targetnum = ptarget->GetObjNumber();

	pcmd->m_x = int(vpt_arrive.x);
	pcmd->m_y = int(vpt_arrive.y);

	pcmd->m_ecmdtype = CMDTYPE_MOVE;
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

	/*
	선택된 유닛수
	그 유닛들의 번호,
	타겟팅된 유닛 번호
	도착지점
	*/

	return pcmd;
}
