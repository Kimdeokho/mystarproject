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
	
	m_curidx64 = -1;
	m_oldidx256 = -1;
	m_curidx256 = -1;

	m_bdestroy = false;
	m_bSelect = false;

	m_globalobj_id += 1;
	m_obj_id = m_globalobj_id;

	m_vcurdir = OFFSET_DIRVEC;
}

CObj::~CObj(void)
{
	Release();
}

void CObj::Initialize(void)
{
	m_pSprite = CDevice::GetInstance()->GetSprite();

	m_curidx32 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 64);
	m_curidx256 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 256);
	m_curidx512 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 512);

	m_oldidx64 = m_curidx64;
	m_oldidx256 = m_curidx256;
	m_oldidx512 = m_curidx512;

	CArea_Mgr::GetInstance()->SetObj_Area512(m_curidx512 , m_curidx512 , this);
	CArea_Mgr::GetInstance()->SetObj_Area256(m_curidx256 , m_curidx256 , this);
	CArea_Mgr::GetInstance()->SetObj_Area64(m_curidx64 , m_curidx64 , this);
	
}

void CObj::Update(void)
{

}
void CObj::idx_update(void)
{
	//ÀÌ°Å ¹¹¾ß???????

	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos , 64);
	m_curidx256 = CMyMath::Pos_to_index(m_vPos , 256);
	m_curidx512 = CMyMath::Pos_to_index(m_vPos , 512);

	if(m_oldidx32 != m_curidx32)
		m_oldidx32 = m_curidx32;

	if(m_oldidx64 != m_curidx64)
	{
		CArea_Mgr::GetInstance()->SetObj_Area64(m_curidx64 , m_oldidx64 , this);
		m_oldidx64 = m_curidx64;
	}

	if(m_oldidx256 != m_curidx256)
	{
		CArea_Mgr::GetInstance()->SetObj_Area256(m_curidx256 , m_oldidx256 , this);
		m_oldidx256 = m_curidx256;
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
	CArea_Mgr::GetInstance()->ReleaseObj_Area64(m_curidx64 , this);
	CArea_Mgr::GetInstance()->ReleaseObj_Area256(m_curidx256 , this);
	CArea_Mgr::GetInstance()->ReleaseObj_Area512(m_curidx512 , this);
}
void CObj::SetPos(const float x , const float y, OBJ_POS_KIND ekind /*= STATIC_OBJ*/)
{
	m_ePosKind = ekind;
	m_vPos.x = x;
	m_vPos.y = y;
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
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
float CObj::GetX(void)
{
	return m_vPos.x;
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
	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;

	return m_rect;
}

const D3DXVECTOR2& CObj::GetPos(void)
{
	return m_vPos;
}
const D3DXVECTOR2* CObj::GetPosAdress(void)
{
	return &m_vPos;
}
void CObj::Inputkey_reaction(const int& nkey)
{

}
void CObj::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
}
MOVE_TYPE CObj::GetType(void)
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

TEAM_NUMBER CObj::GetTeamNumber(void)
{
	return m_eteamnumber;
}

void CObj::SetSelect(bool bselect)
{
	m_bSelect = bselect;
}
void CObj::SetState(STATE estate)
{
	m_estate = estate;
}

STATE CObj::GetState(void)
{
	return m_estate;
}

KATEGORIE CObj::GetKategorie(void)
{
	return m_ekategorie;
}

CComponent* CObj::GetComponent(COMPONENT_LIST ecom_name)
{
	boost::unordered_map<COMPONENT_LIST , CComponent*>::iterator iter;
	iter = m_componentlist.find(ecom_name);

	if(iter != m_componentlist.end())
		return iter->second;

	return NULL;
}

void CObj::Setdir(D3DXVECTOR2& vdir)
{
	//D3DXVec2Normalize(&m_vcurdir , &vdir);
	m_vcurdir = vdir;
}

const D3DXVECTOR2& CObj::GetcurDir(void)
{
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
	return m_vcurdir;
}

int CObj::Getcuridx(const int& tilesize)
{
	if(32 == tilesize)
	{
		return m_curidx32;
	}
	else if(64 ==  tilesize)
	{
		return m_curidx64;
	}
	else if(256 ==  tilesize)
	{
		return m_curidx256;
	}
	else if(512 ==  tilesize)
	{ 
		return m_curidx512;
	}

	return -1;
}

ORDER CObj::GetOrder(void)
{
	return m_eorder;
}

const MYRECT<float>& CObj::GetVertex(void)
{
	return m_vertex;
}

void CObj::SetOrder(ORDER eorder)
{
	m_eorder = eorder;
}

float* CObj::GetSpeed(void)
{
	return &m_fspeed;
}

int CObj::m_globalobj_id;