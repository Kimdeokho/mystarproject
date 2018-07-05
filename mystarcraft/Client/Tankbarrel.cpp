#include "StdAfx.h"
#include "Tankbarrel.h"

#include "Com_Distancesearch.h"
#include "Com_WTank.h"
#include "Com_TankbarrelAnim.h"
#include "Com_SiegebarrelAnim.h"
#include "Com_WSiege.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "Tank.h"
#include "Area_Mgr.h"

CTankbarrel::CTankbarrel(void)
{
}

CTankbarrel::CTankbarrel(CObj* tankbody)
{
	m_tankbody = tankbody;
}

CTankbarrel::~CTankbarrel(void)
{
	Release();
}

void CTankbarrel::Initialize(void)
{
	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	m_eteamnumber = TEAM_0;

	m_bsiegemode = false;
	m_btransform_ready = false;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = IDLE;
	m_ecategory = CATEGORY_UNIT;
	m_eOBJ_NAME = OBJ_TANK;
	
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 7*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 255;
	m_unitinfo.fog_range = 512;

	m_com_targetsearch = new CCom_Distancesearch(SEARCH_ONLY_ENEMY , m_tankbody);	
	m_com_weapon = new CCom_WTank();

	m_com_tankanim = new CCom_TankbarrelAnim(m_matWorld );
	m_com_siegeanim = new CCom_SiegebarrelAnim(m_matWorld );
	m_com_anim = m_com_tankanim;

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_vbarrelpos = D3DXVECTOR2(0,0);
}

void CTankbarrel::Update(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos , 64);

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
		m_tankbody->SetState(ATTACK);
	}
	else if(ATTACK == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
		m_tankbody->SetState(ATTACK);
	}
	else if(MOVE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(TRANSFORMING == m_unitinfo.state)
	{
		if(true == m_btransform_ready)
		{
			if(true == ((CCom_Animation*)m_com_anim)->GetAnimation_end())
			{
				m_unitinfo.state = IDLE;
				m_btransform_ready = false;

				if(false == m_bsiegemode)
				{				
					m_bsiegemode = true;

					//여기에 타겟서치랑 웨폰넣기

					Safe_Delete(m_com_weapon);
					m_com_weapon = new CCom_WSiege();
					m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
					m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;
					m_com_weapon->Initialize(this);
					m_com_targetsearch->Initialize(this);
				}
				else
				{
					m_bsiegemode = false;

					//몸통 역변 신호보냄..
					((CTank*)m_tankbody)->SetTransformReady(true);
					((CTank*)m_tankbody)->Transform();
				}				
			}			
		}
		else
		{
			if( true == ((CCom_Animation*)m_com_anim)->GetRotationComplete())
			{
				m_btransform_ready = true; // 변신 준비완료

				if(true == m_bsiegemode)
				{
					//시즈모드 상태였다면 변신 해제
					//m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);
					((CCom_Animation*)m_com_anim)->SetAnimation(L"SIEGEBARREL_TRANS");					
					//포신접기
				}
			}
		}
	}

}

void CTankbarrel::Render(void)
{
	if( false == m_unitinfo.is_active )
		return;

	m_matWorld._41 = m_vbarrelpos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vbarrelpos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();
}

void CTankbarrel::Inputkey_reaction(const int& nkey)
{
	if(TRANSFORMING == m_unitinfo.state )
		return;


	if( 'O' == nkey)
	{
		//if(TRANSFORMING != m_unitinfo.state)
		{
			m_unitinfo.state = TRANSFORMING;
			m_btransform_ready = false;
			if(false == m_bsiegemode)
			{
				//m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);
				m_vcurdir = CMyMath::dgree_to_dir(4*11.25f);
			}
			else
			{
				//m_vcurdir = CMyMath::dgree_to_dir(10*22.5f);
				m_vcurdir = CMyMath::dgree_to_dir(20*11.25f);
				//((CCom_Animation*)m_com_anim)->Dir_Initialize();
			}

			COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_TARGET_SEARCH);
			if(iter != m_componentlist.end())
				m_componentlist.erase(iter);
		}
	}

	if(VK_RBUTTON == nkey)
	{
		if(false == m_bsiegemode)
		{
			m_unitinfo.state = MOVE;
			m_unitinfo.order = ORDER_MOVE;
		}
		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);
	}
}

void CTankbarrel::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if(TRANSFORMING == m_unitinfo.state)
		return;


	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		if(TRANSFORMING != m_unitinfo.state)
		{
			((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());
		}
	}
}

bool CTankbarrel::GetTransformReady(void)
{
	return m_btransform_ready;
}

void CTankbarrel::SetTransformReady(bool btransform)
{
	m_btransform_ready = btransform;
}

void CTankbarrel::TransformTankbarrel(void)
{
	//역변신
	m_unitinfo.state = IDLE;
	m_ecategory = CATEGORY_UNIT;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 7*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 255;

	Safe_Delete(m_com_weapon);

	m_componentlist.clear();
		
	m_com_anim = m_com_tankanim;
	m_com_weapon = new CCom_WTank();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	//m_componentlist.clear();
	//m_com_anim = m_com_tankanim;
	//m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	//m_com_anim->Initialize(this);
}
void CTankbarrel::TransformSiegebarrel(void)
{
	//정변신
	m_unitinfo.state = TRANSFORMING;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 12*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 255;

	//COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	//COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	////Safe_Delete(m_com_targetsearch);
	//Safe_Delete(m_com_weapon);
	////for( ; iter != iter_end; ++iter)
	////	Safe_Delete(iter->second);

	//m_componentlist.clear();


	////m_com_targetsearch = new CCom_Distancesearch(SEARCH_ONLY_ENEMY , m_tankbody);
	//m_com_anim = m_com_siegeanim;
	//m_com_weapon = new CCom_WSiege();


	//m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	//m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	//m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;


	//iter = m_componentlist.begin();
	//iter_end = m_componentlist.end();

	//for( ; iter != iter_end; ++iter)
	//	iter->second->Initialize(this);

	m_componentlist.clear();
	m_com_anim = m_com_siegeanim;
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_com_anim->Initialize(this);

	m_btransform_ready = false;
}

void CTankbarrel::SetbarrelPos(const D3DXVECTOR2& vpos , const D3DXVECTOR2& vbarrelpos)
{
	m_vPos = vpos;
	m_vbarrelpos = vbarrelpos;
}

const D3DXVECTOR2& CTankbarrel::GetbarrelPos(void)
{
	return m_vbarrelpos;
}

void CTankbarrel::Release(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_ANIMATION);

	if(iter != m_componentlist.end())
		m_componentlist.erase(iter);		
	
	iter = m_componentlist.find(COM_TARGET_SEARCH);

	if(iter != m_componentlist.end())
		m_componentlist.erase(iter);		

	iter = m_componentlist.find(COM_WEAPON);

	if(iter != m_componentlist.end())
		m_componentlist.erase(iter);		

	Safe_Delete(m_com_tankanim);
	Safe_Delete(m_com_siegeanim);	
	m_com_anim = NULL;

	Safe_Delete(m_com_targetsearch);
	Safe_Delete(m_com_weapon);
}
