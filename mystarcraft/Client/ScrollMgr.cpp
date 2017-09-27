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

	if(fx < 0 || fx > BACKBUFFER_SIZEX || fy < 0 || fy > BACKBUFFER_SIZEY)
		return false;

	return true;
}
void CScrollMgr::update(void)
{
	if(0 > m_fScrollX)
		m_fScrollX = 0;
	else if(4096 - BACKBUFFER_SIZEX <= m_fScrollX)
		m_fScrollX = 4096 - BACKBUFFER_SIZEX;

	if(0 > m_fScrollY)
		m_fScrollY = 0;
	else if(4096 - BACKBUFFER_SIZEY <= m_fScrollY)
		m_fScrollY = 4096 - BACKBUFFER_SIZEY;
}
float CScrollMgr::m_fScrollX(1000);
float CScrollMgr::m_fScrollY(1000);