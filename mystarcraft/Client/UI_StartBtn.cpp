#include "StdAfx.h"
#include "UI_StartBtn.h"

#include "FontMgr.h"
#include "MouseMgr.h"
#include "Session_Mgr.h"
#include "RoomSession_Mgr.h"
#include "Room_UIMgr.h"
CUI_StartBtn::CUI_StartBtn(void)
{
}

CUI_StartBtn::~CUI_StartBtn(void)
{
	Release();
}

void CUI_StartBtn::Initialize(void)
{
}

void CUI_StartBtn::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + 150;
	m_rect.top = m_matworld._42;
	m_rect.bottom = m_matworld._42 + 22;

	D3DCOLOR ecolor = D3DCOLOR_ARGB(255,0,255,0);
	if(MyPtInrect(CMouseMgr::GetInstance()->GetScreenMousePt() , &m_rect))
		ecolor = D3DCOLOR_ARGB(255,0,255,0);
	else
		ecolor = D3DCOLOR_ARGB(255,85,185,23);

	CFontMgr::GetInstance()->Setbatch_Font(L"START!!" , m_matworld._41 + 75, m_matworld._42, 
		ecolor);
}

void CUI_StartBtn::Render(void)
{

}

bool CUI_StartBtn::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	if(MyPtInrect(vpos , &m_rect))
	{
		UI_Reaction();
		return true;
	}

	return false;
}

void CUI_StartBtn::UI_Reaction(void)
{
	/*
	WRITE_TCP_PACKET(PT_START_ARRANGEMENT , WriteBuffer,
		WRITE_PT_START_ARRANGEMENT(WriteBuffer , basecnt));*/

	int basecnt = 0;
	DWORD dwbyte = 0;
	HANDLE hFile = CreateFile(L"../Data/map/pyhon.dat" , 
		GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL);
	ReadFile(hFile , &basecnt, sizeof(int), &dwbyte, NULL); //юс╫ц,
	CloseHandle(hFile);

	int* baseslot = new int[basecnt];

	for(int i = 0; i < basecnt; ++i)
		baseslot[i] = i;

	srand(unsigned(time(NULL)));

	for(int i = 0; i < 10; ++i)
	{
		int sour = rand()%basecnt;
		int dest = rand()%basecnt;
		int temp = 0;

		temp		   = baseslot[sour];
		baseslot[sour] = baseslot[dest];
		baseslot[dest] = temp;
	}

	USER_INFO**	userlist = CRoomSession_Mgr::GetInstance()->GetUserinfolist();

	PLACE_DATA	temp[8];
	ZeroMemory(temp , sizeof(temp));

	for(int i = 0; i < basecnt; ++i)
	{
		if(NULL == userlist[i])
			continue;

		wcscpy_s(temp[i].TRIBE , userlist[i]->TRIBE);
		temp[i].SESSION_ID = userlist[i]->SESSION_ID;
		temp[i].TEAMNUM = userlist[i]->TEAMNUM;
		temp[i].START_SLOT = baseslot[i];		
	}

	delete[] baseslot;

	WRITEALL_UDP_PACKET(PU_START_ARRANGEMENT , WriteBuffer,
		WRITE_PU_START_ARRANGEMENT(WriteBuffer , temp));

	WRITEALL_UDP_PACKET(PU_INIT_GAME , WriteBuffer,
		WRITE_PU_INIT_GAME(WriteBuffer, rand()%1000 , CRoomSession_Mgr::GetInstance()->GetPlayerCnt()) );

	
}

void CUI_StartBtn::Release(void)
{

}