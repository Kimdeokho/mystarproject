#pragma once
#include "Include.h"

class CToolView;
class CMyMouse
{
	DECLARE_SINGLETON(CMyMouse)
public:
	void Initialize(void);
	void SetMousePt(const CPoint& _pt);
public:
	const CPoint& GetMousePt(void);
	const CPoint& GetAddScroll(void);
private:
	CToolView*	m_pToolView;
	CPoint		m_Curpt;
	CPoint		m_ptAddScroll;
public:
	CMyMouse(void);
	~CMyMouse(void);
};
