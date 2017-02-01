#pragma once

#include "Include.h"

class CScrollMgr
{
public:
	DECLARE_SINGLETON(CScrollMgr)
public:
	static float	m_fScrollX;
	static float	m_fScrollY;
private:
	CScrollMgr(void);
public:
	~CScrollMgr(void);
};
