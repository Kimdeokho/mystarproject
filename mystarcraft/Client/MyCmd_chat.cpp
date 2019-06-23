#include "StdAfx.h"
#include "MyCmd_chat.h"

#include "Ingame_UIMgr.h"
#include "Session_Mgr.h"
#include "UI_Chat.h"
CMyCmd_chat::CMyCmd_chat(void)
{
	m_session_id = 0;
	memset(m_message , 0 , sizeof(m_message));
	memset(m_user_id , 0 , sizeof(m_user_id));
}

CMyCmd_chat::~CMyCmd_chat(void)
{
}

void CMyCmd_chat::Progress(void)
{
	S_PT_ROOM_RECEIVE_CHAT_M temp;

	wcscpy_s(temp.MESSAGE , m_message);
	wcscpy_s(temp.USER_ID , m_user_id);
	temp.RETAIN_TIME = 7.f;
	temp.CUR_TIME = 0.f;
	temp.SESSION_ID = m_session_id;

	CIngame_UIMgr::GetInstance()->GetUI_Chat()->PushMessage(temp);
}

void CMyCmd_chat::Write(CStreamSP& writestream)
{
	CMyCommand::Write(writestream);

	writestream->WriteDWORD(m_session_id);
	writestream->WriteWCHARs(m_user_id , 32);
	writestream->WriteWCHARs(m_message , 128);
}

void CMyCmd_chat::Read(CStreamSP& readstream)
{
	readstream->ReadDWORD(&m_session_id);
	readstream->ReadWCHARs(m_user_id , 32);
	readstream->ReadWCHARs(m_message , 128);
}

CMyCmd_chat* CMyCmd_chat::StaticCreate(const S_PT_ROOM_RECEIVE_CHAT_M& _msg)
{
	CMyCmd_chat* pcmd = new CMyCmd_chat;

	pcmd->m_ecmdtype = CMDTYPE_CHAT;
	pcmd->m_eteamnum = CSession_Mgr::GetInstance()->GetTeamNumber();

	pcmd->m_session_id	= _msg.SESSION_ID;
	lstrcpy(pcmd->m_user_id , _msg.USER_ID);
	lstrcpy(pcmd->m_message	, _msg.MESSAGE);

	return pcmd;
}
