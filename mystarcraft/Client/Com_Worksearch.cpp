#include "StdAfx.h"
#include "Com_Worksearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
#include "Mineral.h"
#include "GasBuilding.h"
#include "Workman.h"
#include "ComanderMgr.h"

#include "Comandcenter.h"
#include "T_gas.h"
#include "GasResource.h"
CCom_Worksearch::CCom_Worksearch(const int* attack_range , const int* search_range , TARGET_SEARCH_TYPE esearch_type)
{
	m_pattack_range = attack_range;
	m_psearch_range = search_range;
	m_search_type = esearch_type;
	m_core_center = NULL;
}

CCom_Worksearch::~CCom_Worksearch(void)
{
}

void CCom_Worksearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_pmineral = NULL;

	m_pobj = pobj;

	m_com_pathfind = (m_pobj->GetComponent(COM_PATHFINDE));
	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	//m_com_weapon->
	m_target_objid = -1;
	m_mineral_number = 0;
	m_core_number = 0;
}

void CCom_Worksearch::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);
	if(NULL == m_ptarget)
		m_target_objid = 0;

	ORDER eorder = m_pobj->GetUnitinfo().eorder;



	if(ORDER_GATHER == eorder)
	{
		if(NULL != m_ptarget)
		{
			m_myrc= m_pobj->GetMyRect();
			m_myrc.left -= 2;
			m_myrc.right += 2;
			m_myrc.top -= 2;
			m_myrc.bottom += 2;

			if(OBJ_MINERAL == m_ptarget->GetOBJNAME())
			{
				((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_ptarget->GetObjNumber());
				((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

				if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
				{
					//미네랄이면 미네랄을 이미 누가 점유하고있는가?
					/*점유 하고 있다면 ... 다른 미네랄을 찾는다
					만약 전부 누군가 점유하고있다면 원래 미네랄에서 대기한다*/

					const CObj* otherworkman = ((CMineral*)m_ptarget)->Getworkman();

					if( NULL != otherworkman &&
						m_pobj != otherworkman )
					{
						//다시 찾아~
						CArea_Mgr::GetInstance()->Search_Mineral(m_pobj->Getcuridx(64),  255 , m_pobj , m_ptarget);
						m_target_objid = m_ptarget->GetObjNumber();
					}
					else
					{
						if(false == ((CWorkman*)m_pobj)->ismineral_fragment())
						{
							((CWorkman*)m_pobj)->SetMineral_mark(m_ptarget);

							m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());
							if(NULL != m_com_weapon)
								((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);

							if(NULL != m_com_pathfind)
							{
								((CCom_Pathfind*)m_com_pathfind)->ClearPath();
								((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
							}
						}
					}
				}
			}
			else if( OBJ_GASBUILDING == m_ptarget->GetOBJNAME() )
			{
				((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_ptarget->GetObjNumber());
				((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

				if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
				{					
					if( BUILD != m_ptarget->GetUnitinfo().estate)
					{
						const CObj* otherworkman = ((CGasBuilding*)m_ptarget)->Getworkman();

						if(NULL == otherworkman )
						{
							// 진입
							((CGasBuilding*)m_ptarget)->Setworkman(m_pobj);
							((CWorkman*)m_pobj)->SetGas_mark(m_ptarget);

							m_pobj->SetState(BOARDING);//여기서 일꾼 area 위치도 삭제해준다
							m_pobj->area_release();

							((CCom_Pathfind*)m_com_pathfind)->ClearPath();
							((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
							((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
						}
					}
				}
			}			
		}
		else
		{
			m_pobj->SetOrder(ORDER_NONE);
			m_pobj->SetState(IDLE);
			((CCom_Pathfind*)m_com_pathfind)->ClearPath();
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
		}
	}
	else if(ORDER_RETURN_CARGO == eorder)
	{
		m_core_center = CObjMgr::GetInstance()->obj_alivecheck(m_core_number);
		//같은팀 커맨드 센터를 찾는다,,,
		if(NULL == m_core_center)
		{
			m_core_center = CObjMgr::GetInstance()->GetCoreBuilding(m_pobj);
		}

		if(NULL != m_core_center)
		{
			m_core_number = m_core_center->GetObjNumber();
			m_myrc= m_pobj->GetMyRect();
			m_myrc.left -= 3;
			m_myrc.right += 3;
			m_myrc.top -= 3;
			m_myrc.bottom += 3;

			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_core_center->GetObjNumber());
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

			if(MyIntersectrect(&m_outrc , &m_myrc , &(m_core_center->GetMyRect()) ) )
			{
				//미네랄 or 가스 입수
				((CWorkman*)m_pobj)->destroy_frag(); //여기서 자원이 추가된다
				m_pobj->SetOrder(ORDER_GATHER);
			}
		}
		else
		{
			m_pobj->SetOrder(ORDER_NONE);
			m_pobj->SetState(IDLE);
			((CCom_Pathfind*)m_com_pathfind)->ClearPath();
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
			m_core_number = 0;
		}
	}
	else if(ORDER_MOVE == eorder)
	{
		if(OBJ_COMMAND == m_ptarget->GetOBJNAME())
		{
			//미네랄을 들고 있으면,,
			m_pobj->SetOrder(ORDER_RETURN_CARGO);
		}
		if(NULL != m_ptarget)
		{
			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_ptarget->GetObjNumber());
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
		}
	}
	else if(ORDER_MOVE_BUILD == eorder) 
	{
		CObj* pObj = NULL;

		if( NULL != m_ptarget &&
			OBJ_GAS == m_ptarget->GetOBJNAME())
		{
			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_ptarget->GetObjNumber());
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

			m_myrc= m_pobj->GetMyRect();
			m_myrc.left -= 2;
			m_myrc.right += 2;
			m_myrc.top -= 2;
			m_myrc.bottom += 2;

			if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
			{
				
				((CGasResource*)m_ptarget)->SetState(BOARDING);
				((CGasResource*)m_ptarget)->building_area_release(); //가스자원을 지역에서 삭제시켜버리자				

				//추후	바꿈 테스트용 WORKMAN이 건물짓게 할꺼다..
				//if(CMyMath::Pos_to_index(m_pobj->GetPos() , 32) == 
				//	CMyMath::Pos_to_index(preview_info.vcenter_pos , 32))
				{
					((CWorkman*)m_pobj)->Create_Building( m_ptarget);
					//((CCom_Pathfind*)m_com_pathfind)->ClearPath();
					//((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
					//((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
				}

			}

		}
		else
		{
			//if(CMyMath::Pos_to_index(m_pobj->GetPos() , 32) == 
			//	CMyMath::Pos_to_index(preview_info.vcenter_pos , 32))
			{
				//추후	바꿈 테스트용 WORKMAN이 건물짓게 할꺼다..
				((CWorkman*)m_pobj)->Create_Building();

				//((CCom_Pathfind*)m_com_pathfind)->ClearPath();
				//((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
				//((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
			}
		}
	}
	else if(ORDER_BUILDING == eorder)
	{
		//건물 짓는 중
	}
}

void CCom_Worksearch::Render(void)
{

}

void CCom_Worksearch::Release(void)
{

}
