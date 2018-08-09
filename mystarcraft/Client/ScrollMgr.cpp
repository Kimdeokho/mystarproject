#include "StdAfx.h"
#include "ScrollMgr.h"

IMPLEMENT_SINGLETON(CScrollMgr)
CScrollMgr::CScrollMgr(void)
{
}

CScrollMgr::~CScrollMgr(void)
{
}

bool CScrollMgr::inside_camera(const float& fx , const float& fy)
{
	/*fx , fy클라이언트 좌표여야 한다 0~800 , 0~600.*/

	if(fx <= 5 || fx >= BACKBUFFER_SIZEX - 5 || fy <= 20 || fy >= BACKBUFFER_SIZEY - 20)
		return false;

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