#include "StdAfx.h"
#include "UI_IME.h"

#include "Client.h"
#include "TextureMgr.h"
#include "Device.h"
#include "FontMgr.h"
CUI_IME::CUI_IME(D3DXVECTOR2 vpos , D3DXVECTOR2 vsize )
{
	m_vpos = vpos;
	m_vsize = vsize;
}

CUI_IME::~CUI_IME(void)
{
	Release();
}
void CUI_IME::Initialize(void)
{  
	m_InputBufferEdit = CreateWindow(L"Edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
		2000,2000,200,30,g_hWnd,(HMENU)100,g_hInst,NULL);

	m_InputEdit = CreateWindow(L"Edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
		2000,2000,200,30,g_hWnd,(HMENU)100,g_hInst,NULL);

	PostMessage(m_InputEdit, EM_LIMITTEXT, (WPARAM)30, 0);

	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile1x1");

	if(NULL != m_texinfo)
	{
		m_vcenter.x = 0.f;//float(m_texinfo->ImgInfo.Width) / 2;
		m_vcenter.y = 0.f;//float(m_texinfo->ImgInfo.Height) / 2;
		m_vcenter.z = 0.f;
	}
	else
		m_vcenter = D3DXVECTOR3(0,0,0);

	D3DXMatrixIdentity(&m_matfont);

	m_matworld._11 = m_vsize.x;
	m_matworld._22 = m_vsize.y;
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	m_matfont._41 = m_vpos.x;
	m_matfont._42 = m_vpos.y;

	lstrcpy(m_text , L"");



	g_hEdit1 = m_InputBufferEdit;
	g_hEdit2 = m_InputEdit;
	g_OldEditProc1 = (WNDPROC)SetWindowLong(m_InputBufferEdit,GWL_WNDPROC,(LONG)EditSubProc1);//입력내용 저장
	g_OldEditProc2 = g_OldEditProc1;
	//(WNDPROC)SetWindowLong(m_InputEdit,GWL_WNDPROC,(LONG)EditSubProc2);//입력받는 프로시저
}

void CUI_IME::Update(void)
{
	if(GetFocus() == m_InputEdit) 
		GetWindowTextW(m_InputEdit, m_text, 32); 

	m_matworld._11 = m_vsize.x;
	m_matworld._22 = m_vsize.y;
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	if(NULL != m_parentmat)
	{
		m_matworld *= *m_parentmat;
		m_matfont._41 = m_matworld._41;
		m_matfont._42 = m_matworld._42;
	}

	//CFontMgr::GetInstance()->Setbatch_Font(m_text ,m_matfont._41 , m_matfont._42);
}

void CUI_IME::Render(void)
{	


	RECT rc = {0};
	if(NULL != m_texinfo)
	{		
		m_pSprite->SetTransform(&m_matworld);
		m_pSprite->Draw(m_texinfo->pTexture
			, NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
	}


	m_pSprite->SetTransform(&m_matfont);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
		, m_text , lstrlen(m_text) , &rc , DT_NOCLIP
		, D3DCOLOR_ARGB(255,0,255,0));


	int Start_sel;  
	int End_sel;  
	SendMessage(m_InputEdit, EM_GETSEL, (WPARAM)&Start_sel, (LPARAM)&End_sel);  
	m_text[End_sel] = L'\0'; 
	// 0.5초마다 깜빡거리게! (채팅창이 활성화 되어있을 경우에만)  
	if(GetFocus() == m_InputEdit)  
	{  
		// 채팅 좌표 + rt.right 위치에 Draw!  

	} 
}

void CUI_IME::Release(void)
{
	lstrcpy(m_text , L"");

	SetWindowLong(m_InputBufferEdit,GWL_WNDPROC,(LONG)g_OldEditProc1);
	SetWindowLong(m_InputEdit,GWL_WNDPROC,(LONG)g_OldEditProc2);	
}

bool CUI_IME::UI_ptinrect(const D3DXVECTOR2 vpos)
{

	//m_rect.left = m_matworld._41 - m_vsize.x/2;
	//m_rect.right = m_matworld._41 + m_vsize.x/2;
	//m_rect.top = m_matworld._42 - m_vsize.y/2;
	//m_rect.bottom = m_matworld._42 + m_vsize.y/2;

	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + m_vsize.x;
	m_rect.top = m_matworld._42;
	m_rect.bottom = m_matworld._42 + m_vsize.y;

	if(MyPtInrect(vpos , &m_rect))
	{
		UI_Reaction();
		return true;
	}

	return false;
}

void CUI_IME::UI_Reaction(void)
{
	SetFocus(m_InputEdit);

	g_hEdit1 = m_InputBufferEdit;
	g_hEdit2 = m_InputEdit;
	SetWindowLong(m_InputBufferEdit,GWL_WNDPROC,(LONG)EditSubProc1);//입력내용 저장
	SetWindowLong(m_InputEdit,GWL_WNDPROC,(LONG)EditSubProc2);//입력받는 프로시저
}
void CUI_IME::Init_State(void)
{
	SetFocus(g_hWnd);

	//SetWindowLong(g_hEdit2,GWL_WNDPROC,(LONG)g_OldEditProc2);
	//SetWindowLong(g_hEdit1,GWL_WNDPROC,(LONG)g_OldEditProc1);
}

void CUI_IME::TextClear(void)
{
	lstrcpy(m_text , L"");
	SetWindowTextW(m_InputEdit , m_text);
}

const TCHAR* CUI_IME::GetMessage(void)
{
	return m_text;
}
