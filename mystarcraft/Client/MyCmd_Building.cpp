#include "StdAfx.h"
#include "MyCmd_Building.h"

#include "Obj.h"
#include "Workman.h"

#include "Area_Mgr.h"
#include "Session_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "TileManager.h"
CMyCmd_Building::CMyCmd_Building(void)
{
}

CMyCmd_Building::~CMyCmd_Building(void)
{
}

void CMyCmd_Building::Progress(void)
{
	CObj* pobj = CObjMgr::GetInstance()->GetObj(m_objnum);	
	CObj* ptarget = CObjMgr::GetInstance()->GetObj(m_targetnum);
	list<CObj*>	unitlist;
	unitlist.push_back(pobj);

	((CWorkman*)pobj)->SetPreview_info(m_preview_info);

	CArea_Mgr::GetInstance()->SetChoiceTarget(ptarget);
	CUnitMgr::GetInstance()->Calculate_UnitCenterPt(unitlist , m_preview_info.vcenter_pos);
	CTileManager::GetInstance()->Flowfield_Pathfinding(m_preview_info.vcenter_pos);

	pobj->Inputkey_reaction(9999);
}

void CMyCmd_Building::Write(CStreamSP& writestream)
{
	CMyCommand::Write(writestream);

	writestream->WriteUSHORT(m_unitsize);
	writestream->WriteUSHORT(m_targetnum);

	writestream->WriteUSHORT(m_objnum);

	
	writestream->WriteUSHORT(m_preview_info.ebuild);
	writestream->WriteUSHORT(m_preview_info.icol);
	writestream->WriteUSHORT(m_preview_info.irow);
	writestream->WriteFloat(m_preview_info.vcenter_pos.x);
	writestream->WriteFloat(m_preview_info.vcenter_pos.y);
	writestream->WriteFloat(m_preview_info.vpos.x);
	writestream->WriteFloat(m_preview_info.vpos.y);
	writestream->WriteFloat(m_preview_info.vtx.left);
	writestream->WriteFloat(m_preview_info.vtx.right);
	writestream->WriteFloat(m_preview_info.vtx.top);
	writestream->WriteFloat(m_preview_info.vtx.bottom);
}

void CMyCmd_Building::Read(CStreamSP& readstream)
{
	readstream->ReadUSHORT(&m_unitsize);
	readstream->ReadUSHORT(&m_targetnum);

	readstream->ReadUSHORT(&m_objnum);

	USHORT ebuild = 0;

	readstream->ReadUSHORT(&ebuild);
	readstream->ReadUSHORT(&m_preview_info.icol);
	readstream->ReadUSHORT(&m_preview_info.irow);
	readstream->ReadFloat(&m_preview_info.vcenter_pos.x);
	readstream->ReadFloat(&m_preview_info.vcenter_pos.y);
	readstream->ReadFloat(&m_preview_info.vpos.x);
	readstream->ReadFloat(&m_preview_info.vpos.y);
	readstream->ReadFloat(&m_preview_info.vtx.left);
	readstream->ReadFloat(&m_preview_info.vtx.right);
	readstream->ReadFloat(&m_preview_info.vtx.top);
	readstream->ReadFloat(&m_preview_info.vtx.bottom);

	m_preview_info.ebuild = (TERRAN_BUILD_TECH)ebuild;
}

CMyCmd_Building* CMyCmd_Building::StaticCreate(const PREVIEW_INFO& info)
{
	CMyCmd_Building* pcmd = new CMyCmd_Building;
	CObj*			ptarget = NULL;

	pcmd->m_ecmdtype = CMDTYPE_BUILDING;
	pcmd->m_eteamnum = CSession_Mgr::GetInstance()->GetTeamNumber();

	pcmd->m_preview_info = info;

	list<CObj*>* unitlist = CUnitMgr::GetInstance()->Getcur_unitlist();
	pcmd->m_unitsize = unitlist->size();

	ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
	if(NULL == ptarget)
		pcmd->m_targetnum = 0;
	else
		pcmd->m_targetnum = ptarget->GetObjNumber();



	if(!unitlist->empty())
	{
		list<CObj*>::iterator iter = unitlist->begin();
		list<CObj*>::iterator iter_end = unitlist->end();

		for( ; iter != iter_end; ++iter)
			pcmd->m_objnum = (*iter)->GetObjNumber();
	}

	return pcmd;
}
