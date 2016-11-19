#pragma once

#include "MyBrush.h"
class CHillBrush : public CMyBrush
{
public:
	virtual void BrushPaint();
public:
	void SetHillDir(HILL_DIR edir);
private:
	HILL_DIR	m_eHillDir;
public:
	CHillBrush(void);
	virtual ~CHillBrush(void);
};
