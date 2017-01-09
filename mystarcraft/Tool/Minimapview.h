#pragma once


// CMinimapview 뷰입니다.

class CMainFrame;
class CToolView;
class CMinimapview : public CView
{
	DECLARE_DYNCREATE(CMinimapview)

protected:
	CMinimapview();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMinimapview();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	CMainFrame*	m_pMainfrm;
	CToolView*	m_pToolView;
public:
	bool		m_bLbtnClick;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


