#include "StdAfx.h"
#include "Terran_building.h"

#include "UI.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "Component.h"
#include "Com_AirPathfind.h"

#include "Building_Preview.h"
#include "Com_TBuildingAnim.h"
CTerran_building::CTerran_building(void)
{
	D3DXMatrixIdentity(&m_matshadow);
	m_curtex = NULL;
	m_linktex = NULL;
	m_weight = D3DXVECTOR2(0,0);
	m_is_preview = false;
	m_is_partinstall = false;
	m_is_autoinstall = false;

	m_partbuilding = NULL;

	m_main_preview = new CBuilding_Preview;
	m_sub_preview = new CBuilding_Preview;
}

CTerran_building::~CTerran_building(void)
{
	Release();
	m_areaidx_vec.clear();
	m_old_areaidx_vec.clear();

	vector<int>().swap(m_areaidx_vec);
	vector<int>().swap(m_old_areaidx_vec);

	Safe_Delete(m_main_preview);
	Safe_Delete(m_sub_preview);
}

void CTerran_building::Initialize(void)
{

}
void CTerran_building::building_area_Initialize(const int& col , const int& row)
{
	m_icol = col;
	m_irow = row;

	m_areaidx_vec.resize(col*row);
	m_old_areaidx_vec.resize(col*row);

	//여기서 m_vpos는 인덱스위치이다. 

	D3DXVECTOR2	vstartpos = m_vPos - m_weight;
	D3DXVECTOR2 vpos;
	
	int vecidx;
	for(int i = 0; i < col; ++i)
	{
		vpos.y = vstartpos.y + 32 * i;
		for(int j = 0; j < row; ++j)
		{
			vecidx = i * row + j;
			vpos.x = vstartpos.x + 32 * j;

			m_areaidx_vec[vecidx] = CMyMath::Pos_to_index(vpos , 64);
			m_old_areaidx_vec[vecidx] = CMyMath::Pos_to_index(vpos , 64);
			CArea_Mgr::GetInstance()->SetObj_Area64(m_areaidx_vec[vecidx] , m_areaidx_vec[vecidx] , this );
		}
	}
}
void CTerran_building::building_pos_Initialize(const int& col , const int& row)
{
	if(row % 2 == 0)
	{
		m_vPos.x += (row/2) * 32 - 16;  //이건 추후 바뀔 수 있다..
		m_weight.x = float((row/2) * 32 - 16);
	}
	else
	{
		m_vPos.x += (row/2) * 32;
		m_weight.x = float((row/2) * 32);
	}


	if(col % 2 == 0)
	{
		m_vPos.y += (col/2) * 32 - 16;  
		m_weight.y = float((col/2) * 32 - 16);
	}
	else
	{
		m_vPos.y += (col/2) * 32;  		
		m_weight.y = float((col/2) * 32);
	}

	m_curidx32 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 64);

	m_oldidx64 = m_curidx64;
}

void CTerran_building::building_area_update(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos , 64);

	if(m_oldidx32 != m_curidx32)
	{
		m_oldidx32 = m_curidx32;
	}

	if(m_oldidx64 != m_curidx64)
	{
		CArea_Mgr::GetInstance()->SetObj_Area64(m_curidx64 , m_oldidx64 , this );

		m_oldidx64 = m_curidx64;
	}
}
void CTerran_building::Update(void)
{

}

void CTerran_building::Render(void)
{

}

void CTerran_building::Release(void)
{
	if(NULL != m_select_ui)
		m_select_ui->SetDestroy(true);
}

void CTerran_building::Dead(void)
{

}

void CTerran_building::area_release(void)
{
	size_t maxloop = m_areaidx_vec.size();
	for(size_t i = 0; i < maxloop; ++i)
	{
		CArea_Mgr::GetInstance()->ReleaseObj_Area64(m_areaidx_vec[i] , this);
	}

}

void CTerran_building::Inputkey_reaction(const int& nkey)
{

}

void CTerran_building::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CTerran_building::Setlink(bool blink)
{

}

void CTerran_building::TakeOff(void)
{
	m_is_take_off = true;
	m_vairpos = m_vPos; //이륙하기전 기존 위치 저장
	((CCom_TBuildingAnim*)m_com_anim)->SetAirpos(m_vairpos);

	m_unitinfo.estate = TAKE_OFF;
	m_unitinfo.eMoveType = MOVE_AIR;
	m_sortID = SORT_AIR;
	m_com_pathfind->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind));	
	area_release();
	m_oldidx64 = -1;
	if(NULL != m_partbuilding)
	{
		((CTerran_building*)m_partbuilding)->Setlink(false);
	}
}
void CTerran_building::Landing_move(D3DXVECTOR2 vpos)
{
	m_unitinfo.eorder = ORDER_LANDING_MOVE;
	m_vgroundpos.y = vpos.y + m_weight.y;//착륙하기전 착륙위치 저장
	((CCom_TBuildingAnim*)m_com_anim)->SetGroundpos(m_vgroundpos);

	vpos.x += m_weight.x;
	((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(vpos);
}

void CTerran_building::SetPartBuilding(CObj* pobj)
{
	m_partbuilding = pobj;
}
