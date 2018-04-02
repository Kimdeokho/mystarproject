#include "StdAfx.h"
#include "Nuclear_aim.h"

#include "Obj.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Device.h"

#include "Nuclear_Bullet.h"
#include "ObjMgr.h"
CNuclear_aim::CNuclear_aim(CObj* pobj)
{
	m_pobj = pobj;
}

CNuclear_aim::~CNuclear_aim(void)
{
}
void CNuclear_aim::Initialize(void)
{
	m_sortID = SORT_GROUND_EFF;
	m_aim_time = 0.f;
	m_bactive = false;

	m_aimtex = CTextureMgr::GetInstance()->GetGeneralTexture(L"NUCLEAR_AIM");
	m_vcenter = D3DXVECTOR3( float(((*m_aimtex)[0])->ImgInfo.Width/2) , float(((*m_aimtex)[0])->ImgInfo.Height/2) , 0);

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_aimtex->size();
	m_frame.fframespeed = float(m_frame.umax + 1);

	m_matWorld._11 = 2.3f;
	m_matWorld._22 = 2.3f;

	m_caster_id = m_pobj->GetObjNumber();
}

void CNuclear_aim::Update(void)
{
	if(NULL == CObjMgr::GetInstance()->obj_alivecheck(m_caster_id))
	{
		m_caster_id = 0;
		m_bdestroy = true;
		return;
	}

	m_frame.fcurframe += GETTIME*m_frame.fframespeed * 5.3f;

	if(m_frame.fcurframe >= m_frame.umax)
		m_frame.fcurframe = 0.f;

	m_aim_time += GETTIME;

	m_curtex = (*m_aimtex)[ int(m_frame.fcurframe) ];

	if(m_aim_time >=5.f)
	{
		m_bactive = false;
		m_pobj->SetState(IDLE);

		m_bdestroy = true;

		CObj* pbullet;
		pbullet = new CNuclear_Bullet(m_vPos);
		pbullet->Initialize();
		CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
		//น฿ป็
	}

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CNuclear_aim::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(m_curtex->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
}
