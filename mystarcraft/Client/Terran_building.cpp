#include "StdAfx.h"
#include "Terran_building.h"

#include "UI.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "Component.h"
#include "Com_AirPathfind.h"

#include "Building_Preview.h"
#include "Com_TBuildingAnim.h"

#include "UnitMgr.h"
#include "Ingame_UIMgr.h"
#include "LoopEff.h"
#include "TimeMgr.h"

#include "TileManager.h"
#include "FontMgr.h"

#include "SCV.h"
CTerran_building::CTerran_building(void)
{
	m_pfire_eff1 = NULL;
	m_pfire_eff2 = NULL;
	m_pfire_eff3 = NULL;

	D3DXMatrixIdentity(&m_matshadow);
	m_curtex = NULL;
	m_linktex = NULL;

	m_weight = D3DXVECTOR2(0,0);
	m_vgroundpos = D3DXVECTOR2(0,0);
	m_vairpos = D3DXVECTOR2(0,0);

	m_is_preview = false;
	m_is_partinstall = false;
	m_is_autoinstall = false;
	m_is_fire_render = false;
	m_is_rally = false;

	m_partbuilding = NULL;
	m_upg_info = NULL;

	m_main_preview = new CBuilding_Preview;
	m_sub_preview = new CBuilding_Preview;

	m_fbuild_tick = 0.f;
	m_build_hp = 0.f;

}

CTerran_building::~CTerran_building(void)
{
	Release();
	CIngame_UIMgr::GetInstance()->T_BuildTech_Update(m_ebuild_tech , -1);
}

void CTerran_building::Initialize(void)
{

}
void CTerran_building::building_area_Initialize(const int& col , const int& row)
{
	m_icol = col;
	m_irow = row;

	m_areaidx_vec.reserve(col*row);
	m_old_areaidx_vec.reserve(col*row);

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
	CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_obj_id , m_vPos.x , m_vPos.y);
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

	if(m_curidx64 != CMyMath::Pos_to_index(m_vPos , 64))
		CArea_Mgr::GetInstance()->ReleaseObj_Area64(m_curidx64 , this);
	else
		CArea_Mgr::GetInstance()->ReleaseObj_Area64(CMyMath::Pos_to_index(m_vPos , 64) , this);
	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	m_areaidx_vec.clear();
	m_old_areaidx_vec.clear();

	vector<int>().swap(m_areaidx_vec);
	vector<int>().swap(m_old_areaidx_vec);

	CIngame_UIMgr::GetInstance()->ClearPreview();
	Safe_Delete(m_main_preview);
	Safe_Delete(m_sub_preview);

	Safe_Delete(m_pfire_eff1);
	Safe_Delete(m_pfire_eff2);
	Safe_Delete(m_pfire_eff3);

	if(NULL != m_select_ui)
		Safe_Delete(m_select_ui);

	if(NULL != m_energybar_ui)
		Safe_Delete(m_energybar_ui);
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

void CTerran_building::Setlink(bool blink , CObj* pobj)
{
}

void CTerran_building::TakeOff(void)
{
	m_is_take_off = true;
	m_vairpos = m_vPos; //이륙하기전 기존 위치 저장
	((CCom_TBuildingAnim*)m_com_anim)->SetAirpos(m_vairpos);

	m_unitinfo.state = TAKE_OFF;
	m_unitinfo.eMoveType = MOVE_AIR;
	m_sortID = SORT_AIR;
	m_com_pathfind->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind));	
	area_release();
	m_oldidx64 = -1;
	if(NULL != m_partbuilding)
	{
		((CTerran_building*)m_partbuilding)->Setlink(false , NULL);
	}
}
void CTerran_building::Landing_move(D3DXVECTOR2 vpos)
{
	m_unitinfo.order = ORDER_LANDING_MOVE;
	m_vgroundpos.y = vpos.y + m_weight.y;//착륙하기전 착륙위치 저장
	((CCom_TBuildingAnim*)m_com_anim)->SetGroundpos(m_vgroundpos);

	vpos.x += m_weight.x;
	((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(vpos);
}

void CTerran_building::SetPartBuilding(CObj* pobj)
{
	m_partbuilding = pobj;
}

void CTerran_building::fire_eff_initialize(void)
{
	m_is_fire_render = false;
	m_pfire_eff1 = new CLoopEff(L"FIRE_A" , m_vPos , D3DXVECTOR2(1,1) , SORT_GROUND);
	m_pfire_eff2 = new CLoopEff(L"FIRE_C" , m_vPos , D3DXVECTOR2(1,1) , SORT_GROUND);
	m_pfire_eff3 = new CLoopEff(L"FIRE_B" , m_vPos , D3DXVECTOR2(1,1) , SORT_GROUND);

	m_pfire_eff1->Initialize();
	m_pfire_eff2->Initialize();
	m_pfire_eff3->Initialize();
}

void CTerran_building::fire_eff_update(void)
{
	if(BUILD == m_unitinfo.state)
		return;

	float fhp_ratio = float(m_unitinfo.hp) / float(m_unitinfo.maxhp);
	if( fhp_ratio <= 0.4f )
	{
		((CLoopEff*)m_pfire_eff1)->SetTexname(L"BIGFIRE_A");
		((CLoopEff*)m_pfire_eff2)->SetTexname(L"BIGFIRE_C");
		((CLoopEff*)m_pfire_eff3)->SetTexname(L"BIGFIRE_B");
		m_is_fire_render = true;
	}
	else if( fhp_ratio <= 0.7f )
	{
		m_is_fire_render = true;
	}
	else
	{
		m_is_fire_render = false;
	}

	if(true == m_is_fire_render)
	{
		m_pfire_eff1->Update();
		m_pfire_eff2->Update();
		m_pfire_eff3->Update();

		m_pfire_eff1->SetPos(m_vPos.x - m_vertex.left/2 ,m_vPos.y - m_vertex.top/2);
		m_pfire_eff2->SetPos(m_vPos.x , m_vPos.y - m_vertex.top/2);
		m_pfire_eff3->SetPos(m_vPos.x + m_vertex.right/2 ,m_vPos.y - m_vertex.top/2);
	}
}

void CTerran_building::fire_eff_render(void)
{
	if(true == m_is_fire_render)
	{
		m_pfire_eff1->Render();
		m_pfire_eff2->Render();
		m_pfire_eff3->Render();
	}
}

void CTerran_building::Build_Complete(void)
{
	CIngame_UIMgr::GetInstance()->T_BuildTech_Update(m_ebuild_tech , 1);
}

void CTerran_building::upginfo_update(const UPGRADE& eupg)
{
	if(true == m_upg_info[eupg].proceeding &&
		m_upg_info[eupg].obj_num == m_obj_id)
	{
		m_upg_info[eupg].curtime += GETTIME;

		if( m_upg_info[eupg].curtime >= m_upg_info[eupg].maxtime )
		{
			m_upg_info[eupg].curtime = 0;
			m_upg_info[eupg].proceeding = false;
			m_upg_info[eupg].upg_cnt += 1;
			m_upg_info[eupg].maxtime += 0.f;
			m_upg_info[eupg].obj_num = 0;

			m_unitinfo.state = IDLE;
		}
	}
}
