#include "StdAfx.h"
#include "Input_Room.h"

#include "KeyMgr.h"
#include "MouseMgr.h"
#include "Room_UIMgr.h"
CInput_Room::CInput_Room(void)
{
}

CInput_Room::~CInput_Room(void)
{
}

void CInput_Room::Initialize(void)
{

}

void CInput_Room::Update(void)
{
	if(CKeyMgr::GetInstance()->GetKeyUp_Check(VK_LBUTTON))
	{
		D3DXVECTOR2 vpt = CMouseMgr::GetInstance()->GetScreenMousePt();
		CRoom_UIMgr::GetInstance()->UI_Reaction(vpt);
	}
	else if(CKeyMgr::GetInstance()->GetOnceKeyDown_Check(VK_RETURN))
	{
		CRoom_UIMgr::GetInstance()->SendChat();
	}
}

void CInput_Room::Reelase(void)
{

}
