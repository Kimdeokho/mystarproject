#include "StdAfx.h"
#include "MyCmd_PartBuilding.h"

#include "Obj.h"
#include "Terran_building.h"

#include "Area_Mgr.h"
#include "Session_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "TileManager.h"
CMyCmd_PartBuilding::CMyCmd_PartBuilding(void)
{
}

CMyCmd_PartBuilding::~CMyCmd_PartBuilding(void)
{
}

void CMyCmd_PartBuilding::Progress(void)
{
	CObj* pobj = CObjMgr::GetInstance()->GetObj(m_objnum);
	list<CObj*>	unitlist;
	unitlist.push_back(pobj);

	((CTerran_building*)pobj)->SetPreview_info(m_main_previewinfo, m_sub_previewinfo);

	pobj->Inputkey_reaction(m_nkey);
}

void CMyCmd_PartBuilding::Write(CStreamSP& writestream)
{
	CMyCommand::Write(writestream);

	writestream->WriteUSHORT(m_unitsize);
	writestream->WriteUSHORT(m_objnum);
	writestream->WriteUSHORT(m_nkey);


	writestream->WriteUSHORT(m_main_previewinfo.ebuild);
	writestream->WriteUSHORT(m_main_previewinfo.icol);
	writestream->WriteUSHORT(m_main_previewinfo.irow);
	writestream->WriteFloat(m_main_previewinfo.vcenter_pos.x);
	writestream->WriteFloat(m_main_previewinfo.vcenter_pos.y);
	writestream->WriteFloat(m_main_previewinfo.vpos.x);
	writestream->WriteFloat(m_main_previewinfo.vpos.y);
	writestream->WriteFloat(m_main_previewinfo.vtx.left);
	writestream->WriteFloat(m_main_previewinfo.vtx.right);
	writestream->WriteFloat(m_main_previewinfo.vtx.top);
	writestream->WriteFloat(m_main_previewinfo.vtx.bottom);

	writestream->WriteUSHORT(m_sub_previewinfo.ebuild);
	writestream->WriteUSHORT(m_sub_previewinfo.icol);
	writestream->WriteUSHORT(m_sub_previewinfo.irow);
	writestream->WriteFloat(m_sub_previewinfo.vcenter_pos.x);
	writestream->WriteFloat(m_sub_previewinfo.vcenter_pos.y);
	writestream->WriteFloat(m_sub_previewinfo.vpos.x);
	writestream->WriteFloat(m_sub_previewinfo.vpos.y);
	writestream->WriteFloat(m_sub_previewinfo.vtx.left);
	writestream->WriteFloat(m_sub_previewinfo.vtx.right);
	writestream->WriteFloat(m_sub_previewinfo.vtx.top);
	writestream->WriteFloat(m_sub_previewinfo.vtx.bottom);
}

void CMyCmd_PartBuilding::Read(CStreamSP& readstream)
{
	readstream->ReadUSHORT(&m_unitsize);

	readstream->ReadUSHORT(&m_objnum);
	readstream->ReadUSHORT(&m_nkey);

	USHORT ebuild = 0;

	readstream->ReadUSHORT(&ebuild);
	readstream->ReadUSHORT(&m_main_previewinfo.icol);
	readstream->ReadUSHORT(&m_main_previewinfo.irow);
	readstream->ReadFloat(&m_main_previewinfo.vcenter_pos.x);
	readstream->ReadFloat(&m_main_previewinfo.vcenter_pos.y);
	readstream->ReadFloat(&m_main_previewinfo.vpos.x);
	readstream->ReadFloat(&m_main_previewinfo.vpos.y);
	readstream->ReadFloat(&m_main_previewinfo.vtx.left);
	readstream->ReadFloat(&m_main_previewinfo.vtx.right);
	readstream->ReadFloat(&m_main_previewinfo.vtx.top);
	readstream->ReadFloat(&m_main_previewinfo.vtx.bottom);
	m_main_previewinfo.ebuild = (TERRAN_BUILD_TECH)ebuild;

	readstream->ReadUSHORT(&ebuild);
	readstream->ReadUSHORT(&m_sub_previewinfo.icol);
	readstream->ReadUSHORT(&m_sub_previewinfo.irow);
	readstream->ReadFloat(&m_sub_previewinfo.vcenter_pos.x);
	readstream->ReadFloat(&m_sub_previewinfo.vcenter_pos.y);
	readstream->ReadFloat(&m_sub_previewinfo.vpos.x);
	readstream->ReadFloat(&m_sub_previewinfo.vpos.y);
	readstream->ReadFloat(&m_sub_previewinfo.vtx.left);
	readstream->ReadFloat(&m_sub_previewinfo.vtx.right);
	readstream->ReadFloat(&m_sub_previewinfo.vtx.top);
	readstream->ReadFloat(&m_sub_previewinfo.vtx.bottom);
	m_sub_previewinfo.ebuild = (TERRAN_BUILD_TECH)ebuild;
}

CMyCmd_PartBuilding* CMyCmd_PartBuilding::StaticCreate(const PREVIEW_INFO& maininfo, const PREVIEW_INFO& subinfo , const USHORT& nkey)
{
	CMyCmd_PartBuilding* pcmd = new CMyCmd_PartBuilding;
	CObj*			ptarget = NULL;

	pcmd->m_ecmdtype = CMDTYPE_PART_BUILDING;
	pcmd->m_eteamnum = CSession_Mgr::GetInstance()->GetTeamNumber();

	pcmd->m_nkey = nkey;
	pcmd->m_main_previewinfo = maininfo;
	pcmd->m_sub_previewinfo = subinfo;

	list<CObj*>* unitlist = CUnitMgr::GetInstance()->Getcur_unitlist();
	pcmd->m_unitsize = unitlist->size();


	if(!unitlist->empty())
	{
		list<CObj*>::iterator iter = unitlist->begin();
		list<CObj*>::iterator iter_end = unitlist->end();

		for( ; iter != iter_end; ++iter)
			pcmd->m_objnum = (*iter)->GetObjNumber();
	}

	return pcmd;
}
