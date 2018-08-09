#include "StdAfx.h"
#include "Input_Stage.h"

#include "MyCommandList.h"
#include "MyCmd_Move.h"
#include "MyCmd_Attack.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"
#include "MouseMgr.h"
#include "LineMgr.h"
#include "UnitMgr.h"
#include "TileManager.h"
#include "Area_Mgr.h"
CInput_Stage::CInput_Stage(void)
{
}

CInput_Stage::~CInput_Stage(void)
{
	Safe_Delete(m_cmdlist);
}

void CInput_Stage::Initialize(void)
{
	m_cmdlist = new CMyCommandList;

	memset(m_clickwating , 0 , sizeof(m_clickwating));
	m_bwork = false;
	m_select = false;
}

void CInput_Stage::Update(void)
{
	//뭔가 입력하면 m_cmdlist에서 m_cmdlist->PushCommand() 쏼라쏼라
	if(GetActiveWindow() != g_hWnd)
		return;

	Intput_oncekey_reaction();
	Intput_turbokey_reaction();	
	Intput_keyup_reaction();
	Intput_dbclick_reaction();
	Intput_combine_reaction();
}
void CInput_Stage::Intput_oncekey_reaction(void)
{
	if(true == m_inst->GetOnceKeyDown_Check(VK_RBUTTON))
	{
		//선택된 유닛이 있다면
		//CMyCommand* pcommand = new CMyCmd_Move;
		//pcommand 에 여러 정보를 넣은다음
		//m_cmdlist->PushCommand(무브명령 관련 정보담기)

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

			CMyCommand* pcommand = CMyCmd_Move::StaticCreate(vmousept);
			m_cmdlist->PushCommand(pcommand);

			memset(m_clickwating , 0 , sizeof(m_clickwating));
		}		
	}
	if(true == m_inst->GetOnceKeyDown_Check(VK_LBUTTON))
	{
		m_select = false;

		//첫번째 키다운한 위치 저장
		m_downpt = CMouseMgr::GetInstance()->GetScreenMousePt();

		D3DXVECTOR2 vmousept;
		vmousept = CMouseMgr::GetInstance()->GetScreenMousePt();

		bool escape = false;

		if(!CUnitMgr::GetInstance()->GetUnitlistempty())
		{
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
			for(char i = 'A'; i <= 'Z'; ++i)
			{
				if(m_clickwating[i])
				{
					//부대유닛들의 중점을 구하고 선택된 타겟이 있으면 전달한다.
					CUnitMgr::GetInstance()->Calculate_UnitCenterPt(vmousept);
					CTileManager::GetInstance()->Flowfield_Pathfinding(vmousept);

					if('A' == i )
					{
						//어택땅 명령 생성...
					}
					else
						CUnitMgr::GetInstance()->Input_cmd(i , VK_LBUTTON);					

					escape = true;
					m_bwork = true;
					break;
				}
			}
		}

		if(!escape)
		{
			CUnitMgr::GetInstance()->Input_cmd(VK_LBUTTON , m_clickwating);
			CIngame_UIMgr::GetInstance()->Click_cmdbtn(vmousept);
			m_bwork = false;
			m_select = true;
		}

		memset(m_clickwating , 0 , sizeof(m_clickwating));
	}


	for(char a = 'A'; a <= 'Z'; ++a)
	{
		if(true == m_inst->GetOnceKeyDown_Check(a))
		{
			if(!CUnitMgr::GetInstance()->GetUnitlistempty())
			{
				bool escape = false;
				for(char b = 'A'; b <= 'Z'; ++b)
				{
					if(m_clickwating[b])
					{
						CUnitMgr::GetInstance()->Input_cmd(b , a); //보류
						m_clickwating[a] = false;
						m_clickwating[b] = false;
						escape = true;
						break;
					}
				}
				if(!escape)
				{
					if(true == CUnitMgr::GetInstance()->Unit_Unification())
					{
						CUnitMgr::GetInstance()->Input_cmd(a , m_clickwating);

						if(false == m_clickwating[a])
						{
							/*
							WRITE

							선택된 유닛 수
							유닛 번호들,
							neky
							*/
						}
					}
					else
					{
						if('A' == a) //야매...
							m_clickwating[a] = true;
					}

					break;
				}
			}
		}
	}
}

void CInput_Stage::Intput_turbokey_reaction(void)
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

void CInput_Stage::Intput_keyup_reaction(void)
{
	if(true == m_inst->GetKeyUp_Check(VK_LBUTTON))
	{
		CLineMgr::GetInstance()->SetRenderSwitch(false);

		if( false == CIngame_UIMgr::GetInstance()->intersect_minimap_mousept(m_downpt) )
		{
			if(true == m_select)
				CLineMgr::GetInstance()->Select_unit();
		}
	}
}

void CInput_Stage::Intput_dbclick_reaction(void)
{

}

void CInput_Stage::Intput_combine_reaction(void)
{

}


void CInput_Stage::Reelase(void)
{
}

void CInput_Stage::ClearCmdList(void)
{
	m_cmdlist->ClearCommand();
}
