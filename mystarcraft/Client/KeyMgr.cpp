#include "StdAfx.h"
#include "KeyMgr.h"

#include "FontMgr.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"

#include "Input_Test.h"
#include "Input_Login.h"
#include "Input_Loby.h"
#include "Input_Room.h"
#include "Input_Stage.h"

IMPLEMENT_SINGLETON(CKeyMgr)
CKeyMgr::CKeyMgr(void)
{
	memset(m_TurboKeyPress , 0 , sizeof(bool)*MAX_KEY);
	memset(m_KeyPress , 0 , sizeof(bool)*MAX_KEY);
	memset(m_DbClick_ready , 0 , sizeof(bool)*MAX_KEY);
	memset(m_clickCnt , 0 , sizeof(m_clickCnt));
	memset(m_bKeyUp_ready , 0 , MAX_KEY);
	memset(m_dbClick_Timer , 0 , sizeof(m_dbClick_Timer));

	m_bCombine = false;
	m_bDbClick_ready = false;
	//m_bwork = false;
	//m_select = false;

	memset(m_bTurboKeyDown_complete , 0 , sizeof(m_bTurboKeyDown_complete));
	memset(m_bOnceKeyDown_complete , 0 , sizeof(m_bOnceKeyDown_complete));
	memset(m_bKeyUp_complete , 0 , sizeof(m_bKeyUp_complete));
	memset(m_dbclick_complete , 0 , sizeof(m_dbclick_complete));
	memset(m_combinekey , 0 , sizeof(m_combinekey));
	memset(m_bcombinefirst , 0 , sizeof(m_bcombinefirst));
	memset(m_bcombinesecond , 0 , sizeof(m_bcombinesecond));

	//m_bcombinefirst = false;
	//m_bcombinesecond = false;

	m_combineidx = 0;
	m_combine_ready[0] = -1;
	m_combine_ready[1] = -1;

	m_input = NULL;
}

CKeyMgr::~CKeyMgr(void)
{
	Safe_Delete(m_input);
}
//키 누르고 있기(연속키입력) ,누르고 있어도 한번만 입력받기, 동시키입력
void CKeyMgr::TurboKeyDown(const int& nkey)
{
	if(true == m_bCombine)
		return;

	if(GetAsyncKeyState(nkey) & 0x8000)
	{
		m_TurboKeyPress[nkey] = true;

		m_bTurboKeyDown_complete[nkey] = true;
	}
	else
	{
		m_TurboKeyPress[nkey] = false;
		m_bTurboKeyDown_complete[nkey] = false;
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
			m_bOnceKeyDown_complete[nkey] = true;
		}
		else
			m_bOnceKeyDown_complete[nkey] = false;
	}
	else
	{
		m_KeyPress[nkey] = false;
		m_bOnceKeyDown_complete[nkey] = false;
	}	
}
void CKeyMgr::CombineKey(const int& firstkey ,const int& secondkey)
{

	//if(GetAsyncKeyState(firstkey) & 0x8000)
	//{
	//	m_bCombine = true;
	//	if(false == m_bcombinefirst[firstkey])
	//	{
	//		m_bcombinefirst[firstkey] = true;
	//		m_combine_ready[m_combineidx] = firstkey;
	//		++m_combineidx;
	//	}
	//}
	//else
	//{
	//	m_combineidx = 0;
	//	m_bcombinefirst[firstkey] = false;
	//	m_bCombine = false;
	//}


	//if(GetAsyncKeyState(secondkey) & 0x8000)
	//{
	//	if(false == m_bcombinesecond[secondkey])
	//	{
	//		m_bcombinesecond[secondkey] = true;
	//		m_combine_ready[m_combineidx] = secondkey;
	//		++m_combineidx;
	//	}
	//}
	//else
	//	m_bcombinesecond[secondkey] = false;



	//if(m_combineidx >= 2)
	//{
	//	m_combineidx = 0;

	//	if(firstkey == m_combine_ready[0] &&
	//		secondkey == m_combine_ready[1] )
	//	{
	//		m_combinekey[firstkey] = true;
	//		m_combinekey[secondkey] = true;
	//	}

	//	m_bcombinefirst[firstkey] = false;
	//	m_bcombinesecond[secondkey] = false; //보류
	//}
	//else
	//{
	//	m_combinekey[firstkey] = false;
	//	m_combinekey[secondkey] = false;
	//}



	if(GetAsyncKeyState(firstkey) & 0x8000)
	{
		m_bCombine = true;
		if(false == m_bcombinefirst[firstkey])
		{
			m_bcombinefirst[firstkey] = true;
			m_combine_ready[m_combineidx] = firstkey;
			++m_combineidx;
		}
	}
	else
	{
		m_combineidx = 0;
		m_bcombinefirst[firstkey] = false;
		m_combinekey[firstkey] = false;
		
		m_bCombine = false;
	}


	if(GetAsyncKeyState(secondkey) & 0x8000)
	{
		if(false == m_bcombinesecond[secondkey])
		{
			m_bcombinesecond[secondkey] = true;
			m_combine_ready[m_combineidx] = secondkey;
			++m_combineidx;
		}
	}
	else
	{
		m_bcombinesecond[secondkey] = false;
		m_combinekey[secondkey] = false;
	}



	if(firstkey == m_combine_ready[0] &&
		secondkey == m_combine_ready[1] )
	{
		if(m_combineidx >= 2)
		{
			m_combineidx = 0;
			m_combinekey[firstkey] = true;
			m_combinekey[secondkey] = true;

			m_bcombinefirst[firstkey] = false;
			m_bcombinesecond[secondkey] = false;
		}
	}
}
void CKeyMgr::MouseOnceKeyDown(const int& nkey)
{
	if(GetAsyncKeyState(nkey) & 0x8000)
	{
		if(false == m_KeyPress[nkey])
		{
			m_KeyPress[nkey] = true;
			//m_DbClick_ready[nkey] = true;
			m_bKeyUp_ready[nkey] = true;

			++m_clickCnt[nkey];

			m_bOnceKeyDown_complete[nkey] = true;

			//첫번째 키다운한 위치 저장
			//m_downpt = CMouseMgr::GetInstance()->GetScreenMousePt();
		}
		else
		{
			m_bOnceKeyDown_complete[nkey] = false;
		}
	}
	else
	{
		m_bOnceKeyDown_complete[nkey] = false;
		m_KeyPress[nkey] = false;
	}

}
void CKeyMgr::MouseKeyUp(const int& nkey)
{
	if(true == m_bKeyUp_ready[nkey] && false == m_KeyPress[nkey])
	{
		m_bKeyUp_ready[nkey] = false;
		//CLineMgr::GetInstance()->SetRenderSwitch(false);

		if(true == m_DbClick_ready[nkey])
		{
			m_DbClick_ready[nkey] = false;
			m_dbclick_complete[nkey] = true;
		}
		else
		{
			m_bKeyUp_complete[nkey] = true;			
		}		
	}
	else
	{
		m_bKeyUp_complete[nkey] = false;
		m_dbclick_complete[nkey] = false;
	}
}
void CKeyMgr::DbClick(const int& nkey)
{
	//if(true == m_DbClick_ready[nkey]) // m_clickcnt == 1로도 대응가능

	if(1 <= m_clickCnt[nkey]) // m_clickcnt == 1로도 대응가능
	{
		m_dbClick_Timer[nkey] += GETTIME;

		if(m_dbClick_Timer[nkey] > 0.2f)
		{
			m_DbClick_ready[nkey] = false;
			m_dbClick_Timer[nkey] = 0.f;
			m_clickCnt[nkey] = 0;
		}
		else
		{
			if(GetAsyncKeyState(nkey) & 0x8000)
			{
				if(2 == m_clickCnt[nkey])
				{
					//더블클릭 완료
					m_dbClick_Timer[nkey] = 0.f;
					m_clickCnt[nkey] = 0;
					m_DbClick_ready[nkey] = true;			
				}
			}
		}
	}
}
void CKeyMgr::Update(void)
{
	if(GetActiveWindow() != g_hWnd)
		return;

	TurboKeyDown(VK_LEFT);
	TurboKeyDown(VK_RIGHT);
	TurboKeyDown(VK_UP);
	TurboKeyDown(VK_DOWN);
	TurboKeyDown(VK_LBUTTON);
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


	for(int i = 'A'; i <= 'Z'; ++i)
		OnceKeyDown(i);

	OnceKeyDown(VK_RETURN); //엔터
	OnceKeyDown(VK_ESCAPE);

	for(int i = VK_F1; i < VK_F12; ++i)
		OnceKeyDown(i);

	MouseOnceKeyDown(VK_LBUTTON);
	MouseKeyUp(VK_LBUTTON);
	MouseOnceKeyDown(VK_RBUTTON);
	MouseKeyUp(VK_RBUTTON);


	CombineKey(VK_CONTROL , VK_LBUTTON);

	for(int i = '1'; i <= '9'; ++i)
		CombineKey(VK_CONTROL , i);

	DbClick(VK_LBUTTON);

	m_input->Update();
}


//bool CKeyMgr::GetLbdraging(void)
//{
//	return m_TurboKeyPress[VK_LBUTTON];
//}
bool CKeyMgr::IS_CombineFirstKey(const int firstkey)
{
	return m_combinekey[firstkey];
}
bool CKeyMgr::IS_CombineSecondKey(const int secondkey)
{
	return m_combinekey[secondkey];
}
bool CKeyMgr::GetCombineKey_Check(const int& firstkey , const int& secondkey)
{
	return (m_combinekey[firstkey] && m_combinekey[secondkey]);
}

bool CKeyMgr::GetOnceKeyDown_Check(const int& nkey )
{
	return m_bOnceKeyDown_complete[nkey];
}
bool CKeyMgr::GetKeyUp_Check(const int& nkey)
{
	return m_bKeyUp_complete[nkey];
}
bool CKeyMgr::GetTurboKeyDown_Check(const int& nkey)
{
	return m_bTurboKeyDown_complete[nkey];
}
bool CKeyMgr::GetDbClick_Check(const int& nkey)
{
	return m_dbclick_complete[nkey];
}

void CKeyMgr::SetInput_Device(SCENEID escene)
{
	Safe_Delete(m_input);

	switch(escene)
	{
	case SCENE_LOGO:
		{			
			break;
		}
	case SCENE_LOGIN:
		{
			m_input = new CInput_Login;
			break;
		}
	case SCENE_LOBY:
		{
			m_input = new CInput_Loby;
			break;
		}
	case SCENE_ROOM:
		{
			m_input = new CInput_Room;
			break;
		}
	case SCENE_STAGE:
		{
			m_input = new CInput_Stage;
			break;
		}
	case SCENE_TEST:
		{
			m_input = new CInput_Test;
			break;
		}
	}
	m_input->Initialize();
}

