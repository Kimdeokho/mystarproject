#pragma once
#include "ui.h"

class CUI_Chat :
	public CUI
{
private:
	typedef list<S_PT_ROOM_RECEIVE_CHAT_M*>::iterator MESSAGE_ITER;
	typedef list<S_PT_ROOM_RECEIVE_CHAT_M*>::reverse_iterator MESSAGE_RITER;
	list<S_PT_ROOM_RECEIVE_CHAT_M*>		m_message_list;

	DWORD_PTR	m_mysession_id;
public:
	void	PushMessage(const S_PT_ROOM_RECEIVE_CHAT_M& _message);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Release(void);
public:
	CUI_Chat(void);
	~CUI_Chat(void);
};
