
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	//cs.cx = 1024;
	//cs.cy = 768;
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_MainSplitter.CreateStatic(this , 1 , 2);
	m_SubSplitter.CreateStatic(&m_MainSplitter , 2,1);

	m_MainSplitter.CreateView(0,1 , RUNTIME_CLASS(CToolView), CSize(800,600) , pContext);
	m_SubSplitter.CreateView(0,0 , RUNTIME_CLASS(CMinimapview) , CSize(300,225) , pContext);
	m_SubSplitter.CreateView(1,0 , RUNTIME_CLASS(MyForm) , CSize(300,375) , pContext);

	m_pToolView = (CToolView*)m_MainSplitter.GetPane(0,1);
	m_pMiniMapView = (CMinimapview*)m_SubSplitter.GetPane(0,0);
	m_pMyFormView = (MyForm*)m_SubSplitter.GetPane(1,0);

	m_MainSplitter.SetColumnInfo(0,300,10);
	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	return TRUE;
}
