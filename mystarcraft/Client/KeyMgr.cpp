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
#include "Marine.h"
#include "Tank.h"
#include "SCV.h"
#include "Firebat.h"
#include "Medic.h"
#include "Goliath.h"
#include "BattleCruiser.h"
#include "Wraith.h"
#include "Dropship.h"
#include "Vulture.h"

#include "Comandcenter.h"
#include "T_gas.h"

#include "Area_Mgr.h"

#include "ComanderMgr.h"
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
	m_bwork = false;
	m_bdraging = false;

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
			
			//첫번째 키다운한 위치 저장
			m_downpt = CMouseMgr::GetInstance()->GetScreenMousePt();
		}
		else
		{
			//지속적으로 엔드포인트 갱신

			m_curpt = CMouseMgr::GetInstance()->GetScreenMousePt();

			///*드래그 이미지에 관한 함수들이다.*/
			//if(false == CComanderMgr::GetInstance()->intersect_minimap_mousept(m_downpt))
			{
				CLineMgr::GetInstance()->SetRectPoint(m_downpt , m_curpt);
				CLineMgr::GetInstance()->SetRenderSwitch(true);
			}
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
	{
		OnceKeyDown(i);
	}

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
	CObj* pObj = NULL;

	if(true == m_bOnceKeyDown_complete[VK_RBUTTON])
	{
		m_bOnceKeyDown_complete[VK_RBUTTON] = false;

		CFontMgr::GetInstance()->Set_KeyInput_Font(L"우 클릭" );

		if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
		{
			D3DXVECTOR2 vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();

			if(true == CComanderMgr::GetInstance()->intersect_minimap_mousept(vmousept))
			{
				CComanderMgr::GetInstance()->Minimappos_to_screen(vmousept);
			}
			else
			{
				//우클릭된 유닛을 구한다.
				vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				CArea_Mgr::GetInstance()->TargetChoice(vmousept);
			}			

			CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
			CTileManager::GetInstance()->Flowfield_Pathfinding(vmousept);


			CUnitMgr::GetInstance()->Intputkey_reaction(VK_RBUTTON);
			
		}
	}
	if(true == m_bOnceKeyDown_complete[VK_LBUTTON])
	{
		m_bOnceKeyDown_complete[VK_LBUTTON] = false;		

		D3DXVECTOR2 vmousept;
		if(m_clickwating['A'])
		{			
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				//CObj* ptarget = NULL;
				vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();

				if(true == CComanderMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					CComanderMgr::GetInstance()->Minimappos_to_screen(vmousept);
					//어택땅을 미니맵에 클릭시 미니맵 위치를 화면위치로 바꿔준다
				}
				else
				{
					//클릭된 유닛을 구한다.
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
					CArea_Mgr::GetInstance()->TargetChoice(vmousept);
				}			

				//부대유닛들의 중점을 구하고 선택된 타겟이 있으면 전달한다.
				CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
				CTileManager::GetInstance()->Flowfield_Pathfinding(vmousept);	

				CUnitMgr::GetInstance()->Intputkey_reaction('A' , VK_LBUTTON);

				CFontMgr::GetInstance()->Set_KeyInput_Font(L"A + 좌 클릭" );
			}
			
			m_bwork = true;
		}
		else if(m_clickwating['I'])
		{
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();

				if(false == CComanderMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();

					CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
					CTileManager::GetInstance()->Flowfield_Pathfinding(vmousept);	

					CUnitMgr::GetInstance()->Intputkey_reaction('I' , VK_LBUTTON);
				}
			}
		}
		else if(m_clickwating['U'])
		{
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();

				if(false == CComanderMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();

					CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
					CTileManager::GetInstance()->Flowfield_Pathfinding(vmousept);	

					CUnitMgr::GetInstance()->Intputkey_reaction('U' , VK_LBUTTON);
				}
			}
		}
		else if(m_clickwating['S'])
		{
			CUnitMgr::GetInstance()->Intputkey_reaction('S' , VK_LBUTTON);

			m_bwork = true;
		}
		//else if(m_clickwating['C'])
		//{
		//	m_clickwating['C'] = false;
		//	if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
		//	{
		//		CUnitMgr::GetInstance()->Intputkey_reaction('C' , VK_LBUTTON);
		//	}
		//}
		else
		{
			vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();
			//CArea_Mgr::GetInstance()->TargetChoice(vmousept);
			CComanderMgr::GetInstance()->Click_cmdbtn(vmousept);

			CUnitMgr::GetInstance()->Intputkey_reaction(VK_LBUTTON);
			CFontMgr::GetInstance()->Set_KeyInput_Font(L"좌 클릭" );

			m_bwork = false;
		}

		memset(m_clickwating , 0 , sizeof(m_clickwating));
	}



	if(true == m_bOnceKeyDown_complete['A'])
	{
		m_bOnceKeyDown_complete['A'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'A');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'A');

			m_clickwating['V'] = false;
		}
		else
		{
			//유닛이 선택된 상태일시 어택명령 대기한다 건물이면 안됨..
			if(CUnitMgr::GetInstance()->GetUnitlistempty())
				m_clickwating['A'] = false;
			else
				m_clickwating['A'] = true;//공격 마우스띄우기 아이콘

			CUnitMgr::GetInstance()->Intputkey_reaction('A');
		}

	}

	if(true == m_bOnceKeyDown_complete['B'])
	{
		m_bOnceKeyDown_complete['B'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'B');

			m_clickwating['B'] = false;
		}
		else
		{
			if(CUnitMgr::GetInstance()->GetUnitlistempty())
				m_clickwating['B'] = false;
			else
				m_clickwating['B'] = true;

			D3DXVECTOR2 vpos;
			for(int i = 0; i < 1; ++i)
			{
				vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				vpos.x += rand()%50 - 25;
				vpos.y += rand()%50 - 25;
				pObj = new CBattleCruiser;
				CObjMgr::GetInstance()->AddObject(pObj , OBJ_BATTLE);
				pObj->SetPos(vpos);
				pObj->Initialize();
				objcnt += 1;
			}

			CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

			CUnitMgr::GetInstance()->Intputkey_reaction('B');
		}
	}

	if(true == m_bOnceKeyDown_complete['C'])
	{
		m_bOnceKeyDown_complete['C'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'C');

			m_clickwating['B'] = false;
		}
		else
		{
			//if(CUnitMgr::GetInstance()->GetUnitlistempty())
			//	m_clickwating['C'] = false;
			//else
			//	m_clickwating['C'] = true;

			//D3DXVECTOR2 vpos;
			//for(int i = 0; i < 10; ++i)
			//{
			//	vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
			//	vpos.x += rand()%50 - 25;
			//	vpos.y += rand()%50 - 25;
			//	pObj = new CMedic;
			//	CObjMgr::GetInstance()->AddObject(pObj , OBJ_MEDIC);
			//	pObj->SetPos(vpos);
			//	pObj->Initialize();
			//	objcnt += 1;
			//}

			//CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

			CUnitMgr::GetInstance()->Intputkey_reaction('C');
		}
	}

	if(true == m_bOnceKeyDown_complete['D'])
	{
		m_bOnceKeyDown_complete['D'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'D');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'D');

			m_clickwating['V'] = false;
		}
		else
		{
			//if(CUnitMgr::GetInstance()->GetUnitlistempty())
			//	m_clickwating['D'] = false;
			//else
			//	m_clickwating['D'] = true;

			D3DXVECTOR2 vpos;
			for(int i = 0; i < 10; ++i)
			{
				vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				vpos.x += rand()%50 - 25;
				vpos.y += rand()%50 - 25;
				pObj = new CDropship;
				CObjMgr::GetInstance()->AddObject(pObj , OBJ_DROPSHIP);
				pObj->SetPos(vpos);
				pObj->Initialize();
				objcnt += 1;
			}

			CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

			CUnitMgr::GetInstance()->Intputkey_reaction('D');
		}
	}

	if(true == m_bOnceKeyDown_complete['E'])
	{
		m_bOnceKeyDown_complete['E'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'E');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'E');

			m_clickwating['V'] = false;
		}
		else
		{
			//if(CUnitMgr::GetInstance()->GetUnitlistempty())
			//	m_clickwating['C'] = false;
			//else
			//	m_clickwating['C'] = true;

			CUnitMgr::GetInstance()->Intputkey_reaction('E');
		}
	}

	if(true == m_bOnceKeyDown_complete['F'])
	{
		m_bOnceKeyDown_complete['F'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'F');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'F');

			m_clickwating['V'] = false;
		}
		else
		{
			//if(CUnitMgr::GetInstance()->GetUnitlistempty())
			//	m_clickwating['C'] = false;
			//else
			//	m_clickwating['C'] = true;

			D3DXVECTOR2 vpos;
			for(int i = 0; i < 10; ++i)
			{
				vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				vpos.x += rand()%50 - 25;
				vpos.y += rand()%50 - 25;
				pObj = new CFirebat;
				CObjMgr::GetInstance()->AddObject(pObj , OBJ_FIREBAT);
				pObj->SetPos(vpos);
				pObj->Initialize();
				objcnt += 1;
			}

			CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);
			CUnitMgr::GetInstance()->Intputkey_reaction('F');
		}
	}

	if(true == m_bOnceKeyDown_complete['G'])
	{
		m_bOnceKeyDown_complete['G'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'G');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'G');

			m_clickwating['V'] = false;
		}
		else
		{
			D3DXVECTOR2 vpos;
			for(int i = 0; i < 10; ++i)
			{
				vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				vpos.x += rand()%50 - 25;
				vpos.y += rand()%50 - 25;
				pObj = new CGoliath;
				CObjMgr::GetInstance()->AddObject(pObj , OBJ_FIREBAT);
				pObj->SetPos(vpos);
				pObj->Initialize();
				objcnt += 1;
			}

			CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

			CUnitMgr::GetInstance()->Intputkey_reaction('G');
		}
	}

	if(true == m_bOnceKeyDown_complete['I'])
	{
		m_bOnceKeyDown_complete['I'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'I');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'I');

			m_clickwating['V'] = false;
		}
		else
		{
			if(CUnitMgr::GetInstance()->GetUnitlistempty())
				m_clickwating['I'] = false;
			else
				m_clickwating['I'] = true;

			CUnitMgr::GetInstance()->Intputkey_reaction('I');
		}
	}


	if(true == m_bOnceKeyDown_complete['L'])
	{
		m_bOnceKeyDown_complete['L'] = false;

		//m_clickwating['L'] = true;

		CUnitMgr::GetInstance()->Intputkey_reaction('L');
	}

	if(true == m_bOnceKeyDown_complete['N'])
	{
		m_bOnceKeyDown_complete['N'] = false;

		//m_clickwating['N'] = true;

		CUnitMgr::GetInstance()->Intputkey_reaction('N');
	}

	if(true == m_bOnceKeyDown_complete['P'])
	{
		m_bOnceKeyDown_complete['P'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'P');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'P');

			m_clickwating['V'] = false;
		}
		else
		{
			CUnitMgr::GetInstance()->Intputkey_reaction('P');
		}

	}

	if(true == m_bOnceKeyDown_complete['Q'])
	{
		m_bOnceKeyDown_complete['Q'] = false;


		CUnitMgr::GetInstance()->Intputkey_reaction('Q');
	}
	if(true == m_bOnceKeyDown_complete['R'])
	{
		m_bOnceKeyDown_complete['R'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'R');

			m_clickwating['B'] = false;
		}
		else
		{
			CUnitMgr::GetInstance()->Intputkey_reaction('R');
		}
	}

	if(true == m_bOnceKeyDown_complete['S'])
	{
		m_bOnceKeyDown_complete['S'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'S');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'S');

			m_clickwating['V'] = false;
		}
		else
		{
			if(CUnitMgr::GetInstance()->GetUnitlistempty())
				m_clickwating['S'] = false;
			else
				m_clickwating['S'] = true;

			CUnitMgr::GetInstance()->Intputkey_reaction('S');
		}

		//D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		//for(int i = 0; i < 10; ++i)
		//{
		//	vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		//	vpos.x += rand()%50 - 25;
		//	vpos.y += rand()%50 - 25;
		//	pObj = new CSCV;
		//	CObjMgr::GetInstance()->AddObject(pObj , OBJ_SCV);
		//	pObj->SetPos(vpos);
		//	pObj->Initialize();
		//	objcnt += 1;
		//}

		CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);
	}

	if(true == m_bOnceKeyDown_complete['T'])
	{
		m_bOnceKeyDown_complete['T'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'T');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'T');

			m_clickwating['V'] = false;
		}
		else
		{
			CUnitMgr::GetInstance()->Intputkey_reaction('T');
		}


		D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		for(int i = 0; i < 10; ++i)
		{
			vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
			vpos.x += rand()%50 - 25;
			vpos.y += rand()%50 - 25;
			pObj = new CTank;
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_TANK);
			pObj->SetPos(vpos);
			pObj->Initialize();
			objcnt += 1;
		}
		CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

	}

	if(true == m_bOnceKeyDown_complete['U'])
	{
		m_bOnceKeyDown_complete['U'] = false;


		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'U');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'U');

			m_clickwating['V'] = false;
		}
		else
		{
			if(CUnitMgr::GetInstance()->GetUnitlistempty())
				m_clickwating['U'] = false;
			else
				m_clickwating['U'] = true;

			CUnitMgr::GetInstance()->Intputkey_reaction('U');
		}		
	}

	if(true == m_bOnceKeyDown_complete['W'])
	{
		m_bOnceKeyDown_complete['W'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'W');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'W');

			m_clickwating['V'] = false;
		}
		else
		{
			D3DXVECTOR2 vpos;
			for(int i = 0; i < 1; ++i)
			{
				vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				vpos.x += rand()%50 - 25;
				vpos.y += rand()%50 - 25;
				pObj = new CWraith;
				CObjMgr::GetInstance()->AddObject(pObj , OBJ_WRAITH);
				pObj->SetPos(vpos);
				pObj->Initialize();
				objcnt += 1;
			}

			CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

			CUnitMgr::GetInstance()->Intputkey_reaction('W');
		}
	}



	if(true == m_bOnceKeyDown_complete['O'])
	{
		m_bOnceKeyDown_complete['O'] = false;

		CUnitMgr::GetInstance()->Intputkey_reaction('O');
	}

	if(true == m_bOnceKeyDown_complete['V'])
	{
		m_bOnceKeyDown_complete['V'] = false;

		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'V');

			m_clickwating['B'] = false;
		}
		else
		{
			if(CUnitMgr::GetInstance()->GetUnitlistempty())
				m_clickwating['V'] = false;
			else
				m_clickwating['V'] = true;

			CUnitMgr::GetInstance()->Intputkey_reaction('V');
		}

		D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		for(int i = 0; i < 10; ++i)
		{
			vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
			vpos.x += rand()%50 - 25;
			vpos.y += rand()%50 - 25;
			pObj = new CVulture;
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_VULTURE);
			pObj->SetPos(vpos);
			pObj->Initialize();
			objcnt += 1;
		}
	}

	if(true == m_bOnceKeyDown_complete['Z'])
	{
		m_bOnceKeyDown_complete['Z'] = false;
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"Z 입력" );

		//	/*곧 지울것들이다*/
		D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		for(int i = 0; i < 10; ++i)
		{
			vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
			vpos.x += rand()%50 - 25;
			vpos.y += rand()%50 - 25;
			pObj = new CMarine;
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_MARINE);
			pObj->SetPos(vpos);
			pObj->Initialize();
			objcnt += 1;
		}

		CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);
		CUnitMgr::GetInstance()->Intputkey_reaction('Z');


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
	if( true == m_bTurboKeyDown_complete[VK_LBUTTON])
	{
		if(false == m_bwork && true == CComanderMgr::GetInstance()->intersect_minimap_mousept(m_downpt))
			CComanderMgr::GetInstance()->SetMinimapCamPos(m_curpt/*CMouseMgr::GetInstance()->GetScreenMousePt()*/);
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

		//if(false == m_bwork)
		if( false == CComanderMgr::GetInstance()->intersect_minimap_mousept(m_downpt) )
		{
			CFontMgr::GetInstance()->Set_KeyInput_Font(L"좌 클릭 UP" );
			CLineMgr::GetInstance()->Select_unit();
		}
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

bool CKeyMgr::GetLbdraging(void)
{
	return m_TurboKeyPress[VK_LBUTTON];
}

