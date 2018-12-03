#include "StdAfx.h"
#include "Bullet.h"

#include "Ingame_UIMgr.h"
#include "TimeMgr.h"
CBullet::CBullet(void)
{
	m_ptarget = NULL;

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();

	m_ftick_distance = 0.f;
	m_objcnt_num = 0;
}

CBullet::~CBullet(void)
{
}

void CBullet::Initialize(void)
{

}

void CBullet::Update(void)
{

}

void CBullet::Render(void)
{

}

void CBullet::Release(void)
{

}

void CBullet::Dead(void)
{

}
