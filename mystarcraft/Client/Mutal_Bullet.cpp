#include "StdAfx.h"
#include "Mutal_Bullet.h"

#include "Com_BulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "ScrollMgr.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "Skill_DarkSwarm.h"

CMutal_Bullet::CMutal_Bullet(const int& target_id ,const D3DXVECTOR2& vdesetpos , CObj* const pobj)
:m_pself(pobj)
{
	m_target_id = target_id;
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_id);
	m_target_cntnum = m_ptarget->GetObjCountNumber();
	m_target_movetype = m_ptarget->GetUnitinfo().eMoveType;

	m_vdest_pos = vdesetpos;	
	m_eteam = m_pself->GetTeamNumber();
	m_attack_type = m_pself->GetUnitinfo().eAttackType;
}

CMutal_Bullet::~CMutal_Bullet(void)
{
	Release();
}
void CMutal_Bullet::Initialize(void)
{
	m_sortID = SORT_GROUND_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_BulletAnim(m_matWorld , L"MUTAL_BULLET")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_ftick_distance = GETTIME*450;

	m_collcnt = 1;

	m_vec_objid.reserve(4);
}

void CMutal_Bullet::Update(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_id);

	if(NULL != m_ptarget)
	{
		if(m_target_cntnum == m_ptarget->GetObjCountNumber())
		{			
			m_vdest_pos = m_ptarget->GetPos();
			m_old_targetpos = m_vdest_pos;
		}
		else
		{
			m_ptarget = NULL;
			m_target_id = 0;
			m_vdest_pos = m_old_targetpos;
		}
	}
	else
	{
		m_target_id = 0;
		m_vdest_pos = m_old_targetpos;
	}

	D3DXVECTOR2 vdir = m_vdest_pos - m_vPos;	
	D3DXVec2Normalize(&vdir , &vdir);
	m_vPos += vdir*m_ftick_distance;

	if(m_collcnt < 4)
	{
		if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < m_ftick_distance*m_ftick_distance)
		{
			Dead();

			if(NULL != m_ptarget)
			{
				int idx = m_ptarget->Getcuridx(32);

				CSoundDevice::GetInstance()->PlayBattleSound(SND_B_MUTAL_HIT , m_ptarget->GetPos() , -5000);
				if(MOVE_GROUND == m_ptarget->GetUnitinfo().eMoveType)
				{
					if(CSkill_DarkSwarm::m_darkswarm_cnt[idx] == 0)
						m_ptarget->SetDamage(9 / m_collcnt, DAMAGE_NOMAL);
				}
				else
					m_ptarget->SetDamage(9 / m_collcnt, DAMAGE_NOMAL);

				CObj* pnext_target = NULL;
				pnext_target = NextTarget_Search();

				if(NULL != pnext_target)
				{
					m_target_id = pnext_target->GetObjNumber();
					m_target_cntnum = pnext_target->GetObjCountNumber();
					m_target_movetype = pnext_target->GetUnitinfo().eMoveType;

				}
				else
					m_bdestroy = true;
			}
			else
				m_bdestroy = true;

			++m_collcnt;
		}
	}
	else
		m_bdestroy = true;
	

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CMutal_Bullet::Render(void)
{

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}
CObj* CMutal_Bullet::NextTarget_Search(void)
{
	m_vec_objid.push_back(m_ptarget->GetObjCountNumber());

	CObj* ptarget = NULL;
	list<CObj*>* obj_list = CArea_Mgr::GetInstance()->GetArea64();
	float idistance = 0;
	float fminvalue = 4096*4096;
	int isearch_range = 32*4;
	TARGET_SEARCH_TYPE	esearchtype = SEARCH_ONLY_ENEMY;

	MOVE_TYPE etarget_movetype = MOVE_NOT;
	D3DXVECTOR2	vtargetpos = D3DXVECTOR2(0,0);

	int idx[ASTAR_DIR_END];
	CMyMath::Calculator_eight_idx(idx , m_ptarget->GetPos() , 64);

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(idx[i] < 0)
			continue;

		list<CObj*>::iterator iter = obj_list[idx[i] ].begin();
		list<CObj*>::iterator iter_end = obj_list[ idx[i] ].end();
		

		for( ; iter != iter_end; ++iter)
		{
			if( CATEGORY_RESOURCE == (*iter)->GetCategory())
				continue;
			if( TEAM_NONE == (*iter)->GetTeamNumber())
				continue;
			if(m_eteam == (*iter)->GetTeamNumber())
				continue;
			//공중만 공격 가능한 유닛은 공중만 검사하고
			//지상만 공격 가능한 유닛은 지상만 검사한다

			
			if( SEARCH_ONLY_ENEMY == esearchtype)
			{
				etarget_movetype = (*iter)->GetUnitinfo().eMoveType;

				if( ATTACK_ONLY_AIR == m_attack_type)
				{
					if( MOVE_GROUND ==  etarget_movetype)
						continue;
				}
				else if( ATTACK_ONLY_GROUND == m_attack_type )
				{
					if( MOVE_AIR ==  etarget_movetype)
						continue;
				}
			}

			if((*iter)->GetUnitinfo().detect[m_eteam] < 1)
				continue;

			vtargetpos = (*iter)->GetPos();

			idistance = CMyMath::pos_distance(m_ptarget->GetPos() , vtargetpos);

			//지금 찾은 유닛과의 거리가 최소거리보다 작다면

			if(idistance <= isearch_range * isearch_range)
			{
				//해당 유닛의 탐지범위에 적합한지 본다.
				//목표를 찾았다.

				if( idistance < fminvalue )
				{
					int objcnt_num = 0;
					bool duplication = false;

					for(unsigned int j = 0; j < m_vec_objid.size(); ++j)
					{			
						objcnt_num = m_vec_objid[j];

						if( (*iter)->GetObjCountNumber() == objcnt_num)
						{
							duplication = true;
							break;
						}
					}
					if(!duplication)
					{
						fminvalue = idistance;						
						ptarget = (*iter);
					}
				}
			}
		}
	}

	return ptarget;
}

void CMutal_Bullet::Dead(void)
{	
	D3DXVECTOR2 vpos;
	
	if(NULL != m_ptarget)
		vpos = m_ptarget->GetPos();		
	else
		vpos = m_vdest_pos;

	SORT_ID esort = SORT_END;

	if(MOVE_GROUND == m_target_movetype)
		esort = SORT_GROUND_EFF;
	else if(MOVE_AIR == m_target_movetype)
		esort = SORT_AIR_EFF;


	CObj* peff = NULL;
	peff = new CGeneraEff(L"MUTAL_HIT" ,vpos  , D3DXVECTOR2(1.1f , 1.1f), esort );

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);
}

void CMutal_Bullet::Release(void)
{
	vector<int> temp;	
	temp.swap(m_vec_objid);	
}