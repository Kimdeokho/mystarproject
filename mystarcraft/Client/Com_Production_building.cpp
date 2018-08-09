#include "StdAfx.h"
#include "Com_Production_building.h"

#include "Obj.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "Ingame_UIMgr.h"

#include "Com_Pathfind.h"
#include "Com_AirPathfind.h"

#include "SCV.h"
#include "Tank.h"
#include "Marine.h"
#include "Firebat.h"
#include "Medic.h"
#include "Vulture.h"
#include "Tank.h"
#include "Goliath.h"
#include "Wraith.h"
#include "BattleCruiser.h"
#include "Dropship.h"
#include "Ghost.h"

CCom_Production_building::CCom_Production_building(const D3DXVECTOR2& vpos , const D3DXVECTOR2& vweight , const int& icol , const int& irow)
:m_vPos(vpos) , m_weight(vweight) , m_irow(irow) , m_icol(icol)
{
}

CCom_Production_building::~CCom_Production_building(void)
{
}

void CCom_Production_building::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_is_rally = false;
}

void CCom_Production_building::Update(void)
{
	update_production();
}
void CCom_Production_building::unit_collocate(CObj* const pobj)
{
	MYRECT<float>	collocate_rc;
	MYRECT<float>	vtx = pobj->GetVertex();
	MYRECT<float>	building_vtx = m_pobj->GetVertex();

	int stepsize = 8;
	int loopcnt = 0;
	int widthcnt = (32/stepsize)*m_irow + 1;
	int heightcnt = (32/stepsize)*m_icol + 2;
	int idx64;

	D3DXVECTOR2 collocate_pos[4];
	D3DXVECTOR2	temp_pos[4];
	D3DXVECTOR2	result_pos;

	bool bescape = false;
	int idx32 = 0;

	
	while(!bescape)
	{				
		collocate_pos[0].x = m_vPos.x - m_weight.x - loopcnt*32; //밑줄 오른쪽방향
		collocate_pos[0].y = m_vPos.y + building_vtx.bottom + vtx.top + loopcnt*32;

		collocate_pos[1].x = m_vPos.x + building_vtx.right + vtx.left + loopcnt*32; //오른줄 위쪽방향
		collocate_pos[1].y = m_vPos.y + m_weight.y + loopcnt*32;

		collocate_pos[2].x = m_vPos.x + m_weight.x + loopcnt*32; //윗줄 왼쪽방향
		collocate_pos[2].y = m_vPos.y - building_vtx.top - vtx.bottom - loopcnt*32;

		collocate_pos[3].x = m_vPos.x - building_vtx.left - vtx.right - loopcnt*32; //왼줄 아래쪽방향
		collocate_pos[3].y = m_vPos.y - m_weight.y - 32 - loopcnt*32;


		memcpy(temp_pos , collocate_pos , sizeof(D3DXVECTOR2)*4);

		int begin[4] = {0};
		int end[4] = {0};

		begin[0] = (int)collocate_pos[0].x;
		end[0] = int(m_vPos.x + building_vtx.right + loopcnt*32);

		begin[1] = (int)collocate_pos[1].y;
		end[1] = int(m_vPos.y - building_vtx.top - loopcnt*32);

		begin[2] = (int)collocate_pos[2].x;
		end[2] = int(m_vPos.x - building_vtx.left - loopcnt*32);

		begin[3] = (int)collocate_pos[3].y;
		end[3] = int(m_vPos.y + building_vtx.bottom + loopcnt*32);

		for( int i = begin[0]; i < end[0]; i += stepsize)
		{
			temp_pos[0].x = (float)i;

			collocate_rc.left = temp_pos[0].x - vtx.left; 
			collocate_rc.right = temp_pos[0].x + vtx.right;
			collocate_rc.top = temp_pos[0].y - vtx.top;
			collocate_rc.bottom = temp_pos[0].y + vtx.bottom;
			idx64 = CMyMath::Pos_to_index(temp_pos[0] , 64);

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , idx64 , collocate_rc ))
			{				
				idx32 = CMyMath::Pos_to_index(temp_pos[0] , 32);
				if(MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx32))
				{
					result_pos = temp_pos[0];
					bescape = true;
					break;
				}
			}
		}
		if(bescape)
			break;

		for( int i = begin[1]; i >= end[1]; i -= stepsize)
		{					
			temp_pos[1].y = (float)i;

			collocate_rc.left = temp_pos[1].x - vtx.left; 
			collocate_rc.right = temp_pos[1].x + vtx.right;
			collocate_rc.top = temp_pos[1].y - vtx.top;
			collocate_rc.bottom = temp_pos[1].y + vtx.bottom;
			idx64 = CMyMath::Pos_to_index(temp_pos[1] , 64);

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , idx64 , collocate_rc ))
			{
				idx32 = CMyMath::Pos_to_index(temp_pos[1] , 32);
				if(MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx32))
				{
					result_pos = temp_pos[1];
					bescape = true;
					break;
				}
			}
		}
		if(bescape)
			break;

		for( int i = begin[2]; i >= end[2]; i -= stepsize)
		{					
			temp_pos[2].x = (float)i;

			collocate_rc.left = temp_pos[2].x - vtx.left; 
			collocate_rc.right = temp_pos[2].x + vtx.right;
			collocate_rc.top = temp_pos[2].y - vtx.top;
			collocate_rc.bottom = temp_pos[2].y + vtx.bottom;
			idx64 = CMyMath::Pos_to_index(temp_pos[2] , 64);

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , idx64 , collocate_rc ))
			{
				idx32 = CMyMath::Pos_to_index(temp_pos[2] , 32);
				if(MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx32))
				{
					result_pos = temp_pos[2];
					bescape = true;
					break;
				}
			}

		}
		if(bescape)
			break;

		for( int i = begin[3]; i < end[3]; i += stepsize)
		{
			temp_pos[3].y = (float)i;

			collocate_rc.left = temp_pos[3].x - vtx.left; 
			collocate_rc.right = temp_pos[3].x + vtx.right;
			collocate_rc.top = temp_pos[3].y - vtx.top;
			collocate_rc.bottom = temp_pos[3].y + vtx.bottom;
			idx64 = CMyMath::Pos_to_index(temp_pos[3] , 64);

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , idx64 , collocate_rc ))
			{
				idx32 = CMyMath::Pos_to_index(temp_pos[3] , 32);
				if(MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx32))
				{
					result_pos = temp_pos[3];
					bescape = true;
					break;
				}
			}
		}
		if(bescape)
			break;

		widthcnt += (32/stepsize) * 2;
		heightcnt += (32/stepsize) * 2;
		loopcnt += 1;
	}

	pobj->SetPos(result_pos);
}
void CCom_Production_building::rallypoint_pathfinding(void)
{
	if(!m_rallypath.empty())
		m_rallypath.clear();

	int tempidx = m_pobj->Getcuridx(32);
	int preidx = tempidx;
	int igoalidx = CMyMath::Pos_to_index(m_rallypoint , 32);

	int icurstepcnt = 0;
	const int	istep = 9;
	D3DXVECTOR2 temppos;
	int	loopcnt = 0;

	int	oriidx = 0;
	BYTE byfloor = 0;

	short*	flowfieldpath = CTileManager::GetInstance()->Get_flowfield_node();

	while(true)
	{
		//일일히 담는것이아니라 n스텝당 한번씩 담자

		loopcnt = 0;

		temppos = CMyMath::index_to_Pos( flowfieldpath[tempidx]  , SQ_TILECNTX , SQ_TILESIZEX);

		if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(tempidx))
		{
			// 그지역이 장애물이라면
			temppos = CMyMath::index_to_Pos( preidx , SQ_TILECNTX , SQ_TILESIZEX);

			m_rallypath.push_back( temppos );

			break;
		}

		if(tempidx == igoalidx)
		{
			//최종지점 도착
			m_rallypath.push_back( m_rallypoint );
			break;
		}

		if( 0 != icurstepcnt &&
			0 == icurstepcnt % istep )
		{
			m_rallypath.push_back( temppos );
		}
		++icurstepcnt;

		preidx = tempidx;
		tempidx = flowfieldpath[tempidx]; //다음 경로로 가는 인덱스를 준다
	}
}
void CCom_Production_building::add_production_info(const float& maxtime , PRODUCTION_ID eid , const TCHAR* texkey)
{
	PRODUCTION_INFO temp;

	temp.maxtime = maxtime;
	temp.eid = eid;
	temp.texkey = texkey;

	if( m_production_list.size() < 5 )
	{
		m_production_list.push_back(temp);
	}
}
void CCom_Production_building::update_production(void)
{
	if(!m_production_list.empty())
	{
		m_pobj->SetState(PRODUCTION);
		float maxtime = m_production_list.front().maxtime;
		m_production_list.front().curtime += GETTIME;

		if(m_production_list.front().curtime >= maxtime)
		{
			//생산 
			create_unit(m_production_list.front().eid);
			m_production_list.pop_front();
			if(m_production_list.empty())
				m_pobj->SetState(IDLE);
		}

	}
}

void CCom_Production_building::Render(void)
{

}

void CCom_Production_building::Release(void)
{

}

void CCom_Production_building::create_unit(PRODUCTION_ID eid)
{
	CComponent* pcom = NULL;
	CObj* pobj = NULL;

	if(PRODUCTION_SCV == eid)
	{
		pobj = new CSCV(m_vPos);
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_SCV);
	}
	else if(PRODUCTION_TANK == eid)
	{
		pobj = new CTank;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_TANK);
	}
	else if(PRODUCTION_MARINE == eid)
	{
		pobj = new CMarine;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_MARINE);
	}
	else if(PRODUCTION_MEDIC == eid)
	{
		pobj = new CMedic;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_MEDIC);
	}
	else if(PRODUCTION_FIREBAT == eid)
	{
		pobj = new CFirebat;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_FIREBAT);
	}
	else if(PRODUCTION_GHOST == eid)
	{
		pobj = new CGhost;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_GHOST);
	}
	else if(PRODUCTION_TANK == eid)
	{
		pobj = new CTank;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_TANK);
	}
	else if(PRODUCTION_GOLIATH == eid)
	{
		pobj = new CGoliath;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_GOLIATH);
	}
	else if(PRODUCTION_VULTURE == eid)
	{
		pobj = new CVulture;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_VULTURE);
	}
	else if(PRODUCTION_WRAITH == eid)
	{
		pobj = new CWraith;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_WRAITH);
	}
	else if(PRODUCTION_DROPSHIP == eid)
	{
		pobj = new CDropship;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_DROPSHIP);
	}
	else if(PRODUCTION_VESSEL == eid)
	{
		pobj = new CVulture;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_VESSEL);
	}
	else if(PRODUCTION_BATTLE == eid)
	{
		pobj = new CBattleCruiser;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_BATTLE);
	}

	pobj->SetPos(m_vPos);
	pobj->Initialize();
	unit_collocate(pobj);

	if(true == m_is_rally)
	{
		pobj->SetOrder(ORDER_MOVE);
		pcom = pobj->GetComponent(COM_PATHFINDE);

		if(MOVE_GROUND == pobj->GetUnitinfo().eMoveType)
		{
			((CCom_Pathfind*)pcom)->SetGoalPos(m_rallypoint , false);
			((CCom_Pathfind*)pcom)->Setrally_path(m_rallypath);
		}
		else
			((CCom_AirPathfind*)pcom)->SetGoalPos(m_rallypoint , false);
	}
}

void CCom_Production_building::show_production_state()
{
	if(!m_production_list.empty())
	{
		D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();
		//CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(415,550) , m_production_list.front().curtime / m_production_list.front().maxtime );
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 283,interface_pos.y + 430) , m_production_list.front().curtime / m_production_list.front().maxtime );
		CIngame_UIMgr::GetInstance()->SetBuilding_info(m_production_list);
	}

}

void CCom_Production_building::set_rallypoint(const D3DXVECTOR2& vpos)
{
	m_rallypoint = vpos;
}

void CCom_Production_building::set_is_rally(bool isrally)
{
	m_is_rally = isrally;
}
