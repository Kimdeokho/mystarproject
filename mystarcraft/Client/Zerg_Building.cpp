#include "StdAfx.h"
#include "Zerg_Building.h"

#include "MyMath.h"
#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Area_Mgr.h"
#include "UnitMgr.h"
#include "Ingame_UIMgr.h"

#include "UI_MiniUnitDisplay.h"
CZerg_building::CZerg_building(void)
{
	m_build_hp = 0.f;
	m_build_maxhp = 0;

	m_fbuild_tick = 0.f;
	m_weight = D3DXVECTOR2(0,0);
	m_com_anim = NULL;
	m_com_creep = NULL;

	m_unitinfo.etribe = TRIBE_ZERG;

	m_miniunit_display = new CUI_MiniUnitDisplay(m_vPos , &m_eteamnumber);
	m_miniunit_display->Initialize();
	CIngame_UIMgr::GetInstance()->SetMiniUnit_display(m_miniunit_display);

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

CZerg_building::~CZerg_building(void)
{
	Release();
	//CIngame_UIMgr::GetInstance()->T_BuildTech_Update(m_ebuild_tech , -1);
}

void CZerg_building::Initialize(void)
{
}

void CZerg_building::Update(void)
{

}

void CZerg_building::Render(void)
{

}

void CZerg_building::building_area_Initialize(const int& col , const int& row)
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
void CZerg_building::building_pos_Initialize(const int& col , const int& row)
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
void CZerg_building::upginfo_update(const UPGRADE eupg)
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
void CZerg_building::Build_Complete(void)
{

}

void CZerg_building::Release(void)
{
	size_t maxloop = m_areaidx_vec.size();
	for(size_t i = 0; i < maxloop; ++i)
	{
		CArea_Mgr::GetInstance()->ReleaseObj_Area64(m_areaidx_vec[i] , this);
	}


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

	if(NULL != m_select_ui)
		Safe_Delete(m_select_ui);

	if(NULL != m_energybar_ui)
		Safe_Delete(m_energybar_ui);

	if(NULL != m_miniunit_display)
		m_miniunit_display->SetDestroy(true);

}