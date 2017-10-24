#include "StdAfx.h"
#include "GasBuilding.h"

#include "MyMath.h"
#include "Area_Mgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "Gas_frag.h"
#include "Workman.h"
#include "ScrollMgr.h"
#include "FontMgr.h"
#include "GasResource.h"
CGasBuilding::CGasBuilding(void)
{
	m_pworkman = NULL;
	m_fgather_time = 0.f;
	D3DXMatrixIdentity(&m_matshadow);
}

CGasBuilding::~CGasBuilding(void)
{
	m_areaidx_vec.clear();
	m_old_areaidx_vec.clear();

	vector<int>().swap(m_areaidx_vec);
	vector<int>().swap(m_old_areaidx_vec);

	Release();
}

void CGasBuilding::Initialize(void)
{

}
void CGasBuilding::building_area_Initialize(const int& col , const int& row)
{
	m_icol = col;
	m_irow = row;

	m_areaidx_vec.resize(col*row);
	m_old_areaidx_vec.resize(col*row);

	//여기서 m_vpos는 인덱스위치이다. 


	D3DXVECTOR2	vpos = m_vPos;

	int vecidx;
	for(int i = 0; i < col; ++i)
	{
		vpos.y = m_vPos.y + 32 * i;

		for(int j = 0; j < row; ++j)
		{
			vecidx = i * row + j;
			vpos.x = m_vPos.x + 32 * j;

			m_areaidx_vec[vecidx] = CMyMath::Pos_to_index(vpos , 64);
			m_old_areaidx_vec[vecidx] = CMyMath::Pos_to_index(vpos , 64);
			CArea_Mgr::GetInstance()->SetObj_Area64(m_areaidx_vec[vecidx] , m_areaidx_vec[vecidx] , this );
		}
	}


	if(row % 2 == 0)
	{
		m_vPos.x += (row/2) * 32 - 16;  
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
void CGasBuilding::Update(void)
{

	if(m_bcomeon)
	{
		m_fgather_time += GETTIME;

		if(m_fgather_time > 2.0f)
		{
			m_fgather_time = 0.f;
			if(NULL != m_pworkman)
			{			
				//배출알고리즘...
				MYRECT<float>	collocate_rc;
				MYRECT<float>	workman_vtx = m_pworkman->GetVertex();

				int stepsize = 16;
				int loopcnt = 0;
				int widthcnt = (32/stepsize)*m_irow;
				int heightcnt = (32/stepsize)*m_icol + 2;
				int idx64;
				float fmindistance = 10000000;

				D3DXVECTOR2 collocate_pos[4];
				D3DXVECTOR2	temp_pos[4];
				D3DXVECTOR2	result_pos;

				bool bescape = false;


				while(!bescape)
				{				
					collocate_pos[0].x = m_vPos.x - m_weight.x - loopcnt*32; //밑줄 오른쪽방향
					collocate_pos[0].y = m_vPos.y + m_weight.y + 32 + loopcnt*32;

					collocate_pos[1].x = m_vPos.x + m_weight.x + 32 + loopcnt*32; //오른줄 위쪽방향
					collocate_pos[1].y = m_vPos.y + m_weight.y + 32 + loopcnt*32;

					collocate_pos[2].x = m_vPos.x + m_weight.x + loopcnt*32; //윗줄 왼쪽방향
					collocate_pos[2].y = m_vPos.y - m_weight.y - 32 - loopcnt*32;

					collocate_pos[3].x = m_vPos.x - m_weight.x - 32 - loopcnt*32; //왼줄 아래쪽방향
					collocate_pos[3].y = m_vPos.y - m_weight.y - 32 - loopcnt*32;

					memcpy(temp_pos , collocate_pos , sizeof(D3DXVECTOR2)*4);

					for(int i = 0; i < widthcnt; ++i)
					{

						temp_pos[0].x = collocate_pos[0].x + i*stepsize;

						collocate_rc.left = temp_pos[0].x - workman_vtx.left; 
						collocate_rc.right = temp_pos[0].x + workman_vtx.right;
						collocate_rc.top = temp_pos[0].y - workman_vtx.top;
						collocate_rc.bottom = temp_pos[0].y + workman_vtx.bottom;
						idx64 = CMyMath::Pos_to_index(temp_pos[0] , 64);

						CFontMgr::GetInstance()->Setbatch_Font(L"@" , temp_pos[0].x - CScrollMgr::m_fScrollX ,
							temp_pos[0].y - CScrollMgr::m_fScrollY);

						if(true == CArea_Mgr::GetInstance()->Collocate_check(m_pworkman , idx64 , collocate_rc ))
						{
							result_pos = temp_pos[0];
							bescape = true;
							break;
						}
					}
					if(bescape)
						break;

					for(int i = 0; i < heightcnt; ++i)
					{					
						temp_pos[1].y = collocate_pos[1].y - i*stepsize;;

						collocate_rc.left = temp_pos[1].x - workman_vtx.left; 
						collocate_rc.right = temp_pos[1].x + workman_vtx.right;
						collocate_rc.top = temp_pos[1].y - workman_vtx.top;
						collocate_rc.bottom = temp_pos[1].y + workman_vtx.bottom;
						idx64 = CMyMath::Pos_to_index(temp_pos[1] , 64);

						if(true == CArea_Mgr::GetInstance()->Collocate_check(m_pworkman , idx64 , collocate_rc ))
						{
							result_pos = temp_pos[1];
							bescape = true;
							break;
						}
					}
					if(bescape)
						break;

					for(int i = 0; i < widthcnt; ++i)
					{					
						temp_pos[2].x = collocate_pos[2].x - i*stepsize;;

						collocate_rc.left = temp_pos[2].x - workman_vtx.left; 
						collocate_rc.right = temp_pos[2].x + workman_vtx.right;
						collocate_rc.top = temp_pos[2].y - workman_vtx.top;
						collocate_rc.bottom = temp_pos[2].y + workman_vtx.bottom;
						idx64 = CMyMath::Pos_to_index(temp_pos[2] , 64);

						if(true == CArea_Mgr::GetInstance()->Collocate_check(m_pworkman , idx64 , collocate_rc ))
						{
							result_pos = temp_pos[2];
							bescape = true;
							break;
						}

					}
					if(bescape)
						break;

					for(int i = 0; i < heightcnt; ++i)
					{
						temp_pos[3].y = collocate_pos[3].y + i*stepsize;;

						collocate_rc.left = temp_pos[3].x - workman_vtx.left; 
						collocate_rc.right = temp_pos[3].x + workman_vtx.right;
						collocate_rc.top = temp_pos[3].y - workman_vtx.top;
						collocate_rc.bottom = temp_pos[3].y + workman_vtx.bottom;
						idx64 = CMyMath::Pos_to_index(temp_pos[3] , 64);

						if(true == CArea_Mgr::GetInstance()->Collocate_check(m_pworkman , idx64 , collocate_rc ))
						{
							result_pos = temp_pos[3];
							bescape = true;
							break;
						}
					}
					if(bescape)
						break;


					//여기서 커맨드센터와 가장 가까운걸 고른다.

					widthcnt += 2;
					heightcnt += 2;
					loopcnt += 1;
				}

				((CGasResource*)m_pgas_resource)->setgas_amount(-8);//가스를 빼간다..

				m_pworkman->SetPos(result_pos);
				//워크맨쉑을 배출한다..
				CObj* pfragment = NULL;
				pfragment = new CGas_frag;
				CObjMgr::GetInstance()->AddObject(pfragment , OBJ_FRAGMENT);
				pfragment->Initialize();
				((CWorkman*)m_pworkman)->setgas_fragment(pfragment);

				
				m_pworkman->SetState(IDLE);
				m_pworkman->SetOrder(ORDER_RETURN_CARGO);
				m_pworkman->unit_area_Initialize();
				m_pworkman = NULL;
				m_bcomeon = false;
			}
		}
	}

}
void CGasBuilding::Render(void)
{

}

void CGasBuilding::Release(void)
{
}
void CGasBuilding::area_release(void)
{
	size_t maxloop = m_areaidx_vec.size();
	for(size_t i = 0; i < maxloop; ++i)
	{
		CArea_Mgr::GetInstance()->ReleaseObj_Area64(m_areaidx_vec[i] , this);
	}
}

void CGasBuilding::Dead(void)
{

}
CObj* CGasBuilding::Getworkman(void)
{
	return m_pworkman;
}
void CGasBuilding::Setworkman(CObj* pobj)
{
	m_bcomeon = true;
	m_fgather_time = 0.f;
	m_pworkman = pobj;
}

void CGasBuilding::Setgas_resource(CObj* pobj)
{
	m_pgas_resource = pobj;
}
