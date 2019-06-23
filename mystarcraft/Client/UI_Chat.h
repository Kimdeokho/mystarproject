#pragma once
#include "ui.h"

class CUI_Chat :
	public CUI
{
private:
	struct CHAT_TIME
	{
		float curtime;
		float retain_time;

		CHAT_TIME()
		{
			curtime = 0.f;
			retain_time = 0.f;
		}
	};
private:
	typedef list<S_PT_ROOM_RECEIVE_CHAT_M*>::iterator MESSAGE_ITER;
	typedef list<S_PT_ROOM_RECEIVE_CHAT_M*>::reverse_iterator MESSAGE_RITER;
	list<S_PT_ROOM_RECEIVE_CHAT_M*>		m_message_list;

	DWORD_PTR	m_mysession_id;
	int			m_chat_size;
public:
	void	PushMessage(const S_PT_ROOM_RECEIVE_CHAT_M& _message);
	void	SetChatSize(const int chatsize);
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
