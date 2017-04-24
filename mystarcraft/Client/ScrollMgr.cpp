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
	/*fx , fy�� ��ũ�Ѱ��� �Կ����� ���� ���� pos���̾�� �Ѵ�.*/

	if(fx < m_fScrollX - 50 || m_fScrollX + CLINETSIZE_X + 50 < fx)
		return false;
	if(fy < m_fScrollY - 50 || m_fScrollY + CLINETSIZE_Y + 50 < fy)
		return false;

	return true;
}

float CScrollMgr::m_fScrollX(1000);
float CScrollMgr::m_fScrollY(1000);