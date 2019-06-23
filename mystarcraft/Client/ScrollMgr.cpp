#include "StdAfx.h"
#include "ScrollMgr.h"

IMPLEMENT_SINGLETON(CScrollMgr)
CScrollMgr::CScrollMgr(void)
{
}

CScrollMgr::~CScrollMgr(void)
{
}

bool CScrollMgr::inside_camera(const float fx , const float fy)
{
	if( fx >= m_fScrollX - 100 && 
		fx <= m_fScrollX + CLINETSIZE_X + 100 &&
		fy >= m_fScrollY - 100 &&
		fy <= m_fScrollY + CLINETSIZE_Y + 100)
	{
		return true;
	}

	return true;
}
void CScrollMgr::update(void)
{
	if(0 >= m_fScrollX)
		m_fScrollX = 0;
	else if(4096 - BACKBUFFER_SIZEX - 0 <= m_fScrollX)
		m_fScrollX = 4096 - BACKBUFFER_SIZEX - 0;

	if(0 >= m_fScrollY)
		m_fScrollY = 0;
	else if(4096 - BACKBUFFER_SIZEY - 0 <= m_fScrollY)
		m_fScrollY = 4096 - BACKBUFFER_SIZEY - 0;
}
float CScrollMgr::m_fScrollX(0);
float CScrollMgr::m_fScrollY(0);