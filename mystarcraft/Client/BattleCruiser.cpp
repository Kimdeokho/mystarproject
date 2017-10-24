#include "StdAfx.h"
#include "BattleCruiser.h"

#include "Area_Mgr.h"
#include "UnitMgr.h"
#include "MyMath.h"

CBattleCruiser::CBattleCruiser(void)
{
}

CBattleCruiser::~CBattleCruiser(void)
{
	Release();
}

void CBattleCruiser::Initialize(void)
{
	m_sortID = SORT_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_ecategory = UNIT;
	m_eteamnumber = TEAM_0;

	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_NOMAL;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.damage = 25;
	m_unitinfo.hp = 500;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 60;
	m_unitinfo.attack_range = 192;
	m_unitinfo.search_range = 256;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 37.5f;
	m_vertex.right = 37.5f;
	m_vertex.top =  28;
	m_vertex.bottom = 28;


}

void CBattleCruiser::Update(void)
{

}

void CBattleCruiser::Render(void)
{

}

void CBattleCruiser::Inputkey_reaction(const int& nkey)
{

}

void CBattleCruiser::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CBattleCruiser::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}
