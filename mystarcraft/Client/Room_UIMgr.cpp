#include "StdAfx.h"
#include "Room_UIMgr.h"

#include "KeyMgr.h"

#include "UIRoot.h"
#include "UI_IME.h"
#include "UI_Join.h"
#include "UI_IDslot.h"
#include "UI_RoomExitBtn.h"
#include "UI_CreateRoom.h"
#include "UI_RoomInfo.h"
#include "UI_Chat.h"
#include "UI_IME.h"
#include "SceneMgr.h"
#include "Session_Mgr.h"

IMPLEMENT_SINGLETON(CRoom_UIMgr);
CRoom_UIMgr::CRoom_UIMgr(void)
{
}

CRoom_UIMgr::~CRoom_UIMgr(void)
{
	Release();
}

void CRoom_UIMgr::Initialize(void)
{
	ZeroMemory( m_slotlist , sizeof(CUI_IDslot*)*8 );

	m_userlist = new CUIRoot;
	m_userlist->Set_texturekey(L"LIST");
	m_userlist->SetStartEndPos(D3DXVECTOR2( -364 , 0.f) , 
		D3DXVECTOR2( 0.f , 0.f));
	m_userlist->SetColor(D3DCOLOR_ARGB(180,255,255,255));
	m_userlist->Initialize();
	m_list.push_back(m_userlist);
	m_rootlist.push_back(m_userlist);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	m_chat = new CUIRoot;
	m_chat->Set_texturekey(L"CHAT");
	m_chat->SetStartEndPos(D3DXVECTOR2( -364 , 306.f) , 
		D3DXVECTOR2( 0.f , 306.f));
	m_chat->SetColor(D3DCOLOR_ARGB(180,255,255,255));
	m_chat->Initialize();
	m_list.push_back(m_chat);
	m_rootlist.push_back(m_chat);

	m_chat_ime = new CUI_IME(D3DXVECTOR2(20 , 145) , D3DXVECTOR2(355,22));
	m_chat_ime->SetParentMat(m_chat->GetMatrix_Adress());
	m_chat_ime->Initialize();
	m_list.push_back(m_chat_ime);

	m_chatlist = new CUI_Chat;
	m_chatlist->SetPos(D3DXVECTOR2(20,105));
	m_chatlist->SetParentMat(m_chat->GetMatrix_Adress());
	m_chatlist->Initialize();
	m_list.push_back(m_chatlist);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	m_roominfo = new CUIRoot;
	m_roominfo->Set_texturekey(L"INFO");
	m_roominfo->SetStartEndPos(D3DXVECTOR2((float)BACKBUFFER_SIZEX  , 0.f) , 
		D3DXVECTOR2( BACKBUFFER_SIZEX -240 , 0.f));
	m_roominfo->SetColor(D3DCOLOR_ARGB(180,255,255,255));
	m_roominfo->Initialize();
	m_list.push_back(m_roominfo);
	m_rootlist.push_back(m_roominfo);

	m_roominfo_text = new CUI_RoomInfo;
	m_roominfo_text->SetPos(D3DXVECTOR2(0,0));
	m_roominfo_text->SetParentMat(m_roominfo->GetMatrix_Adress());
	m_roominfo_text->Initialize();
	m_list.push_back(m_roominfo_text);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	m_start_btn = new CUIRoot;
	m_start_btn->Set_texturekey(L"OK");
	m_start_btn->SetStartEndPos(D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 4 , (float)BACKBUFFER_SIZEY / 1.4f) , 
		D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 204, (float)BACKBUFFER_SIZEY / 1.4f));
	m_start_btn->SetColor(D3DCOLOR_ARGB(180,255,255,255));
	m_start_btn->Initialize();
	m_list.push_back(m_start_btn);
	m_rootlist.push_back(m_start_btn);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	m_exitbtn_root = new CUIRoot;
	m_exitbtn_root->Set_texturekey(L"CANCLE");
	m_exitbtn_root->SetStartEndPos(D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 168, (float)BACKBUFFER_SIZEY) , 
		D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 168, (float)BACKBUFFER_SIZEY / 1.3f));
	m_exitbtn_root->SetColor(D3DCOLOR_ARGB(180,255,255,255));
	m_exitbtn_root->Initialize();

	m_roomexit_btn = new CUI_RoomExitBtn;
	m_roomexit_btn->SetParentMat(m_exitbtn_root->GetMatrix_Adress());
	m_roomexit_btn->SetPos(D3DXVECTOR2(5,63));	
	m_roomexit_btn->Initialize();
	m_list.push_back(m_roomexit_btn);
	m_list.push_back(m_exitbtn_root);
	m_rootlist.push_back(m_exitbtn_root);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	for(int i = 0; i < 8; ++i)
	{
		m_slotlist[i] = new CUI_IDslot;
		m_slotlist[i]->SetParentMat(m_userlist->GetMatrix_Adress());
		m_slotlist[i]->SetPos(D3DXVECTOR2(60.f , float(60 + i*19) ));
		m_slotlist[i]->Initialize();
		m_list.push_back(m_slotlist[i]);
	}
}

void CRoom_UIMgr::Update(void)
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
			m_ready_exit = false;
			if(R_ROOM_EXIT == m_flag)
			{
				CSceneMgr::GetInstance()->SetScene(SCENE_LOBY);
			}			
		}
	}
}

void CRoom_UIMgr::Render(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for( ; iter != iter_end; ++iter)
		(*iter)->Render();
}

void CRoom_UIMgr::Release(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete((*iter));

	m_list.clear();
}

void CRoom_UIMgr::Exit(EXIT_FLAG eflag)
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

	m_flag = eflag;
}

void CRoom_UIMgr::UI_Reaction(const D3DXVECTOR2& vpt)
{
	list<CUI*>::reverse_iterator riter = m_list.rbegin();
	list<CUI*>::reverse_iterator riter_end = m_list.rend();
	for( ; riter != riter_end; ++riter)
		(*riter)->Init_State();

	riter = m_list.rbegin();
	riter_end = m_list.rend();
	for( ; riter != riter_end; ++riter)
	{
		if( (*riter)->UI_ptinrect(vpt) )
			break;
	}
}

void CRoom_UIMgr::SetUserID(const WCHAR* szid , int idx)
{
	m_slotlist[idx]->SetUser(szid);
}

void CRoom_UIMgr::SetRoomTitle(const TCHAR* sztitle)
{
	m_roominfo_text->SetTitle(sztitle);
}
void CRoom_UIMgr::SendChat(void)
{
	if(GetFocus() == m_chat_ime->GetHandle())
	{
		WCHAR szmessage[32] = L"";
		SESSION_INFO psession_info = CSession_Mgr::GetInstance()->GetSession_Info();
		wcscpy_s(szmessage , m_chat_ime->GetMessage());
		WRITE_TCP_PACKET(PT_ROOM_SENDCHAT , WriteBuffer,
			WRITE_PT_ROOM_SENDCHAT(WriteBuffer,
			psession_info.USER_ID ,szmessage , psession_info.SESSION_ID));

		m_chat_ime->TextClear();
	}
}

void CRoom_UIMgr::Receive_Chat(const S_PT_ROOM_RECEIVE_CHAT_M& pinfo)
{
	m_chatlist->PushMessage(pinfo);
}
