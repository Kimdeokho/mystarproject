#include "StdAfx.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "MouseMgr.h"
#include "ScrollMgr.h"
#include "Hatchery.h"
#include "LineMgr.h"
#include "ObjPoolMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)
CKeyMgr::CKeyMgr(void)
{
	memset(m_KeyPress , 0 , sizeof(bool)*MAX_KEY);
	memset(m_DbClick_ready , 0 , sizeof(bool)*MAX_KEY);

	m_bCombine = false;
	m_bDbClick_ready = false;
	m_bKeyUp = false;

	m_dbClick_Timer = 0.f;
	m_clickCnt = 0;

	memset(&m_downpt , 0 , sizeof(POINT));
	memset(&m_curpt , 0 , sizeof(POINT));
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

		pObj = CObjPoolMgr::GetInstance()->construct(ZU_DRONE);
		pObj->SetObj_ID(CObjPoolMgr::m_Obj_Cnt);
		pObj->SetPos(CMouseMgr::GetMousePt().x + CScrollMgr::m_fScrollX ,  CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY);
		pObj->Initialize();
		CObjMgr::GetInstance()->AddObject(pObj , ZU_DRONE);
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
			//CObjMgr::GetInstance()->DestroyObj(ZB_HACHERY);
			//CObjPoolMgr::GetInstance()->destroy(pObj , ZU_DRONE);
			pObj->SetDestroy(true);
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

		//조합키 메세지에 대한 반응
	}
	else
		m_bCombine = false;
}
void CKeyMgr::MouseKeyDown(const int& nkey)
{
	if(GetAsyncKeyState(nkey) & 0x8000)
	{
		if(false == m_KeyPress[nkey])
		{
			m_downpt.x = float(CMouseMgr::GetMousePt().x);
			m_downpt.y = float(CMouseMgr::GetMousePt().y);

			m_KeyPress[nkey] = true;
			m_DbClick_ready[nkey] = true;
			m_bKeyUp = true;
			++m_clickCnt;

			pObj = CObjPoolMgr::GetInstance()->construct(ZU_DRONE);
			pObj->SetObj_ID(CObjPoolMgr::m_Obj_Cnt);
			pObj->SetPos(CMouseMgr::GetMousePt().x + CScrollMgr::m_fScrollX ,  CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY);
			pObj->Initialize();
			CObjMgr::GetInstance()->AddObject(pObj , ZU_DRONE);
			//CObj* pObj = new CHatchery();
			//pObj->SetPos(CMouseMgr::GetMousePt().x + CScrollMgr::m_fScrollX ,  CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY);
			//pObj->Initialize();
			//CObjMgr::GetInstance()->AddObject(pObj , ZB_HACHERY);


			
		}
		else
		{
			m_curpt.x = float(CMouseMgr::GetMousePt().x);
			m_curpt.y = float(CMouseMgr::GetMousePt().y);

			CLineMgr::GetInstance()->SetRectPoint(m_downpt , m_curpt);
			CLineMgr::GetInstance()->SetRenderSwitch(true);
		}
	}
	else
	{
		m_KeyPress[nkey] = false;
	}
}
void CKeyMgr::MouseKeyUp(const int& nkey)
{
	if(true == m_bKeyUp && false == m_KeyPress[nkey])
	{
		m_bKeyUp = false;
		CLineMgr::GetInstance()->SetRenderSwitch(false);
		//m_curpt = CMouseMgr::GetMousePt();
	}
}
void CKeyMgr::DbClick(const int& nkey)
{
	if(true == m_DbClick_ready[nkey]) // m_clickcnt == 1로도 대응가능
	{
		m_dbClick_Timer += GETTIME;

		if(m_dbClick_Timer > 0.5f)
		{
			m_DbClick_ready[nkey] = false;
			m_dbClick_Timer = 0.f;

			m_clickCnt = 0;
		}
		else
		{
			if(GetAsyncKeyState(nkey) & 0x8000)
			{
				if(2 == m_clickCnt)
				{
					//더블클릭 완료
					m_dbClick_Timer = 0.f;
					m_DbClick_ready[nkey] = false;

					m_clickCnt = 0;
				}
			}
		}
	}
}
void CKeyMgr::Update(void)
{
	//clock_t begin , end;

	//begin = clock();

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

	OnceKeyDown('A');

	MouseKeyDown(VK_LBUTTON);
	MouseKeyUp(VK_LBUTTON);

	DbClick(VK_LBUTTON);

	//end = clock();

	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , end - begin , 200, 200);
}
