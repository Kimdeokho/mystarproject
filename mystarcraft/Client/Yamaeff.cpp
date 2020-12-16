#include "StdAfx.h"
#include "Yamaeff.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"

#include "ObjMgr.h"
#include "Yama_bim.h"
#include "Device.h"
#include "SoundDevice.h"
CYamaeff::CYamaeff(CObj* pobj , CObj* ptarget)
{
	m_pobj = pobj;
	m_ptarget = ptarget;

	m_vtarget_pos = m_ptarget->GetPos();
	m_targetid = m_ptarget->GetObjNumber();
}

CYamaeff::~CYamaeff(void)
{
}

void CYamaeff::Initialize(void)
{
	m_skillname = SN_YAMATO;
	m_yamaready_tex = CTextureMgr::GetInstance()->GetGeneralTexture(L"YAMA_READY");
	m_yamafire_tex = CTextureMgr::GetInstance()->GetGeneralTexture(L"YAMA_FIRE");

	m_yamaready_center = D3DXVECTOR3( float(((*m_yamaready_tex)[0])->ImgInfo.Width/2) , float(((*m_yamaready_tex)[0])->ImgInfo.Height/2) , 0);

	m_loopcnt = 0;
	m_bready = true;

	m_bfire = false;

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_yamaready_tex->size();
	m_frame.fframespeed = float(m_frame.umax + 1);
}

void CYamaeff::Update(void)
{
	//if(!m_start)
	//	return;

	m_pobj->SetState(TRANSFORMING);
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_targetid);

	if(NULL == m_ptarget)
		m_targetid = 0;

	m_frame.fcurframe += GETTIME*m_frame.fframespeed * 1.3f;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;

		if(true == m_bfire)
		{
			m_pobj->SetState(IDLE);
			m_bdestroy = true;
		}

		if(true == m_bready)
		{
			++m_loopcnt;
			if(m_loopcnt >= 2)
			{
				m_frame.fcurframe = 0.f;
				m_frame.umax = m_yamafire_tex->size();
				m_frame.fframespeed = float(m_frame.umax + 1);
				m_bready = false;
				m_bfire = true;

				//m_targetid != 0 이면 야마토빔 생성
				if(NULL != m_ptarget)
				{
					CSoundDevice::GetInstance()->PlayBattleSound(SND_B_BATTLE_YFIRE , m_pobj->GetPos());
					CObj* pbullet = new CYama_bim(m_ptarget->GetObjNumber() , m_ptarget->GetPos());
					pbullet->SetPos(m_pobj->GetPos());
					pbullet->Initialize();
					CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
				}				
			}
		}
	}
	if(true == m_bready)
		m_curtex = (*m_yamaready_tex)[ int(m_frame.fcurframe) ];
	else
		m_curtex = (*m_yamafire_tex)[ int(m_frame.fcurframe) ];

	D3DXVECTOR2 veff_pos = m_pobj->GetPos();
	D3DXVECTOR2 vdir = m_pobj->GetcurDir();

	m_vPos = veff_pos + vdir*40;
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CYamaeff::Render(void)
{
	//if(!m_start)
	//	return;

	 CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	 CDevice::GetInstance()->GetSprite()->Draw(m_curtex->pTexture , NULL , &m_yamaready_center , NULL , D3DCOLOR_ARGB(255,255,255,255));
}
void CYamaeff::Release(void)
{

}