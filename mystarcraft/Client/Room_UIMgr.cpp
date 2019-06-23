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
#include "UI_Combobox.h"
#include "UI_IME.h"
#include "UI_StartBtn.h"
#include "SceneMgr.h"
#include "Session_Mgr.h"
#include "RoomSession_Mgr.h"
#include "SoundDevice.h"

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
	m_userlist->SetStartEndPos(D3DXVECTOR2( -564 , 0.f) , 
		D3DXVECTOR2( 0.f , 0.f));
	m_userlist->SetColor(D3DCOLOR_ARGB(180,255,255,255));
	m_userlist->Initialize();
	m_list.push_back(m_userlist);
	m_rootlist.push_back(m_userlist);


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	m_chat = new CUIRoot;
	m_chat->Set_texturekey(L"CHAT");
	m_chat->SetStartEndPos(D3DXVECTOR2( -564 , 306.f) , 
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
	m_roominfo->SetStartEndPos(D3DXVECTOR2((float)BACKBUFFER_SIZEX + 200 , 0.f) , 
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

	DWORD_PTR d1 = CRoomSession_Mgr::GetInstance()->GetMasterSession_ID();
	DWORD_PTR d2 = CSession_Mgr::GetInstance()->GetSession_Info().SESSION_ID;
	if(CRoomSession_Mgr::GetInstance()->GetMasterSession_ID() ==
		CSession_Mgr::GetInstance()->GetSession_Info().SESSION_ID)
	{
		m_startbtn_root = new CUIRoot;
		m_startbtn_root->Set_texturekey(L"OK");
		m_startbtn_root->SetStartEndPos(D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 4 , (float)BACKBUFFER_SIZEY / 1.4f) , 
			D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 204, (float)BACKBUFFER_SIZEY / 1.4f));
		m_startbtn_root->SetColor(D3DCOLOR_ARGB(180,255,255,255));
		m_startbtn_root->Initialize();
		m_list.push_back(m_startbtn_root);
		m_rootlist.push_back(m_startbtn_root);

		m_startbtn = new CUI_StartBtn;
		m_startbtn->SetParentMat(m_startbtn_root->GetMatrix_Adress());
		m_startbtn->SetPos(D3DXVECTOR2(5,48));
		m_startbtn->Initialize();
		m_list.push_back(m_startbtn);
	}
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

		m_combobox[i] = new CUI_Combobox(i);
		m_combobox[i]->SetParentMat(m_userlist->GetMatrix_Adress());
		m_combobox[i]->SetPos(D3DXVECTOR2(240.f , float(60 + i*19) ));
		m_combobox[i]->AddData(L"Terran");
		m_combobox[i]->AddData(L"Zerg");
		m_combobox[i]->AddData(L"Random");
		m_combobox[i]->SetRepresentData(L"Random");
		m_combobox[i]->SetActive(false);
		m_combobox[i]->Initialize();
		
	}
	for(int i = 7; i >= 0; --i)
		m_list.push_back(m_combobox[i]);

	//SetUser(CSession_Mgr::GetInstance()->GetSession_Info().USER_ID , 0);
	SetRoomTitle(CRoomSession_Mgr::GetInstance()->GetTitle());
	

	WRITE_TCP_PACKET(PT_ROOM_USER_RENEWAL , WriteBuffer 
		, WRITE_PT_ROOM_USER_RENEWAL(WriteBuffer , L"Random"));

	//방입장하기전에 내가 원하는 종족이 자동으로 선택되어있느냐,
	//슬롯에 정해진 콤보박스의 데이터를 보내는가
	//어떻게 디자인하느냐에 따라 다르다 
	

	CSoundDevice::GetInstance()->PlayEffSound(SND_EFF_JOIN , 0);
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
			else if(R_STAGE_PRE == m_flag)
			{
				CSceneMgr::GetInstance()->SetScene(SCENE_STAGEPRE);
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
		CSoundDevice::GetInstance()->PlayEffSound(SND_EFF_EXIT , 0);

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

void CRoom_UIMgr::SetUser(const WCHAR* szid , int idx)
{
	m_slotlist[idx]->SetUser(szid);

	if(!lstrcmp(szid , L""))
		m_combobox[idx]->SetActive(false);
	else
		m_combobox[idx]->SetActive(true);
}

void CRoom_UIMgr::RoomExitUser(int idx)
{
	//이거외에 콤보박스 셀렉 초기화도 해야할듯
	m_combobox[idx]->SetActive(false);
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
			psession_info.USER_ID ,szmessage , psession_info.SESSION_ID , 0.f , -1.f));

		m_chat_ime->TextClear();
	}
}

void CRoom_UIMgr::Receive_Chat(const S_PT_ROOM_RECEIVE_CHAT_M& pinfo)
{
	m_chatlist->PushMessage(pinfo);
}

void CRoom_UIMgr::SetCombobox_ChangeData(const WCHAR* _race, const int& slotidx)
{
	m_combobox[slotidx]->SetRepresentData(_race);
}

void CRoom_UIMgr::Release(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete((*iter));

	m_list.clear();
}