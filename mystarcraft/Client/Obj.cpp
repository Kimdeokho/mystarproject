#include "StdAfx.h"
#include "Obj.h"
#include "MyMath.h"

#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "Device.h"
#include "Component.h"
#include "UnitMgr.h"
#include "UI_Select.h"

#include "TileManager.h"

#include "FontMgr.h"
#include "ScrollMgr.h"

CObj::CObj(void)
{
	m_select_ui = NULL;
	m_energybar_ui = NULL;
	m_pSprite = CDevice::GetInstance()->GetSprite();

	D3DXMatrixIdentity(&m_matWorld);
	m_ePosKind = STATIC_OBJ;
	

	m_curidx32 = -1;
	m_oldidx32 = -1;

	m_oldidx64 = -1;	
	m_curidx64 = -1;


	m_bdestroy = false;
	m_bSelect = false;

	m_globalobj_id += 1;

	m_vcurdir = OFFSET_DIRVEC;

	m_curdiridx = 0;
	m_obj_id = 0;
	m_bmagicbox = false;

}

CObj::~CObj(void)
{
	Release();
}

void CObj::Initialize(void)
{	
}
void CObj::unit_area_Initialize(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 64);

	m_oldidx32 = m_curidx32;
	m_oldidx64 = m_curidx64;

	CArea_Mgr::GetInstance()->SetObj_Area64(m_curidx64 , m_curidx64 , this );
}
void CObj::area_update(void)
{
	if(false == m_unitinfo.is_active ||
		ORDER_BUNKER_BOARDING == m_unitinfo.order)
		return;

	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos , 64);

	if(m_oldidx32 != m_curidx32)
	{
		//if( MOVE != m_unitinfo.state && BURROW != m_unitinfo.state)
		
		//CTileManager::GetInstance()->SetFlowfield_cost_update(m_curidx32 , m_oldidx32);
		m_oldidx32 = m_curidx32;		
	}

	if(m_oldidx64 != m_curidx64)
	{
		CArea_Mgr::GetInstance()->SetObj_Area64(m_curidx64 , m_oldidx64 , this );
		m_oldidx64 = m_curidx64;

	}
}
void CObj::area_release(void)
{
	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	if(m_curidx64 != CMyMath::Pos_to_index(m_vPos , 64))
		CArea_Mgr::GetInstance()->ReleaseObj_Area64(m_curidx64 , this);
	else
		CArea_Mgr::GetInstance()->ReleaseObj_Area64(CMyMath::Pos_to_index(m_vPos , 64) , this);
}


void CObj::Update(void)
{

}


void CObj::Render(void)
{
}

void CObj::Release(void)
{
	if(!m_componentlist.empty())
	{
		COMPONENT_PAIR::iterator iter = m_componentlist.begin();
		COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete(iter->second);
		}
		m_componentlist.clear();
	}
}
void CObj::SetPos(const float x , const float y, OBJ_POS_KIND ekind /*= STATIC_OBJ*/)
{
	m_ePosKind = ekind;
	m_vPos.x = x;
	m_vPos.y = y;
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CObj::SetPos(const D3DXVECTOR2& vpos)
{
	m_vPos = vpos;

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
//MOVE_TYPE CObj::GetType(void)
//{
//	return m_eType;
//}

int CObj::GetObjNumber(void)
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

void CObj::SetSelect(SELECT_FLAG eflag ,D3DCOLOR ecolor)
{
	if(NULL != m_select_ui)
	{
		((CUI_Select*)m_select_ui)->SetSelect(eflag , ecolor);
	}

	if(eflag == NONE_SELECT)
	{
		m_bSelect = false;
		if(NULL != m_energybar_ui)
			m_energybar_ui->SetActive(false);
	}
	else
	{
		if(eflag != BLINK_SELECT)
		{
			m_bSelect = true;
			if(NULL != m_energybar_ui)
				m_energybar_ui->SetActive(true);		
		}
	}
}
void CObj::SetState(STATE state)
{
	m_unitinfo.state = state;
}

CATEGORY CObj::GetCategory(void)
{
	return m_ecategory;
}

CComponent* CObj::GetComponent(COMPONENT_LIST ecom_name)
{
	COMPONENT_PAIR::iterator iter;
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

const int& CObj::Getcuridx(const int& tilesize)
{
	if(32 == tilesize)
	{
		return m_curidx32;
	}
	else if(64 ==  tilesize)
	{
		return m_curidx64;
	}

	return m_curidx32;
}
const MYRECT<float>& CObj::GetVertex(void)
{
	return m_vertex;
}

void CObj::SetOrder(ORDER order)
{
	//m_order = order;
	m_unitinfo.order = order;
}

const D3DXMATRIX& CObj::GetMat(void)
{
	return m_matWorld;
}
void CObj::SetActive(bool is_active)
{
	m_unitinfo.is_active = is_active;
}
void CObj::SetSpeed(const float& fspeed)
{
	m_unitinfo.fspeed = fspeed;
}

void CObj::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
{
	//m_hp -= idamage;
	float tempdamage = (float)idamage - m_unitinfo.armor;
	if( ARMOR_SMALL == m_unitinfo.eArmorType)
	{
		if(DAMAGE_BOOM == edamagetype)
			tempdamage *= 0.5f;
	}
	else if( ARMOR_MEDIUM == m_unitinfo.eArmorType)
	{
		if(DAMAGE_VIBRATE == edamagetype)
			tempdamage *= 0.5f;
		else if(DAMAGE_BOOM == edamagetype)
			tempdamage *= 0.75f;
	}
	else if( ARMOR_LARGE == m_unitinfo.eArmorType)
	{
		if(DAMAGE_VIBRATE == edamagetype)
			tempdamage *= 0.25f;
	}

	m_unitinfo.hp -= (int)(tempdamage + 0.5f);

	if(m_unitinfo.hp <= 0)
	{
		//킬수 + 1 하면 되는디..
		m_unitinfo.hp = 0;
		m_bdestroy = true;
		Dead();
	}
	if(m_unitinfo.hp >= m_unitinfo.maxhp)
	{
		m_unitinfo.hp = m_unitinfo.maxhp;
	}
}

void CObj::SetMP(const int& imp)
{
	m_unitinfo.mp += imp;

	if(m_unitinfo.mp <= 0)
		m_unitinfo.mp = 0;
}
// int CObj::GetHP(void)
// {
// 	return m_hp;
// }

void CObj::SetObjID(const int& id)
{
	m_obj_id = id;
}

const	UNITINFO& CObj::GetUnitinfo()
{
	return m_unitinfo;
}

void CObj::Setdiridx(const int& diridx)
{
	m_curdiridx = diridx;
}

void CObj::SetTeamNumber(TEAM_NUMBER eteamnum)
{
	m_eteamnumber = eteamnum;
}

void CObj::SetMagicBox(bool bmagicbox)
{
	m_bmagicbox = bmagicbox;
}

void CObj::SetObjName(OBJID eobjname)
{
	m_eOBJ_NAME = eobjname;
}

void CObj::Dead(void)
{

}

OBJID CObj::GetOBJNAME(void)
{
	return m_eOBJ_NAME;
}

void CObj::SetDestroy(bool bdestroy)
{
	m_bdestroy = bdestroy;
}

void CObj::Update_Cmdbtn(void)
{

}

void CObj::Update_Wireframe(void)
{

}

void CObj::SetWait(bool is_wait)
{
	m_unitinfo.is_wait = is_wait;
}

void CObj::SetHP(const int& ihp)
{
	m_unitinfo.hp += ihp;
}



int CObj::m_globalobj_id;