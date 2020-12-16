#include "StdAfx.h"
#include "Com_Worksearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "FontMgr.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
#include "Com_Transport.h"

#include "Mineral.h"
#include "GasBuilding.h"
#include "Workman.h"
#include "Ingame_UIMgr.h"

#include "Comandcenter.h"
#include "T_gas.h"
#include "GasResource.h"

#include "Dropship.h"

CCom_Worksearch::CCom_Worksearch()
{
}

CCom_Worksearch::~CCom_Worksearch(void)
{
}

void CCom_Worksearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_com_transport = NULL;
	m_pmineral = NULL;

	m_pobj = pobj;

	m_com_pathfind = (m_pobj->GetComponent(COM_PATHFINDE));
	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	//m_com_weapon->
	m_target_objid = 0;
	m_mineral_number = 0;
	m_core_number = 0;

	m_fsearch_time = 0.f;

	m_collRange = GETTIME * m_pobj->GetUnitinfo().fspeed * 4;
}

void CCom_Worksearch::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if(NULL == m_ptarget)
		m_target_objid = 0;

	ORDER order = m_pobj->GetUnitinfo().order;

	if(ORDER_GATHER == order)
	{
		if(NULL != m_ptarget)
		{
			m_myrc= m_pobj->GetMyRect();
			m_myrc.left -= m_collRange;
			m_myrc.right += m_collRange;
			m_myrc.top -= m_collRange;
			m_myrc.bottom += m_collRange;

			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_ptarget->GetObjNumber());
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

			if(OBJ_MINERAL == m_ptarget->GetOBJNAME())
			{
				if(MyIntersectrect( &m_myrc , &(m_ptarget->GetMyRect()) ) )
				{
					//미네랄이면 미네랄을 이미 누가 점유하고있는가?
					/*점유 하고 있다면 ... 다른 미네랄을 찾는다
					만약 전부 누군가 점유하고있다면 원래 미네랄에서 대기한다*/

					m_pobj->Setdir( m_ptarget->GetPos() - m_pobj->GetPos());

					if(NULL != m_com_pathfind)
					{
						((CCom_Pathfind*)m_com_pathfind)->ClearPath();
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
					}

					if( false ==  ((CMineral*)m_ptarget)->Getworkman(m_pobj))//지금 내가 캐고있지 않다
					{
						m_pobj->SetState(IDLE);
						if(false == ((CMineral*)m_ptarget)->Check_workman(m_pobj))//미네랄 대기자에도 없다
						{							
							if( ((CMineral*)m_ptarget)->Getworkman_count() < 2)
							{
								//대기자가 1명이거나 0명이면 대기자 목록에 추가한다
								((CMineral*)m_ptarget)->Setworkman(m_pobj);
								((CWorkman*)m_pobj)->SetMineral_mark(m_ptarget);
							}
							else
							{
								//대기자가 2명 이상이면 다른 미네랄 찾기
								CArea_Mgr::GetInstance()->Search_Mineral(m_pobj->Getcuridx(64),  255 , m_pobj , m_ptarget);
								m_target_objid = m_ptarget->GetObjNumber();	
							}
						}
					}
					else
					{
						if(false == ((CWorkman*)m_pobj)->ismineral_fragment())
						{							
							if(NULL != m_com_weapon)
								((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);
						}
						else
							m_pobj->SetOrder(ORDER_RETURN_CARGO);
					}
				}

			}
			else if( OBJ_T_GAS == m_ptarget->GetOBJNAME()  ||
				OBJ_Z_GAS == m_ptarget->GetOBJNAME())
			{

				if(MyIntersectrect(&m_myrc , &(m_ptarget->GetMyRect()) ) )
				{					
					if( BUILD != m_ptarget->GetUnitinfo().state)
					{
						const CObj* otherworkman = ((CGasBuilding*)m_ptarget)->Getworkman();

						if(NULL == otherworkman &&
							otherworkman != m_pobj)
						{
							// 진입
							((CGasBuilding*)m_ptarget)->Setworkman(m_pobj);
							((CWorkman*)m_pobj)->SetGas_mark(m_ptarget);

							m_pobj->SetActive(false);//여기서 일꾼 area 위치도 삭제해준다
							m_pobj->area_release();
						}
						else
							m_pobj->SetState(IDLE);
						((CCom_Pathfind*)m_com_pathfind)->ClearPath();
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
						((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
					}
				}
			}
			else if(OBJ_GAS == m_ptarget->GetOBJNAME())
			{
				if(MyIntersectrect( &m_myrc , &(m_ptarget->GetMyRect()) ) )
				{
					m_pobj->SetOrder(ORDER_NONE);
					m_pobj->SetState(IDLE);
					((CCom_Pathfind*)m_com_pathfind)->ClearPath();
					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
					((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
				}
			}
		}
		else
		{
			//CArea_Mgr::GetInstance()->Search_Mineral(m_pobj->Getcuridx(64),  255 , m_pobj , m_ptarget);
			//m_target_objid = m_ptarget->GetObjNumber();
			//m_pobj->SetOrder(ORDER_NONE);
			//m_pobj->SetState(IDLE);
			//((CCom_Pathfind*)m_com_pathfind)->ClearPath();
			//((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
			//((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
		}
	}
	else if(ORDER_RETURN_CARGO == order)
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
			m_myrc.left -= m_collRange;
			m_myrc.right += m_collRange;
			m_myrc.top -= m_collRange;
			m_myrc.bottom += m_collRange;

			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_core_center->GetObjNumber());
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

			if(MyIntersectrect(&m_myrc , &(m_core_center->GetMyRect()) ) )
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
	else if(ORDER_MOVE == order)
	{
		if(OBJ_COMMAND == m_ptarget->GetOBJNAME())
		{
			//미네랄을 들고 있으면,,
			m_pobj->SetOrder(ORDER_RETURN_CARGO);
		}
		else if(NULL != m_com_transport)
		{
			if(m_ptarget->GetTeamNumber() == m_pobj->GetTeamNumber())
			{
				if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < 32*32)
				{			
					//범위에 들어오면
					m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

					if(true == m_com_transport->setunit(m_pobj))
					{
						m_pobj->SetSelect(NONE_SELECT);
						m_pobj->area_release();
						m_pobj->SetActive(false);
						m_com_transport = NULL;
					}
					else
					{
						m_pobj->SetState(IDLE); 						
					}
					m_pobj->SetOrder(ORDER_NONE);

					m_target_objid = 0;
					m_ptarget = NULL;
					((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);
					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
				}
			}
		}

		if(NULL != m_ptarget)
		{
			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_ptarget->GetObjNumber());
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
		}
		else
		{
			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(0);
			//((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
		}
	}
	else if(ORDER_MOVE_BUILD == order) 
	{
		CObj* pObj = NULL;

		if( NULL != m_ptarget &&
			OBJ_GAS == m_ptarget->GetOBJNAME())
		{
			((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_ptarget->GetObjNumber());
			((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

			m_myrc= m_pobj->GetMyRect();
			m_myrc.left -= m_collRange;
			m_myrc.right += m_collRange;
			m_myrc.top -= m_collRange;
			m_myrc.bottom += m_collRange;

			if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
			{
				
				//(m_ptarget)->SetActive(false);
				m_ptarget->SetSelect(NONE_SELECT);
				((CGasResource*)m_ptarget)->building_area_release(); //가스자원을 지역에서 삭제시켜버리자				


				((CWorkman*)m_pobj)->Create_Building( m_ptarget);
			}

		}
		else
		{
			((CWorkman*)m_pobj)->Create_Building();
		}
	}
	else if(ORDER_BUILDING == order)
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
