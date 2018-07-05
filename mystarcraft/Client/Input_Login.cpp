#include "StdAfx.h"
#include "Input_Login.h"


#include "Login_UIMgr.h"
#include "MouseMgr.h"
#include "KeyMgr.h"
CInput_Login::CInput_Login(void)
{
}

CInput_Login::~CInput_Login(void)
{
}

void CInput_Login::Initialize(void)
{

}

void CInput_Login::Update(void)
{
	if(CKeyMgr::GetInstance()->GetKeyUp_Check(VK_LBUTTON))
	{
		D3DXVECTOR2 vpt = CMouseMgr::GetInstance()->GetScreenMousePt();
		CLogin_UIMgr::GetInstance()->UI_Reaction(vpt);
	}
}

void CInput_Login::Reelase(void)
{

}
