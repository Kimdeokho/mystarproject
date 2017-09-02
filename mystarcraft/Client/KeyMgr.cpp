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
#include "Drone.h"
#include "MyMath.h"
#include "TileManager.h"
#include "UnitMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)
CKeyMgr::CKeyMgr(void)
{
	memset(m_KeyPress , 0 , sizeof(bool)*MAX_KEY);
	memset(m_DbClick_ready , 0 , sizeof(bool)*MAX_KEY);
	memset(m_clickCnt , 0 , sizeof(m_clickCnt));
	memset(m_bKeyUp_ready , 0 , MAX_KEY);
	memset(m_dbClick_Timer , 0 , sizeof(m_dbClick_Timer));

	m_bCombine = false;
	m_bDbClick_ready = false;
	

	memset(&m_downpt , 0 , sizeof(POINT));
	memset(&m_curpt , 0 , sizeof(POINT));

	memset(m_bTurboKeyDown_complete , 0 , sizeof(m_bTurboKeyDown_complete));
	memset(m_bOnceKeyDown_complete , 0 , sizeof(m_bOnceKeyDown_complete));
	memset(m_bKeyUp_complete , 0 , sizeof(m_bKeyUp_complete));
	memset(m_dbclick_complete , 0 , sizeof(m_dbclick_complete));
	memset(m_combinekey , 0 , sizeof(m_combinekey));
	memset(m_bcombinefirst , 0 , sizeof(m_bcombinefirst));
	memset(m_bcombinesecond , 0 , sizeof(m_bcombinesecond));

	//m_bcombinefirst = false;
	//m_bcombinesecond = false;

	objcnt = 0;
	m_nkey = 0;

	m_combineidx = 0;
	m_combine_ready[0] = -1;
	m_combine_ready[1] = -1;


	memset(m_clickwating , 0 , sizeof(m_clickwating));
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

		m_bTurboKeyDown_complete[nkey] = true;
		//플레이어 입장에서 입력한것
		//Intputkey_reaction(nkey);
	}
	else
	{
		m_KeyPress[nkey] = false;
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
			//CObjMgr::GetInstance()->DestroyObj(ZB_HACHERY);
			//CObjPoolMgr::GetInstance()->destroy(pObj , ZU_DRONE);
			//pObj->SetDestroy(true);
			//여기에 nkey를 현재선택된 오브젝트에 보내서 메세지 반응하게한다.

			//Intputkey_reaction(nkey);
			//CObjMgr::GetInstance()->Intputkey_reaction(nkey);

			m_bOnceKeyDown_complete[nkey] = true;
		}
	}
	else
	{
		m_KeyPress[nkey] = false;
		m_bOnceKeyDown_complete[nkey] = false;
	}	
}
void CKeyMgr::CombineKey(const int& firstkey ,const int& secondkey)
{

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
		m_bcombinesecond[secondkey] = false;


	if(m_combineidx >= 2)
	{
		m_combineidx = 0;

		if(firstkey == m_combine_ready[0] &&
			secondkey == m_combine_ready[1] )
		{
			m_combinekey[firstkey] = true;
			m_combinekey[secondkey] = true;
		}
		else
		{
			m_combinekey[firstkey] = false;
			m_combinekey[secondkey] = false;
		}

		m_bcombinefirst[firstkey] = false;
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

			//Intputkey_reaction(nkey);/*지형 길을 먼저구해야하기때문에 위에 있어야한다*/
			//CObjMgr::GetInstance()->Intputkey_reaction(nkey);	

			m_downpt.x = float(CMouseMgr::GetMousePt().x);
			m_downpt.y = float(CMouseMgr::GetMousePt().y);
		}
		else
		{
			//이건 터보키에 넣고
			m_curpt.x = float(CMouseMgr::GetMousePt().x);
			m_curpt.y = float(CMouseMgr::GetMousePt().y);

			///*드래그 이미지에 관한 함수들이다.*/
			CLineMgr::GetInstance()->SetRectPoint(m_downpt , m_curpt);
			CLineMgr::GetInstance()->SetRenderSwitch(true);
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
		CLineMgr::GetInstance()->SetRenderSwitch(false);

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
		m_bKeyUp_complete[nkey] = false;
}
void CKeyMgr::DbClick(const int& nkey)
{
	//if(true == m_DbClick_ready[nkey]) // m_clickcnt == 1로도 대응가능

	if(1 <= m_clickCnt[nkey]) // m_clickcnt == 1로도 대응가능
	{
		m_dbClick_Timer[nkey] += GETTIME;

		if(m_dbClick_Timer[nkey] > 0.5f)
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

					//m_dbclick_complete[nkey] = true;
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
	OnceKeyDown('Z');
	OnceKeyDown('Q');
	OnceKeyDown('W');

	MouseOnceKeyDown(VK_LBUTTON);
	MouseKeyUp(VK_LBUTTON);
	MouseOnceKeyDown(VK_RBUTTON);
	MouseKeyUp(VK_RBUTTON);


	CombineKey(VK_CONTROL , '1');
	CombineKey(VK_CONTROL , '2');
	CombineKey(VK_CONTROL , '3');
	CombineKey(VK_CONTROL , '4');
	CombineKey(VK_CONTROL , '5');

	DbClick(VK_LBUTTON);

	//Intputkey_reaction(0);
	//end = clock();

	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , end - begin , 200, 200);

	
	Intput_oncekey_reaction();
	Intput_turbokey_reaction();	
	Intput_keyup_reaction();
	Intput_dbclick_reaction();
	Intput_combine_reaction();

}
void CKeyMgr::Intput_oncekey_reaction(void)
{
	if(true == m_bOnceKeyDown_complete[VK_RBUTTON])
	{
		m_bOnceKeyDown_complete[VK_RBUTTON] = false;

		CFontMgr::GetInstance()->Set_KeyInput_Font(L"우 클릭" );

		CTileManager::GetInstance()->Flowfield_Pathfinding();
		
		/*일반유닛 길찾기 컴포넌트 ,
		  공중유닛 길찾기 컴포넌트 ,
		  일꾼 길찾기 컴포넌트 이런식으로 구분해보자..*/

		CUnitMgr::GetInstance()->Intputkey_reaction(VK_RBUTTON);
	}
	if(true == m_bOnceKeyDown_complete[VK_LBUTTON])
	{
		m_bOnceKeyDown_complete[VK_LBUTTON] = false;		

		if(m_clickwating['A'])
		{
			CTileManager::GetInstance()->Flowfield_Pathfinding();
			CUnitMgr::GetInstance()->Intputkey_reaction('A' , VK_LBUTTON);
			m_clickwating['A'] = false;

			CFontMgr::GetInstance()->Set_KeyInput_Font(L"A + 좌 클릭" );
		}
		else
		{
			CUnitMgr::GetInstance()->Intputkey_reaction(VK_LBUTTON);
			CFontMgr::GetInstance()->Set_KeyInput_Font(L"좌 클릭" );
		}
	}
	if(true == m_bOnceKeyDown_complete['Z'])
	{
		m_bOnceKeyDown_complete['Z'] = false;
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"Z 입력" );

		//	/*곧 지울것들이다*/
		pObj = new CDrone;
		pObj->SetPos(CMouseMgr::GetMousePt().x + CScrollMgr::m_fScrollX ,  CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY);
		pObj->Initialize();

		int icase = rand()%23 + 15;

		float fvtx = (float)(icase)/2;
		//float fvtx = 8.5f;

		pObj->SetVertex(fvtx , fvtx, fvtx, fvtx);
		//pObj->SetVertex(8.f , 8.f , 8.f , 8.f);
		CObjMgr::GetInstance()->AddObject(pObj , ZU_DRONE);
		objcnt += 1;
		CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);
		CUnitMgr::GetInstance()->Intputkey_reaction('Z');


	}
	if(true == m_bOnceKeyDown_complete['A'])
	{
		m_bOnceKeyDown_complete['A'] = false;

		//유닛이 선택된 상태일시 어택명령 대기한다 건물이면 안됨..
		if(CUnitMgr::GetInstance()->GetUnitlistempty())
			m_clickwating['A'] = false;
		else
			m_clickwating['A'] = true;//공격 마우스띄우기 아이콘

		CUnitMgr::GetInstance()->Intputkey_reaction('A');
	}
	if(true == m_bOnceKeyDown_complete['Q'])
	{
		m_bOnceKeyDown_complete['Q'] = false;

		CUnitMgr::GetInstance()->Intputkey_reaction('Q');
	}
	if(true == m_bOnceKeyDown_complete['W'])
	{
		m_bOnceKeyDown_complete['W'] = false;

		CUnitMgr::GetInstance()->Intputkey_reaction('W');
	}
}
void CKeyMgr::Intput_turbokey_reaction(void)
{
	float fspeed = 1500.f;

	if(true == m_bTurboKeyDown_complete['1'])
	{

	}
	if(true == m_bTurboKeyDown_complete['2'])
	{
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"터보 2 입력" );
	}
	if(true == m_bTurboKeyDown_complete['3'])
	{
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"터보 3 입력" );
	}
	if( true == m_bTurboKeyDown_complete[VK_LEFT])
	{
		CScrollMgr::m_fScrollX -= GETTIME*fspeed;
	}
	if( true == m_bTurboKeyDown_complete[VK_RIGHT])
	{
		CScrollMgr::m_fScrollX += GETTIME*fspeed;
	}
	if( true == m_bTurboKeyDown_complete[VK_UP])
	{
		CScrollMgr::m_fScrollY -= GETTIME*fspeed;
	}
	if( true == m_bTurboKeyDown_complete[VK_DOWN])
	{
		CScrollMgr::m_fScrollY += GETTIME*fspeed;
	}


}
void CKeyMgr::Intput_dbclick_reaction(void)
{
	if(true == m_dbclick_complete[VK_LBUTTON])
	{
		m_dbclick_complete[VK_LBUTTON] = false;
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"L 더블클릭" );
	}
}
void CKeyMgr::Intput_keyup_reaction(void)
{
	if(true == m_bKeyUp_complete[VK_LBUTTON])
	{
		m_bKeyUp_complete[VK_LBUTTON] = false;
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"좌 클릭 UP" );

		CLineMgr::GetInstance()->Select_unit();
	}
}

void CKeyMgr::Intput_combine_reaction(void)
{
	if(true == m_combinekey[VK_CONTROL] &&
		true == m_combinekey['1'] )
	{
		m_combinekey[VK_CONTROL] = false;
		m_combinekey['1'] = false;

		CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 1" );
	}

	if(true == m_combinekey[VK_CONTROL] &&
		true == m_combinekey['2'] )
	{
		m_combinekey[VK_CONTROL] = false;
		m_combinekey['2'] = false;

		CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 2" );
	}

	if(true == m_combinekey[VK_CONTROL] &&
		true == m_combinekey['3'] )
	{
		m_combinekey[VK_CONTROL] = false;
		m_combinekey['3'] = false;

		CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 3" );
	}

	if(true == m_combinekey[VK_CONTROL] &&
		true == m_combinekey['4'] )
	{
		m_combinekey[VK_CONTROL] = false;
		m_combinekey['4'] = false;

		CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 4" );
	}

	if(true == m_combinekey[VK_CONTROL] &&
		true == m_combinekey['5'] )
	{
		m_combinekey[VK_CONTROL] = false;
		m_combinekey['5'] = false;

		CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 5" );
	}
}

