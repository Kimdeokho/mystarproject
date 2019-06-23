#include "StdAfx.h"
#include "UI_Chat.h"

#include "Session_Mgr.h"
#include "FontMgr.h"
#include "TimeMgr.h"
CUI_Chat::CUI_Chat(void)
{
	m_chat_size = 5;
}

CUI_Chat::~CUI_Chat(void)
{
	Release();
}

void CUI_Chat::Initialize(void)
{
	m_mysession_id = CSession_Mgr::GetInstance()->GetSession_Info().SESSION_ID;
}

void CUI_Chat::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y - 5;
	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	MESSAGE_ITER iter = m_message_list.begin();
	MESSAGE_ITER iter_end = m_message_list.end();

	TCHAR tszmessage[128] = L"";
	int i = 0;
	D3DCOLOR	ecolor;
	for( ; iter != iter_end; )
	{
		//S_PT_ROOM_RECEIVE_CHAT_M.MESSAGE;
		lstrcpy(tszmessage, L"");
		lstrcat(tszmessage , (*iter)->USER_ID);
		lstrcat(tszmessage , L" : ");
		lstrcat(tszmessage , (*iter)->MESSAGE);

		if(m_mysession_id == (*iter)->SESSION_ID)
			ecolor = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			ecolor = D3DCOLOR_ARGB(255, 255, 255, 255);

		(*iter)->CUR_TIME += GETTIME;

		if((*iter)->RETAIN_TIME < 0.f)
		{
			CFontMgr::GetInstance()->Setbatch_Font(tszmessage , 
				m_matworld._41 , m_matworld._42 - i * 18,  ecolor , true);
			++iter;
		}
		else
		{
			if( (*iter)->CUR_TIME > (*iter)->RETAIN_TIME)
			{				
				Safe_Delete((*iter));
				iter = m_message_list.erase( iter );
			}
			else
			{
				CFontMgr::GetInstance()->Setbatch_Font(tszmessage , 
					m_matworld._41 , m_matworld._42 - i * 18,  ecolor , true);
				++iter;
			}
		}

		++i;
	}
}

void CUI_Chat::Render(void)
{

}

bool CUI_Chat::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	return false;
}

void CUI_Chat::UI_Reaction(void)
{

}
void CUI_Chat::PushMessage(const S_PT_ROOM_RECEIVE_CHAT_M& _message)
{
	S_PT_ROOM_RECEIVE_CHAT_M* ptemp = new S_PT_ROOM_RECEIVE_CHAT_M;
	memcpy(ptemp , &_message , sizeof(S_PT_ROOM_RECEIVE_CHAT_M));
	m_message_list.push_front(ptemp);

	if(m_message_list.size() > (unsigned int)m_chat_size)
	{
		Safe_Delete(m_message_list.back());
		m_message_list.pop_back();
	}
}

void CUI_Chat::Release(void)
{
	MESSAGE_ITER iter = m_message_list.begin();
	MESSAGE_ITER iter_end = m_message_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(*iter);

	m_message_list.clear();
}

void CUI_Chat::SetChatSize(const int chatsize)
{
	m_chat_size = chatsize;
}

