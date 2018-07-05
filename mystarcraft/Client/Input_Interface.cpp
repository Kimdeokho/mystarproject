#include "StdAfx.h"
#include "Input_Interface.h"

#include "KeyMgr.h"

CInput_Interface::CInput_Interface(void)
{
	m_inst = CKeyMgr::GetInstance();
}

CInput_Interface::~CInput_Interface(void)
{
}
