#include "StdAfx.h"
#include "Debug_Mgr.h"
#include "KeyMgr.h"
#include "TileManager.h"
#include "Session_Mgr.h"

IMPLEMENT_SINGLETON(CDebug_Mgr)

CDebug_Mgr::CDebug_Mgr(void)
{
	memset(m_oldfog , 0 , sizeof(FOG_INFO)*SQ_TILECNTX*SQ_TILECNTY);
}

CDebug_Mgr::~CDebug_Mgr(void)
{
}

void CDebug_Mgr::Update(void)
{
	if(CKeyMgr::GetInstance()->GetOnceKeyDown_Check(VK_F1))
	{
		if(false == m_dbglist[DBG_COLLBOX])
			m_dbglist[DBG_COLLBOX] = true;
		else
			m_dbglist[DBG_COLLBOX] = false;

	}
	if(CKeyMgr::GetInstance()->GetOnceKeyDown_Check(VK_F2))
	{
		if(false == m_dbglist[DBG_FOG])
			m_dbglist[DBG_FOG] = true;
		else
			m_dbglist[DBG_FOG] = false;
	}
	if(CKeyMgr::GetInstance()->GetOnceKeyDown_Check(VK_F3))
	{
		if(false == m_dbglist[DBG_TILEPOP])
			m_dbglist[DBG_TILEPOP] = true;
		else
			m_dbglist[DBG_TILEPOP] = false;
	}
	if(CKeyMgr::GetInstance()->GetOnceKeyDown_Check(VK_F4))
	{
		if(false == m_dbglist[DBG_PATHFIND])
			m_dbglist[DBG_PATHFIND] = true;
		else
			m_dbglist[DBG_PATHFIND] = false;
	}
	if(CKeyMgr::GetInstance()->GetOnceKeyDown_Check(VK_F5))
	{
		if(false == m_dbglist[DBG_FLOWPATH])
			m_dbglist[DBG_FLOWPATH] = true;
		else
			m_dbglist[DBG_FLOWPATH] = false;
	}
}

void CDebug_Mgr::DBG_Fog_on(void)
{
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
	FOG_INFO** info = CTileManager::GetInstance()->GetFogTile();

	for(int i = 0; i < SQ_TILECNTX*SQ_TILECNTY; ++i)
	{
		memcpy( &m_oldfog[i] , info[i] , sizeof(FOG_INFO));

		info[i]->eSight[eteam] = FOG_ALPHA;
		info[i]->fog_color[eteam] = D3DCOLOR_ARGB(0,0,0,0);
	}
}

void CDebug_Mgr::DBG_Fog_off(void)
{
	FOG_INFO** info = CTileManager::GetInstance()->GetFogTile();

	for(int i = 0; i < SQ_TILECNTX*SQ_TILECNTY; ++i)
	{
		memcpy( info[i] , &m_oldfog[i] , sizeof(FOG_INFO));
	}
}

bool CDebug_Mgr::m_dbglist[] = {0};


