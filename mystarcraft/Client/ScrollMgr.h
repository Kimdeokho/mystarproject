#pragma once

#include "Include.h"

class CScrollMgr
{
public:
	DECLARE_SINGLETON(CScrollMgr)
public:
	static float	m_fScrollX;
	static float	m_fScrollY;
public:
	//bool inside_camera(const float& fx , const float& fy);
	static void update(void);
private:
	CScrollMgr(void);
public:
	~CScrollMgr(void);
};
