
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "ToolDoc.h"
#include "ToolView.h"
#include "Device.h"

#include "TileMgr.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "TerrainBrushMgr.h"
#include "TileDebug.h"
#include "MyMouse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
//	CTerrainBrushMgr::GetInstance()->Release();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	CDevice::GetInstance()->GetDevice()->Clear(0, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_XRGB(0,0,255)/*0xff0000ff*/, 1.f, 0);


	CDevice::GetInstance()->Render_Begin();
	//CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);;

	CTileMgr::GetInstance()->TileRender();
	if(m_bGrid == true)
		CTileMgr::GetInstance()->ShowGrid();
	CTileMgr::GetInstance()->Rohmbus_Render();

	CTileDebug::GetInstance()->DebugRender();

	//CDevice::GetInstance()->GetSprite()->End();
	CDevice::GetInstance()->Render_End();


	CDevice::GetInstance()->GetDevice()->Present(NULL, NULL, m_hWnd, NULL);
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();


	srand((unsigned int)time(NULL));


	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.

	SetScrollSizes(MM_TEXT, CSize(SQ_TILECNTX*SQ_TILESIZEX , SQ_TILECNTY*SQ_TILESIZEY));

	m_pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow;
	m_pMainFrm->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0
		, rcWindow.right - rcWindow.left
		, rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	m_pMainFrm->SetWindowPos(NULL
		, 100, 100, int(WINSIZE_X + fRowFrm), int(WINSIZE_Y + fColFrm)
		, SWP_NOZORDER);

	g_hWnd = m_hWnd;
	if(FAILED(CDevice::GetInstance()->InitDevice()))
	{
		AfxMessageBox(L"����̽� �ʱ�ȭ ����");
		return;
	}

	if(CTextureMgr::GetInstance()->ReadImagePath(L"../Data/ImgPath.txt"))
	{
		MessageBox(L"�ؽ��� �ҷ����� ����");
	}
	if(CTextureMgr::GetInstance()->SingleReadImagePath(L"../Data/SingleImgPath.txt"))
	{
		MessageBox(L"�̱��ؽ��� �ҷ����� ����");
	}

	CTileMgr::GetInstance()->InitTile();
	CTerrainBrushMgr::GetInstance()->Initialize();
	CMyMouse::GetInstance()->Initialize();

	m_bGrid = false;
	m_bLbutton = false;
	m_DebugMode = false;
	m_OldClickIdx = 0;
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����

BOOL CToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return false;//CScrollView::OnEraseBkgnd(pDC);
}

void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch(nChar)
	{
	case 'G':
		{
			if(m_bGrid == true)
				m_bGrid = false;
			else
				m_bGrid = true;

			Invalidate(TRUE);
			break;
		}
	case 'D':
		{
			CTileDebug::GetInstance()->SetDebugGroup();
			//CTileDebug::GetInstance()->DebugTile_PosSet();
			break;
		}
	case 'M':
		{
			CTileDebug::GetInstance()->SetMoveOption();
			break;
		}
	case 'Z':
		{
			CRewind::GetInstance()->StackRewind();
			break;
		}
	}

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	CScrollView::OnMouseMove(nFlags, point);

	CMyMouse::GetInstance()->SetMousePt(point);

	CTileMgr::GetInstance()->Rohmbus_Picking(point);

	int newidx = CTerrainBrushMgr::GetInstance()->get_sqindex();
	if(m_bLbutton == true)
	{
		if(m_OldClickIdx != newidx)
		{
			m_OldClickIdx = newidx;
			if(CTerrainBrushMgr::GetInstance()->TerrainCheck())
			{
				MessageBox(L"���� üũ ����");
			}
		}
	}

	CTileDebug::GetInstance()->DebugTile_PosSet();


	Invalidate(FALSE);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	m_bLbutton = true;

	m_OldClickIdx = CTerrainBrushMgr::GetInstance()->get_sqindex();
	if(CTerrainBrushMgr::GetInstance()->TerrainCheck())
	{
		MessageBox(L"���� üũ ����");
	}

	Invalidate(FALSE);
}

void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonUp(nFlags, point);
	m_bLbutton = false;
}
