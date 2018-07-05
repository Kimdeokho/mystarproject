#include "StdAfx.h"
#include "UI_Chat.h"

#include "Session_Mgr.h"
#include "FontMgr.h"
CUI_Chat::CUI_Chat(void)
{
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

	MESSAGE_RITER riter = m_message_list.rbegin();
	MESSAGE_RITER riter_end = m_message_list.rend();

	TCHAR tszmessage[128] = L"";
	int i = 0;
	D3DCOLOR	ecolor;
	for( ; riter != riter_end; ++riter)
	{
		//S_PT_ROOM_RECEIVE_CHAT_M.MESSAGE;
		lstrcpy(tszmessage, L"");
		lstrcat(tszmessage , (*riter)->USER_ID);
		lstrcat(tszmessage , L" : ");
		lstrcat(tszmessage , (*riter)->MESSAGE);

		if(m_mysession_id == (*riter)->SESSION_ID)
			ecolor = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			ecolor = D3DCOLOR_ARGB(255, 255, 255, 255);

		CFontMgr::GetInstance()->Setbatch_Font(tszmessage , 
			m_matworld._41 , m_matworld._42 - i * 18
			, ecolor , true);
		++i;

		if( i > 4)
			break;
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
	m_message_list.push_back(ptemp);
}

void CUI_Chat::Release(void)
{
	MESSAGE_ITER iter = m_message_list.begin();
	MESSAGE_ITER iter_end = m_message_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(*iter);

	m_message_list.clear();
}

