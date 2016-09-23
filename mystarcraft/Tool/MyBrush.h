#pragma once

#include "Include.h"
class CMyBrush
{
protected:
	int		m_curFloor;
	int		m_curTerrainID;
public:
	virtual void BrushPaint(){};
public:
	CMyBrush(void);
	virtual ~CMyBrush(void);
};
