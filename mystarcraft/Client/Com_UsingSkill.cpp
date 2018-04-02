#include "StdAfx.h"
#include "Com_UsingSkill.h"
#include "Com_CC.h"

#include "ObjMgr.h"
#include "MyMath.h"
#include "Spidermine.h"
#include "BattleCruiser.h"
#include "Com_AirPathfind.h"

#include "Nuclear_aim.h"
#include "Skill_irradi.h"
#include "Skill_Defensive.h"
#include "Yamaeff.h"
CCom_UsingSkill::CCom_UsingSkill(void)
{
}

CCom_UsingSkill::~CCom_UsingSkill(void)
{
}

void CCom_UsingSkill::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;
	m_objpos = m_pobj->GetPosAdress();

	m_skill_order = SO_NONE;
	m_ptarget = NULL;
	m_targetid = 0;
}

void CCom_UsingSkill::Update(void)
{
	if(SO_NONE == m_skill_order)
		return;

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_targetid);

	if(NULL == m_ptarget)
		m_targetid = 0;
	else
		m_using_pos = m_ptarget->GetPos();

	if(SO_INSTALL_MINE == m_skill_order)
	{
		if(CMyMath::pos_distance(m_using_pos , m_pobj->GetPos()) <= 2)
		{
			CObj* pObj = NULL;
			D3DXVECTOR2 vpos;
			pObj = new CSpidermine;

			pObj->SetPos(m_using_pos);
			pObj->Initialize();
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_MINE);

			m_pobj->SetOrder(ORDER_NONE);
			m_pobj->SetState(IDLE);
			m_skill_order = SO_NONE;
		}
	}
	else if( SO_YAMATO == m_skill_order) 
	{
		if(NULL != m_ptarget)
		{
			if(CMyMath::pos_distance( m_using_pos, (*m_objpos)) <= 320*320)
			{
				D3DXVECTOR2 vdir = m_using_pos - (*m_objpos);
				D3DXVec2Normalize(&vdir , &vdir);
				m_pobj->Setdir(vdir);

				CSkill* pskill = new CYamaeff(m_pobj , m_ptarget);
				pskill->Initialize();
				//pskill->Use(m_ptarget->GetObjNumber() , m_ptarget->GetPos());
				CComponent*	pcom = m_pobj->GetComponent(COM_CC);
				((CCom_CC*)pcom)->AddBuff(pskill);

				m_pobj->SetOrder(ORDER_NONE);
				m_pobj->SetState(TRANSFORMING);

				pcom = m_pobj->GetComponent(COM_AIR_PATHFIND);
				if(NULL != pcom)
				{
					((CCom_AirPathfind*)pcom)->SetAir_moveupdate(false);
					((CCom_AirPathfind*)pcom)->SetTargetObjID(0);
				}
				m_skill_order = SO_NONE;
			}
		}
	}
	else if( SO_NUCLEAR == m_skill_order)
	{
		if(CMyMath::pos_distance(m_using_pos , (*m_objpos)) <= 352*352)
		{
			D3DXVECTOR2 vdir = m_using_pos - (*m_objpos);
			D3DXVec2Normalize(&vdir , &vdir);
			m_pobj->Setdir(vdir);

			CObj*	pnc_aim = new CNuclear_aim(m_pobj);			
			pnc_aim->Initialize();
			pnc_aim->SetPos(m_using_pos);
			CObjMgr::GetInstance()->AddObject(pnc_aim , OBJ_ETC);

			m_pobj->SetOrder(ORDER_NONE);
			m_pobj->SetState(TRANSFORMING);

			m_skill_order = SO_NONE;
		}
	}
	else if( SO_IRRADIATE == m_skill_order)
	{
		if(NULL != m_ptarget)
		{
			if(CMyMath::pos_distance(m_using_pos , (*m_objpos)) <= 9*32*9*32)
			{
				CSkill*	pskill = new CSkill_irradi(m_pobj , m_ptarget);			
				pskill->Initialize();
				
				CComponent* pcom = m_ptarget->GetComponent(COM_CC);

				((CCom_CC*)pcom)->AddDebuff(pskill);

				m_pobj->SetOrder(ORDER_NONE);
				m_pobj->SetState(TRANSFORMING);

				pcom = m_pobj->GetComponent(COM_AIR_PATHFIND);
				if(NULL != pcom)
				{
					((CCom_AirPathfind*)pcom)->SetAir_moveupdate(false);
					((CCom_AirPathfind*)pcom)->SetTargetObjID(0);
				}
				m_skill_order = SO_NONE;
			}
		}
	}
	else if( SO_DEFENSIVE == m_skill_order)
	{
		if(NULL != m_ptarget)
		{
			if(CMyMath::pos_distance(m_using_pos , (*m_objpos)) <= 320*320)
			{
				CSkill*	pskill = new CSkill_Defensive(m_pobj , m_ptarget);			
				pskill->Initialize();

				CComponent* pcom = m_ptarget->GetComponent(COM_CC);

				((CCom_CC*)pcom)->AddBuff(pskill);

				m_pobj->SetOrder(ORDER_NONE);
				m_pobj->SetState(TRANSFORMING);

				pcom = m_pobj->GetComponent(COM_AIR_PATHFIND);
				if(NULL != pcom)
				{
					((CCom_AirPathfind*)pcom)->SetAir_moveupdate(false);
					((CCom_AirPathfind*)pcom)->SetTargetObjID(0);
				}
				m_skill_order = SO_NONE;
			}
		}
	}
}

void CCom_UsingSkill::Render(void)
{

}

void CCom_UsingSkill::Release(void)
{

}

void CCom_UsingSkill::Setskill_order(SKILL_ORDER so)
{
	m_skill_order = so;
}

void CCom_UsingSkill::SetUsingSkill(SKILL_ORDER so, CObj* pobj , const D3DXVECTOR2& vpos)
{
	m_skill_order = so;

	m_ptarget = pobj;
	if(NULL != m_ptarget)
	{
		m_using_pos = m_ptarget->GetPos();
		m_targetid = m_ptarget->GetObjNumber();
	}
	else
	{
		m_targetid = 0;
		m_using_pos = vpos;
	}
}