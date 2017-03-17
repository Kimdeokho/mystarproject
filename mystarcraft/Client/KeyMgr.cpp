#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)
CKeyMgr::CKeyMgr(void)
{
	memset(m_KeyPress , 0 , sizeof(bool)*MAX_KEY);
	m_bCombine = false;
}

CKeyMgr::~CKeyMgr(void)
{
}
//키 누르고 있기(연속키입력) ,누르고 있어도 한번만 입력받기, 동시키입력
void CKeyMgr::TurboKeyDown(const int& nkey)
{
	if(true == m_bCombine)
		return;

	if(GetAsyncKeyState(nkey) & 0x8000)
	{
		m_KeyPress[nkey] = true;
		//여기에 nkey를 현재선택된 오브젝트에 보내서 메세지 반응하게한다.
	}
	else
	{
		m_KeyPress[nkey] = false;
	}	
}

void CKeyMgr::OnceKeyDown(const int& nkey)
{
	if(true == m_bCombine)
		return;

	if(GetAsyncKeyState(nkey) & 0x8000)
	{
		if(false == m_KeyPress[nkey])
		{
			m_KeyPress[nkey] = true;
			//여기에 nkey를 현재선택된 오브젝트에 보내서 메세지 반응하게한다.
		}
	}
	else
	{
		m_KeyPress[nkey] = false;
	}	
}
void CKeyMgr::CombineKey(const int& firstkey ,const int& secondkey)
{
	if(GetAsyncKeyState(firstkey) & 0x8000 &&
		GetAsyncKeyState(secondkey) & 0x8000)
	{
		m_bCombine = true;
	}
	else
		m_bCombine = false;
}
void CKeyMgr::Update(void)
{
	TurboKeyDown(VK_LEFT);
	TurboKeyDown(VK_RIGHT);
	TurboKeyDown(VK_UP);
	TurboKeyDown(VK_DOWN);
	TurboKeyDown('1');
	TurboKeyDown('2');
	TurboKeyDown('3');
	TurboKeyDown('4');
	TurboKeyDown('5');
	TurboKeyDown('6');
	TurboKeyDown('7');
	TurboKeyDown('8');
	TurboKeyDown('9');
	TurboKeyDown('0');
}

