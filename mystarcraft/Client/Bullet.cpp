#include "StdAfx.h"
#include "Bullet.h"

#include "ComanderMgr.h"
CBullet::CBullet(void)
{
	m_ptarget = NULL;

	m_upg_info = CComanderMgr::GetInstance()->GetUpginfo();
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
