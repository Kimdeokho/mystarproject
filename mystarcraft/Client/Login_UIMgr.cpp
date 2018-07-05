#include "StdAfx.h"
#include "Login_UIMgr.h"

#include "KeyMgr.h"

#include "UIRoot.h"
#include "UI_IME.h"
#include "UI_Connect.h"
#include "SceneMgr.h"
#include "Session_Mgr.h"
#include "TestSession.h"
IMPLEMENT_SINGLETON(CLogin_UIMgr)

CLogin_UIMgr::CLogin_UIMgr(void)
{
}

CLogin_UIMgr::~CLogin_UIMgr(void)
{
	Release();
}

void CLogin_UIMgr::Initialize(void)
{
	//144, 76
	CUIRoot* pui = new CUIRoot;
	pui->Set_texturekey(L"LOGIN_IME");
	pui->SetStartEndPos(D3DXVECTOR2( float(BACKBUFFER_SIZEX/2) - 144 , float(BACKBUFFER_SIZEY + 200)) , 
		D3DXVECTOR2( float(BACKBUFFER_SIZEX/2) - 144 , float(BACKBUFFER_SIZEY/2 - 76)));
	pui->SetColor(D3DCOLOR_ARGB(170,255,255,255));
	pui->Initialize();

	m_logime = new CUI_IME(D3DXVECTOR2(20,45) , D3DXVECTOR2(250,20));
	m_logime->SetParentMat(pui->GetMatrix_Adress());
	m_logime->Initialize();	

	//CUI* ptemp = new CUI_IME(D3DXVECTOR2(20,75) , D3DXVECTOR2(250,20));
	//ptemp->SetParentMat(pui->GetMatrix_Adress());
	//ptemp->Initialize();
	

	m_log_connect = new CUI_Connect(D3DXVECTOR2(144 , 120));
	m_log_connect->SetParentMat(pui->GetMatrix_Adress());
	m_log_connect->Initialize();	

	m_list.push_back(pui);
	//m_list.push_back(ptemp);
	m_list.push_back(m_logime);
	m_list.push_back(m_log_connect);

	m_rootlist.push_back(pui);
}
void CLogin_UIMgr::Update(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for( ; iter != iter_end; ++iter)
		(*iter)->Update();

	if(true == m_ready_exit)
	{
		bool is_check = false;
		list<CUIRoot*>::iterator iter = m_rootlist.begin();
		list<CUIRoot*>::iterator iter_end = m_rootlist.end();
		for( ; iter != iter_end; ++iter)
		{
			if( (*iter)->GetExitComplete() )
				is_check = true;
			else
			{
				is_check = false;
				break;
			}
		}
		if(is_check)
		{
			CTestSession*  psession = CSession_Mgr::GetInstance()->GetTCP_Session();
			psession->WritePacket(PT_LOBY_ENTER , WriteBuffer , WRITE_PT_LOBY_ENTER(
				WriteBuffer , m_logime->GetMessage() , L"127.0.0.1" , DEFAULT_PORT ));

			m_ready_exit = false;
			CSceneMgr::GetInstance()->SetScene(SCENE_LOBY);
		}
	}
}
void CLogin_UIMgr::Render(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for( ; iter != iter_end; ++iter)
		(*iter)->Render();
}

void CLogin_UIMgr::Release(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete((*iter));

	m_list.clear();
}
void CLogin_UIMgr::Exit(void)
{
	bool is_check = false;

	list<CUIRoot*>::iterator iter = m_rootlist.begin();
	list<CUIRoot*>::iterator iter_end = m_rootlist.end();
	for( ; iter != iter_end; ++iter)
	{
		if( !(*iter)->GetEntryComplete() )
		{
			is_check = true;
		}
	}

	if(!is_check)
	{
		iter = m_rootlist.begin();
		iter_end = m_rootlist.end();
		for( ; iter != iter_end; ++iter)
			(*iter)->SetExit(true);

		m_ready_exit = true;
	}
}
void CLogin_UIMgr::UI_Reaction(const D3DXVECTOR2& vpt)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Init_State();
	}

	iter = m_list.begin();
	iter_end = m_list.end();
	for( ; iter != iter_end; ++iter)
	{
		if( (*iter)->UI_ptinrect(vpt) )
			break;
	}
}

void CLogin_UIMgr::Connect_Server(void)
{
	if(CSession_Mgr::GetInstance()->BeginTCP())
	{
		CLogin_UIMgr::GetInstance()->Exit();
	}
}
