#include "StdAfx.h"
#include "Loby_UIMgr.h"

#include "KeyMgr.h"

#include "UIRoot.h"
#include "UI_IME.h"
#include "UI_Join.h"
#include "UI_CreateRoom.h"
#include "UI_RoomPopup.h"
#include "UI_RoomList.h"

#include "SceneMgr.h"
#include "Session_Mgr.h"
#include "TestSession.h"
IMPLEMENT_SINGLETON(CLoby_UIMgr)
CLoby_UIMgr::CLoby_UIMgr(void)
{
}

CLoby_UIMgr::~CLoby_UIMgr(void)
{
	Release();
}

void CLoby_UIMgr::Initialize(void)
{
	//144, 76
	CUIRoot* pui = new CUIRoot;
	pui->Set_texturekey(L"ROOMLIST");
	pui->SetStartEndPos(D3DXVECTOR2( -364 , 0.f) , 
		D3DXVECTOR2( 0.f , 0.f));
	pui->SetColor(D3DCOLOR_ARGB(180,255,255,255));
	pui->Initialize();
	m_list.push_back(pui);
	m_rootlist.push_back(pui);

	m_createroom = new CUI_CreateRoom( D3DXVECTOR2( 44 , 332) );
	m_createroom->SetParentMat(pui->GetMatrix_Adress());
	m_createroom->Initialize();
	m_list.push_back(m_createroom);

	m_RoomList = new CUI_RoomList;
	m_RoomList->SetPos(D3DXVECTOR2(70 , 100));
	m_RoomList->SetParentMat(pui->GetMatrix_Adress());
	m_RoomList->Initialize();	
	m_list.push_back(m_RoomList);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	pui = new CUIRoot;
	pui->Set_texturekey(L"");
	pui->SetStartEndPos(D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 4 , (float)BACKBUFFER_SIZEY / 1.5f) , 
		D3DXVECTOR2( (float)BACKBUFFER_SIZEX - 204, (float)BACKBUFFER_SIZEY / 1.5f));
	pui->Initialize();
	m_list.push_back(pui);
	m_rootlist.push_back(pui);

	m_join = new CUI_Join(D3DXVECTOR2(0,0));
	m_join->SetParentMat(pui->GetMatrix_Adress());
	m_join->Initialize();
	m_list.push_back(m_join);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	m_roompopup = new CUI_RoomPopup;
	m_roompopup->SetPos(D3DXVECTOR2(BACKBUFFER_SIZEX/2 - 180,BACKBUFFER_SIZEY/2 - 100));
	m_roompopup->SetColor((D3DCOLOR_ARGB(180,255,255,255)));
	m_roompopup->Initialize();
	m_list.push_back(m_roompopup);
	m_ready_exit = false;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	m_flag = LB_FLAG_NONE;
}

void CLoby_UIMgr::Update(void)
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
			if(LB_ROOM_JOIN == m_flag)
			{
				int roomidx = m_RoomList->Getcursel();
				if(roomidx >= 0)
				{
					WRITE_TCP_PACKET(PT_ROOM_ENTER , WriteBuffer , WRITE_PT_ROOM_ENTER(WriteBuffer,
						USHORT(roomidx) ) );
					CSceneMgr::GetInstance()->SetScene(SCENE_ROOM);
				}				
			}
			else if(LB_ROOM_CREATE == m_flag)
			{
				const TCHAR* tsz = m_roompopup->GetRoomTitle();
				WRITE_TCP_PACKET(PT_CREATE_ROOM , WriteBuffer , WRITE_PT_CREATE_ROOM(WriteBuffer
					,tsz));

				CSceneMgr::GetInstance()->SetScene(SCENE_ROOM);
			}
		}
	}
}

void CLoby_UIMgr::Render(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for( ; iter != iter_end; ++iter)
		(*iter)->Render();
}

void CLoby_UIMgr::UI_Reaction(const D3DXVECTOR2& vpt)
{
	//지금 클릭된걸 제일 먼저 올라오게


	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();

	list<CUI*>	templist;
	CUI* ptemp_ui = NULL;
	for( ; iter != iter_end; ++iter)
	{
		if( MyPtInrect(vpt , &(*iter)->GetMyRect()) )
		{
			ptemp_ui = (*iter);
			m_list.erase(iter);
			break;
		}
	}

	////////////////////////////////////////////////////////////////////

	iter = m_list.begin();
	iter_end = m_list.end();

	if(NULL != ptemp_ui)
		m_list.push_back(ptemp_ui);

	////////////////////////////////////////////////////////////////////

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

void CLoby_UIMgr::Release(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete((*iter));

	m_list.clear();
	m_rootlist.clear();
}

void CLoby_UIMgr::Exit(EXIT_FLAG eflag)
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

void CLoby_UIMgr::Room_Popup(bool is_popup)
{
	m_roompopup->SetActive(is_popup);
}
void CLoby_UIMgr::Create_Room(void)
{
	Exit(LB_ROOM_CREATE);
	Room_Popup(false);
}

void CLoby_UIMgr::Add_RoomTitle(const S_PT_ROOMLIST_RENEWAL_M& roominfo)
{
	m_RoomList->AddRoomTitle(roominfo);
}
void CLoby_UIMgr::Erase_Room(const S_PT_ROOMLIST_ERASE_M& roominfo)
{
	m_RoomList->EraseRoom(roominfo);
}
int	CLoby_UIMgr::GetRoomIdx(void)
{
	return m_RoomList->Getcursel();
}