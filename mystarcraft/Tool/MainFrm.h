
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

//#include "ToolView.h"
//#include "Minimapview.h"
//#include "MyForm.h"


class MyForm;
class CToolView;
class CMinimapview;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	CSplitterWnd	m_MainSplitter;//메인 프레임 분할
	CSplitterWnd	m_SubSplitter; //서브 프레임 분할

	CToolView*		m_pToolView; //메인뷰
	CMinimapview*	m_pMiniMapView; //미니맵 뷰
	MyForm*			m_pMyFormView;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


