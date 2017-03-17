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
//Ű ������ �ֱ�(����Ű�Է�) ,������ �־ �ѹ��� �Է¹ޱ�, ����Ű�Է�
void CKeyMgr::TurboKeyDown(const int& nkey)
{
	if(true == m_bCombine)
		return;

	if(GetAsyncKeyState(nkey) & 0x8000)
	{
		m_KeyPress[nkey] = true;
		//���⿡ nkey�� ���缱�õ� ������Ʈ�� ������ �޼��� �����ϰ��Ѵ�.
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
			//���⿡ nkey�� ���缱�õ� ������Ʈ�� ������ �޼��� �����ϰ��Ѵ�.
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

