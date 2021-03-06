
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TileMgr.h"
#include "ToolView.h"
#include "Minimapview.h"
#include "MyForm.h"
#include "TerrainBrushMgr.h"
#include "TileDebug.h"
#include "Rewind.h"
#include "MyMouse.h"
#include "ObjMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	CTextureMgr::GetInstance()->DestroyInstance();
	CTileMgr::GetInstance()->DestroyInstance();
	CTerrainBrushMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();
	CTileDebug::GetInstance()->DestroyInstance();
	CRewind::GetInstance()->DestroyInstance();
	CMyMouse::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	//cs.cx = 1024;
	//cs.cy = 768;
	return TRUE;
}

// CMainFrame 진단

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


// CMainFrame 메시지 처리기

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_MainSplitter.CreateStatic(this , 1 , 2);
	m_SubSplitter.CreateStatic(&m_MainSplitter , 2,1);

	m_MainSplitter.CreateView(0,1 , RUNTIME_CLASS(CToolView), CSize(1280,720) , pContext);
	m_SubSplitter.CreateView(0,0 , RUNTIME_CLASS(CMinimapview) , CSize(256,256) , pContext);
	m_SubSplitter.CreateView(1,0 , RUNTIME_CLASS(MyForm) , CSize(256,500) , pContext);

	m_pToolView = (CToolView*)m_MainSplitter.GetPane(0,1);
	m_pMiniMapView = (CMinimapview*)m_SubSplitter.GetPane(0,0);
	m_pMyFormView = (MyForm*)m_SubSplitter.GetPane(1,0);

	m_MainSplitter.SetColumnInfo(0,256,10);
	//return CFrameWnd::OnCreateClient(lpcs, pContext);






	return TRUE;
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMainFrame::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFrameWnd::PostNcDestroy();
}
