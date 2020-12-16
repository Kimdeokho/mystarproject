#include "stdafx.h"
#include "UnitMgr.h"

#include "Obj.h"
#include "TileManager.h"
#include "Unit.h"

#include "FontMgr.h"
#include "MyMath.h"
#include "MouseMgr.h"
#include "ScrollMgr.h"
#include "Area_Mgr.h"

#include "Com_Targetsearch.h"
#include "Ingame_UIMgr.h"

#include "UI_Cmd_info.h"
#include "SoundDevice.h"

IMPLEMENT_SINGLETON(CUnitMgr)
CUnitMgr::CUnitMgr(void)
{

}

CUnitMgr::~CUnitMgr(void)
{

}

void CUnitMgr::Initialize(void)
{
	m_vUnitcenterpt		= D3DXVECTOR2(0,0);
	m_vGoalPos			= D3DXVECTOR2(0,0);
	m_magicbox_unitcnt	= 0;
	m_vGoalIdx			= 0;
}
void CUnitMgr::SetUnit(CObj* pobj)
{
	m_curunitList.push_back(pobj);
}
bool CUnitMgr::is_unit(const CObj* const pobj)
{
	list<CObj*>::iterator iter = m_curunitList.begin();
	list<CObj*>::iterator iter_end = m_curunitList.end();

	for( ; iter != iter_end; ++iter)
	{
		if(pobj == *iter)
			return true;
	}

	return false;
}
void CUnitMgr::discharge_unit(void)
{
	list<CObj*>::iterator iter = m_curunitList.begin();
	list<CObj*>::iterator iter_end = m_curunitList.end();

	for( ; iter != iter_end; ++iter)
		(*iter)->SetSelect(NONE_SELECT);

	m_curunitList.clear();
}

void CUnitMgr::Intputkey_reaction(const int& nkey)
{
	if(!m_curunitList.empty())
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		
		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Inputkey_reaction(nkey);
		}
	}
}
void CUnitMgr::Intputkey_reaction(const int& firstkey , const int& secondkey)
{
	if(!m_curunitList.empty())
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Inputkey_reaction(firstkey , secondkey);
		}
	}
}

bool CUnitMgr::Input_cmd(const int& nkey , bool* waitkey)
{
	if(!m_curunitList.empty())
	{
		return m_curunitList.front()->Input_cmd(nkey , waitkey);
	}
	return false;
}
bool CUnitMgr::Input_cmd(const int& firstkey , const int& secondkey)
{
	if(!m_curunitList.empty())
	{
		return m_curunitList.front()->Input_cmd(firstkey , secondkey);
	}
	return false;
}


void CUnitMgr::clear_destroy_unitlist(CObj* pobj)
{	
	if(!m_curunitList.empty())
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		for( ; iter != iter_end; )
		{
			if(pobj == (*iter))
			{
				iter = m_curunitList.erase(iter);
				break;
			}
			else
				++iter;
		}

		if(m_curunitList.empty())
			CIngame_UIMgr::GetInstance()->ClearWireFrame();

	}
	else
	{
		CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(NULL , STATE_NONE);
	}

	int objcnt = pobj->GetObjCountNumber();
	map<int , CObj*>::iterator map_iter;
	for(int i = 0; i < 10; ++i)
	{
		if(!m_troops[i].empty())
		{
			map_iter = m_troops[i].find(objcnt);
			if(m_troops[i].end() != map_iter)
				m_troops[i].erase(map_iter);
		}

	}
}
void CUnitMgr::Calculate_UnitCenterPt(list<CObj*>& unitlist, const D3DXVECTOR2& vgoalpos)
{
	if(unitlist.empty())
		return;

	list<CObj*>::iterator iter = unitlist.begin();
	list<CObj*>::iterator iter_end = unitlist.end();

	CObj* pobj = unitlist.front();
	m_vUnitcenterpt = D3DXVECTOR2(0,0);


	iter	 = unitlist.begin();
	iter_end = unitlist.end();

	CComponent* com_targetsearch = NULL;

	MYRECT<float>	magicbox;
	magicbox.left = 4096;
	magicbox.right = 0;
	magicbox.top = 4096;
	magicbox.bottom = 0;
	D3DXVECTOR2 vpos;

	for( ; iter != iter_end; ++iter) 
	{
		vpos = (*iter)->GetPos();
		if( vpos.x <= magicbox.left)
			magicbox.left = vpos.x;
		if( vpos.x >= magicbox.right)
			magicbox.right = vpos.x;
		if( vpos.y <= magicbox.top)
			magicbox.top = vpos.y;
		if( vpos.y >= magicbox.bottom)
			magicbox.bottom = vpos.y;

		if( MOVE_NOT != (*iter)->GetUnitinfo().eMoveType &&	
			TRANSFORMING != (*iter)->GetUnitinfo().state &&
			BURROW != (*iter)->GetUnitinfo().state &&
			CATEGORY_UNIT == (*iter)->GetCategory())
			(*iter)->SetState(MOVE);
	}

	iter = unitlist.begin();
	m_magicbox_unitcnt = 0;


	for( ; iter != iter_end; ++iter)
	{
		vpos = (*iter)->GetPos();
		if(magicbox.right - magicbox.left <= 192 &&
			magicbox.bottom - magicbox.top <= 192 )
		{
			(*iter)->SetMagicBox(true);
		}
		else
			(*iter)->SetMagicBox(false);

		++m_magicbox_unitcnt;
		m_vUnitcenterpt += vpos;
	}

	m_vUnitcenterpt /= float(m_magicbox_unitcnt);
	m_vGoalPos = vgoalpos;
	m_vGoalIdx = CMyMath::Pos_to_index(m_vGoalPos , 32);
}
void CUnitMgr::Calculate_UnitCenterPt(const D3DXVECTOR2& vgoalpos /*, CObj* ptarget*/)
{

	//서버(멀티플레이)일땐 필요없음
	list<CObj*>::iterator iter = m_curunitList.begin();
	list<CObj*>::iterator iter_end = m_curunitList.end();


	CObj* pobj = m_curunitList.front();
	m_vUnitcenterpt = D3DXVECTOR2(0,0);
	

	iter	 = m_curunitList.begin();
	iter_end = m_curunitList.end();

	CComponent* com_targetsearch = NULL;

	MYRECT<float>	magicbox;
	magicbox.left = 4096;
	magicbox.right = 0;
	magicbox.top = 4096;
	magicbox.bottom = 0;
	D3DXVECTOR2 vpos;

	for( ; iter != iter_end; ++iter) 
	{
		vpos = (*iter)->GetPos();
		if( vpos.x <= magicbox.left)
			magicbox.left = vpos.x;
		if( vpos.x >= magicbox.right)
			magicbox.right = vpos.x;
		if( vpos.y <= magicbox.top)
			magicbox.top = vpos.y;
		if( vpos.y >= magicbox.bottom)
			magicbox.bottom = vpos.y;

		if( MOVE_NOT != (*iter)->GetUnitinfo().eMoveType &&	
			TRANSFORMING != (*iter)->GetUnitinfo().state &&
			BURROW != (*iter)->GetUnitinfo().state &&
			CATEGORY_UNIT == (*iter)->GetCategory())
			(*iter)->SetState(MOVE);
	}

	iter = m_curunitList.begin();
	m_magicbox_unitcnt = 0;

	for( ; iter != iter_end; ++iter)
	{
		vpos = (*iter)->GetPos();

		if(magicbox.right - magicbox.left <= 192 &&
			magicbox.bottom - magicbox.top <= 192 )
		{
			(*iter)->SetMagicBox(true);
		}
		else
			(*iter)->SetMagicBox(false);

		++m_magicbox_unitcnt;
		m_vUnitcenterpt += vpos;
	}

	m_vUnitcenterpt /= float(m_magicbox_unitcnt);
	m_vGoalPos = vgoalpos;
	m_vGoalIdx = CMyMath::Pos_to_index(m_vGoalPos , 32);


}

bool CUnitMgr::Unit_Unification(void)
{
	if(!m_curunitList.empty())
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		OBJID eobjid = m_curunitList.front()->GetOBJNAME();
		for( ; iter != iter_end; ++iter)
		{
			if(eobjid != (*iter)->GetOBJNAME())
				return false;
		}
		return true;
	}
	return false;
}
bool CUnitMgr::GetUnitlistempty(void)
{
	if(m_curunitList.empty())
		return true;

	return false;
}
D3DXVECTOR2 CUnitMgr::GetUnitGoalPos(void)
{
	return m_vGoalPos;
}
D3DXVECTOR2 CUnitMgr::GetUnitCentterPt(void)
{
	return m_vUnitcenterpt;
}
int CUnitMgr::GetGoalidx(void)
{
	return m_vGoalIdx;
}

void CUnitMgr::Update_UI_Infomation(void)
{
	if(!m_curunitList.empty())
	{
		CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

		if(1 == m_curunitList.size())
		{
			pui->Update_Cmdbtn( m_curunitList.front() );
			CIngame_UIMgr::GetInstance()->Update_Wireframe( m_curunitList.front() );
		}
		else
		{
			list<CObj*>::iterator iter = m_curunitList.begin();
			list<CObj*>::iterator iter_end = m_curunitList.end();

			bool bflag = false;
			CObj* pobj = m_curunitList.front();
			for( ; iter != iter_end; ++iter)
			{
				if( pobj->GetOBJNAME() != (*iter)->GetOBJNAME() )
				{
					bflag = true;
					break;
				}
			}
			if(true == bflag)
				pui->Update_Cmdbtn(NULL);
			else
			{
				if(OBJ_SCV == pobj->GetOBJNAME() || OBJ_DRONE == pobj->GetOBJNAME())
					pui->Update_Cmdbtn(NULL);
				else
					pui->Update_Cmdbtn(pobj);				
			}
			
			CFontMgr::GetInstance()->renewal_infomation_font();
			CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(NULL , STATE_NONE);
		}
	}
	else
	{
		//명령 버튼을 비운다
		CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		(pui)->clear_btn();
	}
}

int CUnitMgr::GetSelectunit_size(void)
{
	return int(m_curunitList.size());
}

list<CObj*>* CUnitMgr::Getcur_unitlist(void)
{
	return &m_curunitList;
}
void CUnitMgr::Troop_call(const int idx)
{
	if(!m_troops[idx - 48].empty())
	{
		if(!m_curunitList.empty())
		{
			list<CObj*>::iterator iter = m_curunitList.begin();
			list<CObj*>::iterator iter_end = m_curunitList.end();

			for( ; iter != iter_end; ++iter)
				(*iter)->SetSelect(NONE_SELECT);

			m_curunitList.clear();
		}

		map<int , CObj*>::iterator iter_map = m_troops[idx - 48].begin();
		map<int , CObj*>::iterator iter_map_end = m_troops[idx - 48].end();

		for( ; iter_map != iter_map_end; ++iter_map)
		{
			m_curunitList.push_back(iter_map->second);
			iter_map->second->SetSelect(GENERAL_SELECT);
		}
	}	
}
void CUnitMgr::Assigned_troop(const int idx)
{
	if(!m_curunitList.empty())
	{
		m_troops[idx - 48].clear();

		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		for( ; iter != iter_end; ++iter)
		{			
			m_troops[idx - 48].insert(make_pair((*iter)->GetObjCountNumber() , *iter));
		}
	}	
}

