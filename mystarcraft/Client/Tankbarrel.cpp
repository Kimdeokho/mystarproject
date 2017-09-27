#include "StdAfx.h"
#include "Tankbarrel.h"

#include "Com_Targetsearch.h"
#include "Com_WTank.h"
#include "Com_TankbarrelAnim.h"
#include "Com_SiegebarrelAnim.h"
#include "Com_WSiege.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "Tank.h"
CTankbarrel::CTankbarrel(void)
{
}

CTankbarrel::CTankbarrel(CObj* tankbody)
{
	m_tankbody = tankbody;
}

CTankbarrel::~CTankbarrel(void)
{
}

void CTankbarrel::Initialize(void)
{
	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	m_eteamnumber = TEAM_0;

	m_bsiegemode = false;
	m_btransform_ready = false;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_ecategory = UNIT;
	
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_BOOM;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.damage = 30;
	m_unitinfo.hp = 150;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 224;
	m_unitinfo.search_range = 300;
	m_unitinfo.fog_range = 512;

	m_com_targetsearch = new CCom_Targetsearch(&m_unitinfo.attack_range , &m_unitinfo.search_range , SEARCH_ONLY_ENEMY);
	m_com_anim = new CCom_TankbarrelAnim(m_matWorld , m_curtex);
	m_com_weapon = new CCom_WTank(m_unitinfo.damage , DAMAGE_BOOM);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	((CCom_Targetsearch*)m_com_targetsearch)->Initialize(this , m_tankbody);
}

void CTankbarrel::Update(void)
{

	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos , 64);
	m_curidx256 = CMyMath::Pos_to_index(m_vPos , 256);
	m_curidx512 = CMyMath::Pos_to_index(m_vPos , 512);

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
		m_tankbody->SetState(ATTACK);
	}
	else if(MOVE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(TRANSFORMING == m_unitinfo.estate)
	{
		if(true == m_btransform_ready)
		{
			if(true == ((CCom_Animation*)m_com_anim)->GetAnimation_end())
			{
				m_unitinfo.estate = IDLE;
				m_btransform_ready = false;

				if(false == m_bsiegemode)
				{				
					m_bsiegemode = true;										
					m_btransform_ready = false;
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
	m_pSprite->SetTransform(&m_matWorld);
	if(TEAM_1 == m_eteamnumber)
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,0,0));
	}
	else
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}

}

void CTankbarrel::Inputkey_reaction(const int& nkey)
{
	if( 'O' == nkey)
	{
		if(TRANSFORMING != m_unitinfo.estate)
		{
			m_unitinfo.estate = TRANSFORMING;
			m_btransform_ready = false;
			if(false == m_bsiegemode)
			{
				m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);
			}
			else
			{
				m_vcurdir = CMyMath::dgree_to_dir(10*22.5f);
			}
		}
	}
}

void CTankbarrel::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

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
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_ecategory = UNIT;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_BOOM;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.damage = 30;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 224;
	m_unitinfo.search_range = 300;
	m_unitinfo.fog_range = 512;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(iter->second);

	m_componentlist.clear();

	m_com_targetsearch = new CCom_Targetsearch(&m_unitinfo.attack_range , &m_unitinfo.search_range , SEARCH_ONLY_ENEMY);
	m_com_anim = new CCom_TankbarrelAnim(m_matWorld , m_curtex);
	m_com_weapon = new CCom_WTank(m_unitinfo.damage , DAMAGE_BOOM);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;

	iter = m_componentlist.begin();
	iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	((CCom_Targetsearch*)m_com_targetsearch)->Initialize(this , m_tankbody);
}
void CTankbarrel::TransformSiegebarrel(void)
{
	m_unitinfo.damage = 70;
	m_unitinfo.hp = 150;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 384;
	m_unitinfo.search_range = 384;
	m_unitinfo.fog_range = 512;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(iter->second);

	m_componentlist.clear();


	m_com_targetsearch = new CCom_Targetsearch(&m_unitinfo.attack_range , &m_unitinfo.search_range , SEARCH_ONLY_ENEMY);
	m_com_anim = new CCom_SiegebarrelAnim(m_matWorld , m_curtex);
	m_com_weapon = new CCom_WSiege(m_unitinfo.damage , DAMAGE_BOOM);


	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;

	iter = m_componentlist.begin();
	iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	((CCom_Targetsearch*)m_com_targetsearch)->Initialize(this , m_tankbody);

	m_btransform_ready = false;

	//float fdgree = 10*22.5f;
	//m_vcurdir.x = cosf(CMyMath::dgree_to_radian(fdgree));
	//m_vcurdir.y = sinf(CMyMath::dgree_to_radian(fdgree));
}
