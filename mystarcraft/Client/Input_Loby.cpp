#include "StdAfx.h"
#include "Input_Loby.h"

#include "KeyMgr.h"
#include "MouseMgr.h"
#include "Loby_UIMgr.h"

CInput_Loby::CInput_Loby(void)
{
}

CInput_Loby::~CInput_Loby(void)
{
}

void CInput_Loby::Initialize(void)
{

}

void CInput_Loby::Update(void)
{
	if(CKeyMgr::GetInstance()->GetKeyUp_Check(VK_LBUTTON))
	{
		D3DXVECTOR2 vpt = CMouseMgr::GetInstance()->GetScreenMousePt();
		CLoby_UIMgr::GetInstance()->UI_Reaction(vpt);
	}
}

void CInput_Loby::Reelase(void)
{

}
