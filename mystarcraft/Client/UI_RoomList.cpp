#include "StdAfx.h"
#include "UI_RoomList.h"

#include "FontMgr.h"
#include "MouseMgr.h"
CUI_RoomList::CUI_RoomList(void)
{
}

CUI_RoomList::~CUI_RoomList(void)
{
	Release();
}

void CUI_RoomList::Initialize(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	m_cursel = -1;
}

void CUI_RoomList::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	MYRECT<float>	rc;
	D3DCOLOR ecolor;

	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter = m_roomlist.begin();
	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter_end = m_roomlist.end();
	int i = 0;
	for(; iter != iter_end; ++iter)
	{
		rc.left = m_vpos.x;
		rc.right = m_vpos.x + 280;
		rc.top = m_vpos.y;
		rc.bottom = m_vpos.y + (i+1)*20;

		if( m_cursel == i)
			ecolor = D3DCOLOR_ARGB(255, 0, 255, 0);
		else if(MyPtInrect(CMouseMgr::GetInstance()->GetScreenMousePt() , &rc))
			ecolor = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			ecolor = D3DCOLOR_ARGB(255,85,185,23);

		CFontMgr::GetInstance()->Setbatch_Font( (*iter)->TITLE , m_matworld._41 ,
			m_matworld._42 + i * 20 , ecolor, true);

		++i;
	}
}

void CUI_RoomList::Render(void)
{

}

bool CUI_RoomList::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	MYRECT<float>	rc;

	m_cursel = -1;
	for(DWORD i = 0; i < m_roomlist.size(); ++i)
	{
		rc.left = m_vpos.x;
		rc.right = m_vpos.x + 280;
		rc.top = m_vpos.y;
		rc.bottom = m_vpos.y + (i+1)*20;

		if(MyPtInrect(vpos , &rc))
			m_cursel = i;

		return true;
	}
	return false;
}

void CUI_RoomList::UI_Reaction(void)
{

}

void CUI_RoomList::Init_State(void)
{
}

void CUI_RoomList::Release(void)
{
	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter = m_roomlist.begin();
	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter_end = m_roomlist.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete( *iter );

	m_roomlist.clear();
}
void CUI_RoomList::EraseRoom(const S_PT_ROOMLIST_ERASE_M& roominfo)
{
	if(m_roomlist.empty())
		return;

	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter = m_roomlist.begin();
	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter_end = m_roomlist.end();

	for( ; iter != iter_end;)
	{
		if( (*iter)->ROOM_IDX == roominfo.ROOM_IDX )
		{
			Safe_Delete(*iter);
			iter = m_roomlist.erase(iter);
		}
		else
			++iter;
	}
}
void CUI_RoomList::AddRoomTitle(const S_PT_ROOMLIST_RENEWAL_M& roominfo)
{
	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter = m_roomlist.begin();
	list<S_PT_ROOMLIST_RENEWAL_M*>::iterator iter_end = m_roomlist.end();

	for( ; iter != iter_end; ++iter)
	{
		if( (*iter)->ROOM_IDX == roominfo.ROOM_IDX )
			return;
	}

	S_PT_ROOMLIST_RENEWAL_M* temp = new S_PT_ROOMLIST_RENEWAL_M;
	memcpy(temp , &roominfo , sizeof(S_PT_ROOMLIST_RENEWAL_M));
	m_roomlist.push_back(temp);
}

int CUI_RoomList::Getcursel(void)
{
	return m_cursel;
}
