#include "StdAfx.h"
#include "Obj.h"
#include "MyMath.h"

#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "Device.h"
CObj::CObj(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ePosKind = STATIC_OBJ;
	m_pSprite = NULL;

	m_curidx32 = -1;
	m_oldidx32 = -1;
	m_oldidx64 = -1;
	m_oldidx512 = -1;
	m_curidx64 = -1;
	m_curidx512 = -1;

	m_bdestroy = false;

	m_obj_id += 1;
}

CObj::~CObj(void)
{
	m_obj_id -= 1;
}

void CObj::Initialize(void)
{
	m_pSprite = CDevice::GetInstance()->GetSprite();

	m_curidx64 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 64);
	m_curidx512 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 512);
	m_oldidx64 = m_curidx64;
	m_oldidx512 = m_curidx512;
}

void CObj::Update(void)
{

}
void CObj::idx_update(void)
{
	//m_curidx32 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 64);
	m_curidx512 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 512);

	//if(m_oldidx32 != m_curidx32)
	//	m_oldidx32 = m_curidx32;

	if(m_oldidx64 != m_curidx64)
	{
		CArea_Mgr::GetInstance()->SetObj_Area64(m_curidx64 , m_oldidx64 , this);
		m_oldidx64 = m_curidx64;
	}

	if(m_oldidx512 != m_curidx512)
	{
		CArea_Mgr::GetInstance()->SetObj_Area512(m_curidx512 , m_oldidx512 , this);
		m_oldidx512 = m_curidx512;
	}
}
void CObj::SetDestroy(bool bdestroy)
{
	m_bdestroy = bdestroy;
}
void CObj::Render(void)
{
}

void CObj::Release(void)
{

}
void CObj::SetPos(const float x , const float y, OBJ_POS_KIND ekind /*= STATIC_OBJ*/)
{
	m_ePosKind = ekind;
	m_vPos.x = x;
	m_vPos.y = y;
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CObj::InitialRect(void)
{

}

bool CObj::Be_in_camera(void)
{
	if( m_vPos.x >= CScrollMgr::m_fScrollX && 
		m_vPos.x <= CScrollMgr::m_fScrollX + CLINETSIZE_X &&
		m_vPos.y >= CScrollMgr::m_fScrollY &&
		m_vPos.y <= CScrollMgr::m_fScrollY + CLINETSIZE_Y)
	{
		return true;
	}

	return false;
}

float CObj::GetY(void)
{
	return m_vPos.y;
}

SORT_ID CObj::GetsortID(void)
{
	return m_sortID;
}

bool CObj::GetDestroy(void)
{
	return m_bdestroy;
}

const MYRECT<float>& CObj::GetMyRect(void)
{
	return m_rect;
}

const D3DXVECTOR2& CObj::GetPos(void)
{
	return m_vPos;
}

void CObj::Inputkey_reaction(const int& nkey)
{

}

OBJ_TYPE CObj::GetType(void)
{
	return m_eType;
}

int CObj::GetObjID(void)
{
	return m_obj_id;
}

D3DXVECTOR2& CObj::GetReferencePos(void)
{
	return m_vPos;
}

int CObj::m_obj_id;