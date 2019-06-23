#include "StdAfx.h"
#include "MyCmd_Leave.h"

#include "Session_Mgr.h"
#include "RoomSession_Mgr.h"
#include "Ingame_UIMgr.h"

#include "UI_Notice.h"

CMyCmd_Leave::CMyCmd_Leave(void)
{
	m_session_id = 0;
}

CMyCmd_Leave::~CMyCmd_Leave(void)
{
}

void CMyCmd_Leave::Progress(void)
{	
	CRoomSession_Mgr::GetInstance()->quit_play(m_session_id);

	SetFocus(NULL);
	if(CSession_Mgr::GetInstance()->GetSession_Info().SESSION_ID == m_session_id)
	{
		DestroyWindow(g_hWnd);
		//printf("%d 메세지를 받음\n" , CSession_Mgr::GetInstance()->GetSession_Info().SESSION_ID);
	}
	

	if( 1 == CRoomSession_Mgr::GetInstance()->GetPlayerCnt() )
	{
		CUI_Notice* pui = CIngame_UIMgr::GetInstance()->GetUI_Notice();
		pui->SetActive(true);
		pui->set_message(L"당신이 이겼습니다.");
		//pui->SetPos(D3DXVECTOR2(0,0));

	}
}

void CMyCmd_Leave::Write(CStreamSP& writestream)
{
	CMyCommand::Write(writestream);

	writestream->WriteDWORD(m_session_id);
}

void CMyCmd_Leave::Read(CStreamSP& readstream)
{
	readstream->ReadDWORD(&m_session_id);
}

CMyCmd_Leave* CMyCmd_Leave::StaticCreate(const DWORD session_id)
{
	CMyCmd_Leave* pcmd = new CMyCmd_Leave;

	pcmd->m_ecmdtype = CMDTYPE_LEAVE_PLAYER;
	pcmd->m_eteamnum = CSession_Mgr::GetInstance()->GetTeamNumber();
	pcmd->m_session_id = session_id;

	return pcmd;
}
