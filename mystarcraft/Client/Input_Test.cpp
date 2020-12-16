#include "StdAfx.h"
#include "Input_Test.h"

#include "KeyMgr.h"
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
#include "Ghost.h"
#include "Firebat.h"
#include "Medic.h"
#include "Goliath.h"
#include "BattleCruiser.h"
#include "Wraith.h"
#include "Dropship.h"
#include "Vulture.h"
#include "Vessle.h"
#include "FontMgr.h"

#include "Comandcenter.h"
#include "T_gas.h"

#include "Area_Mgr.h"

#include "Ingame_UIMgr.h"
CInput_Test::CInput_Test(void)
{
}

CInput_Test::~CInput_Test(void)
{
}

void CInput_Test::Initialize(void)
{
	//이걸 KeyMgr에 종속시켜서 SCene이 바뀔때 바꿔줘도 될듯

	memset(m_clickwating , 0 , sizeof(m_clickwating));
	memset(&m_downpt , 0 , sizeof(POINT));
	memset(&m_curpt , 0 , sizeof(POINT));

	m_bwork = false;
	m_select = false;

	
	objcnt = 0;
}

void CInput_Test::Update(void)
{
	if(GetActiveWindow() != g_hWnd)
		return;

	Intput_oncekey_reaction();
	Intput_turbokey_reaction();	
	Intput_keyup_reaction();
	Intput_dbclick_reaction();
	Intput_combine_reaction();
}
void CInput_Test::Intput_oncekey_reaction(void)
{
	CObj* pObj = NULL;

	if(true == m_inst->GetOnceKeyDown_Check(VK_RBUTTON))
	{
		if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
		{
			D3DXVECTOR2 vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();

			if(true == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(vmousept))
			{
				CIngame_UIMgr::GetInstance()->Minimappos_to_screen(vmousept);
				CArea_Mgr::GetInstance()->SetChoiceTarget(NULL);
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
	if(true == m_inst->GetOnceKeyDown_Check(VK_LBUTTON))
	{		
		m_select = false;

		//첫번째 키다운한 위치 저장
		m_downpt = CMouseMgr::GetInstance()->GetScreenMousePt();
		//m_bwork = true;

		D3DXVECTOR2 vmousept;
		vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();

		if(m_clickwating['A'])
		{			
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				//CObj* ptarget = NULL;			

				if(true == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					CIngame_UIMgr::GetInstance()->Minimappos_to_screen(vmousept);
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
			}

			m_bwork = true;
		}
		else if(m_clickwating['D'])
		{
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				if(false == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
					CArea_Mgr::GetInstance()->TargetChoice(vmousept);

					CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
					//CTileManager::GetInstance()->Flowfield_Pathfinding(vmousept);
					CUnitMgr::GetInstance()->Intputkey_reaction('D' , VK_LBUTTON);
				}
			}
		}
		else if(m_clickwating['I'])
		{
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				if(false == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
					CArea_Mgr::GetInstance()->TargetChoice(vmousept);

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
				if(false == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();

					CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);	

					CUnitMgr::GetInstance()->Intputkey_reaction('U' , VK_LBUTTON);
				}
			}
		}
		else if(m_clickwating['S'])
		{
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				CUnitMgr::GetInstance()->Intputkey_reaction('S' , VK_LBUTTON);
				m_bwork = true;
			}
		}
		else if(m_clickwating['Y'])
		{
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				if(false == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
					CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
					CArea_Mgr::GetInstance()->TargetChoice(vmousept);

					CUnitMgr::GetInstance()->Intputkey_reaction('Y' , VK_LBUTTON);
				}
			}
		}
		else if(m_clickwating['N'])
		{
			if(false == CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				if(false == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(vmousept))
				{
					vmousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
					CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
					CTileManager::GetInstance()->Flowfield_Pathfinding(vmousept);

					CUnitMgr::GetInstance()->Intputkey_reaction('N' , VK_LBUTTON);
				}
			}
		}
		else
		{
			//CArea_Mgr::GetInstance()->TargetChoice(vmousept);
			CIngame_UIMgr::GetInstance()->Click_cmdbtn(vmousept);

			CUnitMgr::GetInstance()->Intputkey_reaction(VK_LBUTTON);
			CFontMgr::GetInstance()->Set_KeyInput_Font(L"좌 클릭" );

			m_bwork = false;
			m_select =  true;
		}

		memset(m_clickwating , 0 , sizeof(m_clickwating));
	}



	if(true == m_inst->GetOnceKeyDown_Check('A'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('B'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('C'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('D'))
	{
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
			if(CUnitMgr::GetInstance()->GetUnitlistempty())
				m_clickwating['D'] = false;
			else
				m_clickwating['D'] = true;

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

	if(true == m_inst->GetOnceKeyDown_Check('E'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('F'))
	{
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

			//D3DXVECTOR2 vpos;
			//for(int i = 0; i < 10; ++i)
			//{
			//	vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
			//	vpos.x += rand()%50 - 25;
			//	vpos.y += rand()%50 - 25;
			//	pObj = new CFirebat;
			//	CObjMgr::GetInstance()->AddObject(pObj , OBJ_FIREBAT);
			//	pObj->SetPos(vpos);
			//	pObj->Initialize();
			//	objcnt += 1;
			//}

			//CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);
			CUnitMgr::GetInstance()->Intputkey_reaction('F');
		}
	}

	if(true == m_inst->GetOnceKeyDown_Check('G'))
	{
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
				CObjMgr::GetInstance()->AddObject(pObj , OBJ_GOLIATH);
				pObj->SetPos(vpos);
				pObj->Initialize();
				objcnt += 1;
			}

			CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

			CUnitMgr::GetInstance()->Intputkey_reaction('G');
		}
	}

	if(true == m_inst->GetOnceKeyDown_Check('H'))
	{		
		if(m_clickwating['B'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('B' , 'H');

			m_clickwating['B'] = false;
		}
		else if(m_clickwating['V'])
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
				CUnitMgr::GetInstance()->Intputkey_reaction('V' , 'H');

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
				pObj = new CGhost;
				CObjMgr::GetInstance()->AddObject(pObj , OBJ_GHOST);
				pObj->SetPos(vpos);
				pObj->Initialize();
				objcnt += 1;
			}

			CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);

			CUnitMgr::GetInstance()->Intputkey_reaction('H');
		}
	}

	if(true == m_inst->GetOnceKeyDown_Check('I'))
	{
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


	if(true == m_inst->GetOnceKeyDown_Check('L'))
	{
		//m_clickwating['L'] = true;

		CUnitMgr::GetInstance()->Intputkey_reaction('L');
	}

	if(true == m_inst->GetOnceKeyDown_Check('M'))
	{
		CUnitMgr::GetInstance()->Intputkey_reaction('M');
	}

	if(true == m_inst->GetOnceKeyDown_Check('N'))
	{
		//m_clickwating['N'] = true;

		if(CUnitMgr::GetInstance()->GetUnitlistempty())
			m_clickwating['N'] = false;
		else
			m_clickwating['N'] = true;

		CUnitMgr::GetInstance()->Intputkey_reaction('N');
	}

	if(true == m_inst->GetOnceKeyDown_Check('P'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('Q'))
	{
		CUnitMgr::GetInstance()->Intputkey_reaction('Q');
	}
	if(true == m_inst->GetOnceKeyDown_Check('R'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('S'))
	{
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

		D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		for(int i = 0; i < 10; ++i)
		{
			vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
			vpos.x += rand()%50 - 25;
			vpos.y += rand()%50 - 25;
			pObj = new CSCV(vpos);
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_SCV);
			pObj->Initialize();
			objcnt += 1;
		}

		CFontMgr::GetInstance()->Setnumber_combine_Font(L"obj_num%d" , objcnt , 400, 300);
	}

	if(true == m_inst->GetOnceKeyDown_Check('T'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('U'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('O'))
	{
		CUnitMgr::GetInstance()->Intputkey_reaction('O');
	}

	if(true == m_inst->GetOnceKeyDown_Check('P'))
	{
		CUnitMgr::GetInstance()->Intputkey_reaction('P');
	}

	if(true == m_inst->GetOnceKeyDown_Check('V'))
	{
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
			pObj = new CVessle;
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_VESSEL);
			pObj->SetPos(vpos);
			pObj->Initialize();
			objcnt += 1;
		}
	}

	if(true == m_inst->GetOnceKeyDown_Check('W'))
	{
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

	if(true == m_inst->GetOnceKeyDown_Check('Y'))
	{
		if(CUnitMgr::GetInstance()->GetUnitlistempty())
			m_clickwating['Y'] = false;
		else
			m_clickwating['Y'] = true;
	}

	if(true == m_inst->GetOnceKeyDown_Check('Z'))
	{
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
	if(true == m_inst->GetOnceKeyDown_Check(VK_ESCAPE))
	{
		memset(m_clickwating , 0 , sizeof(m_clickwating));

		CUnitMgr::GetInstance()->Intputkey_reaction(VK_ESCAPE);
	}
}
void CInput_Test::Intput_turbokey_reaction(void)
{
	float fspeed = 1500.f;

	if(true == m_inst->GetTurboKeyDown_Check('1'))
	{

	}
	if(true == m_inst->GetTurboKeyDown_Check('2'))
	{
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"터보 2 입력" );
	}
	if(true == m_inst->GetTurboKeyDown_Check('3'))
	{
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"터보 3 입력" );
	}
	if( true == m_inst->GetTurboKeyDown_Check(VK_LEFT))
	{
		CScrollMgr::m_fScrollX -= GETTIME*fspeed;
	}
	if( true == m_inst->GetTurboKeyDown_Check(VK_RIGHT))
	{
		CScrollMgr::m_fScrollX += GETTIME*fspeed;
	}
	if( true == m_inst->GetTurboKeyDown_Check(VK_UP))
	{
		CScrollMgr::m_fScrollY -= GETTIME*fspeed;
	}
	if( true == m_inst->GetTurboKeyDown_Check(VK_DOWN))
	{
		CScrollMgr::m_fScrollY += GETTIME*fspeed;
	}
	if( true == m_inst->GetTurboKeyDown_Check(VK_LBUTTON))
	{
		if(false == m_bwork && true == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(m_downpt))
			CIngame_UIMgr::GetInstance()->SetMinimapCamPos(CMouseMgr::GetInstance()->GetScreenMousePt());

		//지속적으로 엔드포인트 갱신
		m_curpt = CMouseMgr::GetInstance()->GetScreenMousePt();
		///*드래그 이미지에 관한 함수들이다.*/
		CLineMgr::GetInstance()->SetRectPoint(m_downpt , m_curpt);
		CLineMgr::GetInstance()->SetRenderSwitch(true);
	}

}
void CInput_Test::Intput_dbclick_reaction(void)
{
	if(true == m_inst->GetDbClick_Check(VK_LBUTTON))
	{
		CFontMgr::GetInstance()->Set_KeyInput_Font(L"L 더블클릭" );
	}
}
void CInput_Test::Intput_keyup_reaction(void)
{
	if(true == m_inst->GetKeyUp_Check(VK_LBUTTON))
	{
		CLineMgr::GetInstance()->SetRenderSwitch(false);

		if( false == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(m_downpt) )
		{
			CFontMgr::GetInstance()->Set_KeyInput_Font(L"좌 클릭 UP" );
			if(true == m_select)
				CLineMgr::GetInstance()->Select_unit();
		}
	}
}

void CInput_Test::Intput_combine_reaction(void)
{
	if(true == m_inst->GetCombineKey_Check(VK_CONTROL , '1'))
	{

	}
	if(true == m_inst->GetCombineKey_Check(VK_CONTROL , '2'))
	{

	}
	if(true == m_inst->GetCombineKey_Check(VK_CONTROL , '3'))
	{

	}
	if(true == m_inst->GetCombineKey_Check(VK_CONTROL , '4'))
	{

	}
	if(true == m_inst->GetCombineKey_Check(VK_CONTROL , '5'))
	{

	}
	//if(true == m_combinekey[VK_CONTROL] &&
	//	true == m_combinekey['1'] )
	//{
	//	CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 1" );
	//}

	//if(true == m_combinekey[VK_CONTROL] &&
	//	true == m_combinekey['2'] )
	//{
	//	CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 2" );
	//}

	//if(true == m_combinekey[VK_CONTROL] &&
	//	true == m_combinekey['3'] )
	//{
	//	CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 3" );
	//}

	//if(true == m_combinekey[VK_CONTROL] &&
	//	true == m_combinekey['4'] )
	//{
	//	CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 4" );
	//}

	//if(true == m_combinekey[VK_CONTROL] &&
	//	true == m_combinekey['5'] )
	//{
	//	CFontMgr::GetInstance()->Set_KeyInput_Font(L"control + 5" );
	//}
}
void CInput_Test::Reelase(void)
{

}
