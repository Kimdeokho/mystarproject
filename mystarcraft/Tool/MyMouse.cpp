#include "StdAfx.h"
#include "MyMouse.h"

#include "MainFrm.h"
#include "ToolView.h"

IMPLEMENT_SINGLETON(CMyMouse)
CMyMouse::CMyMouse(void)
{
	m_pToolView = NULL;
}

CMyMouse::~CMyMouse(void)
{
}

void CMyMouse::Initialize(void)
{
	m_pToolView = ((CMainFrame*)AfxGetMainWnd())->m_pToolView;
}
const CPoint& CMyMouse::GetMousePt(void)
{
	return m_Curpt;
}
const CPoint& CMyMouse::GetAddScroll(void)
{
	m_ptAddScroll.x = m_Curpt.x + m_pToolView->GetScrollPos(0);
	m_ptAddScroll.y = m_Curpt.y + m_pToolView->GetScrollPos(1);

	return m_ptAddScroll;
}
void CMyMouse::SetMousePt(const CPoint& _pt)
{
	m_Curpt = _pt;
}
