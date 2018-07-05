#pragma once
#include "ui.h"

class CUI_RoomList :
	public CUI
{
private:
	int		m_cursel;
	list<S_PT_ROOMLIST_RENEWAL_M*>	m_roomlist;
public:
	void	AddRoomTitle(const S_PT_ROOMLIST_RENEWAL_M& roominfo);
	void	EraseRoom(const S_PT_ROOMLIST_ERASE_M& roominfo);
	int		Getcursel(void);
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	bool UI_ptinrect(const D3DXVECTOR2 vpos);
	void UI_Reaction(void);
	void Init_State(void);
	void Release(void);
public:
	CUI_RoomList(void);
	~CUI_RoomList(void);
};
