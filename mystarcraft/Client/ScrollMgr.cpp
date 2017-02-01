#include "StdAfx.h"
#include "ScrollMgr.h"

IMPLEMENT_SINGLETON(CScrollMgr)
CScrollMgr::CScrollMgr(void)
{
}

CScrollMgr::~CScrollMgr(void)
{
}
float CScrollMgr::m_fScrollX(0);
float CScrollMgr::m_fScrollY(0);