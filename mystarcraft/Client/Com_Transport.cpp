#include "StdAfx.h"
#include "Com_Transport.h"

#include "Ingame_UIMgr.h"
#include "Obj.h"

#include "TimeMgr.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "SoundDevice.h"

#include "Com_Pathfind.h"
CCom_Transport::CCom_Transport(const int& icapacity , const float& timeoffset)
{
	m_curcapacity = icapacity;
	m_timeoffset = timeoffset; //내리는 간격
}

CCom_Transport::~CCom_Transport(void)
{
}

void CCom_Transport::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_collocate_time = 0.5f;

	m_is_landing = false;
}

void CCom_Transport::Update(void)
{

	if(true == m_is_landing)
	{
		if(false == unit_landing())
		{
			m_is_landing = false;
		}
	}
}

void CCom_Transport::Render(void)
{

}

void CCom_Transport::Release(void)
{

}

bool CCom_Transport::setunit(CObj* pobj)
{
	BOARDING_INFO info;

	info.obj_address = pobj;

	OBJID eid = pobj->GetOBJNAME();
	if( OBJ_SCV == eid)
	{
		info.texkey = L"BOARDING_SCV";
	}
	else if(OBJ_MARINE == eid)
	{
		info.texkey = L"BOARDING_MARINE";
	}
	else if(OBJ_MEDIC == eid)
	{
		info.texkey = L"BOARDING_MEDIC";
	}
	else if(OBJ_FIREBAT == eid)
	{
		info.texkey = L"BOARDING_FIREBAT";
	}
	else if(OBJ_GHOST == eid)
	{
		info.texkey = L"BOARDING_GHOST";
	}
	else if(OBJ_VULTURE == eid)
	{
		info.irow = 2;
		info.texkey = L"BOARDING_VULTURE";
	}
	else if(OBJ_TANK == eid)
	{
		info.icol = 2;
		info.irow = 2;
		info.texkey = L"BOARDING_TANK";
	}
	else if(OBJ_GOLIATH == eid)
	{
		info.irow = 2;
		info.texkey = L"BOARDING_GOLIATH";
	}
	else if(OBJ_DRONE == eid)
	{
		info.texkey = L"BOARDING_DRONE";
	}
	else if(OBJ_ZERGLING == eid)
	{
		info.texkey = L"BOARDING_ZERGLING";
	}
	else if(OBJ_HYDRA == eid)
	{
		info.irow = 2;
		info.texkey = L"BOARDING_HYDRA";
	}
	else if(OBJ_LURKER == eid)
	{
		info.irow = 2;
		info.icol = 2;
		info.texkey = L"BOARDING_LURKER";
	}
	else if(OBJ_ULTRA == eid)
	{
		info.irow = 2;
		info.icol = 2;		
		info.texkey = L"BOARDING_ULTRA";
	}
	else if(OBJ_DEFILER == eid)
	{
		info.irow = 2;
		info.texkey = L"BOARDING_DEFILER";
	}
	else if(OBJ_BROODLING == eid)
	{
		info.texkey = L"BOARDING_BROODLING";
	}


	if( m_curcapacity >= info.icol * info.irow )
	{
		if(TRIBE_ZERG == m_pobj->GetUnitinfo().etribe)
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_Z_LOAD , m_pobj->GetPos());
		else if(TRIBE_TERRAN == m_pobj->GetUnitinfo().etribe)
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_T_LOAD , m_pobj->GetPos());

		m_curcapacity -= info.icol*info.irow;
		m_boarding_unit.insert(make_pair(info.icol*info.irow , info));
		return true;
	}
	else
		return false;
}
bool CCom_Transport::empty_boardinglist(void)
{
	return m_boarding_unit.empty();
}
void CCom_Transport::boarding_wire_ui(void)
{
	CIngame_UIMgr::GetInstance()->set_boarding_infolist(m_boarding_unit , m_pobj->GetOBJNAME());
}

bool CCom_Transport::unit_landing(void)
{
	if(m_boarding_unit.empty())
	{
		m_collocate_time = 0.5f;
		return false;
	}

	m_collocate_time += GETTIME;


	BOARDING_INFO temp;
	if(m_collocate_time >= m_timeoffset)
	{
		if(TRIBE_ZERG == m_pobj->GetUnitinfo().etribe)
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_Z_UNLOAD , m_pobj->GetPos());
		else if(TRIBE_TERRAN == m_pobj->GetUnitinfo().etribe)
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_T_UNLOAD , m_pobj->GetPos());

		CObj* pobj = NULL;
		m_collocate_time = 0.f;

		temp = m_boarding_unit.begin()->second;
		m_boarding_unit.erase(m_boarding_unit.begin());
		collocate_unit( (CObj*)(temp.obj_address) );
		m_curcapacity += temp.icol * temp.irow;

		//printf("%d \n" , m_curcapacity);
	}

	return true;
}

void CCom_Transport::collocate_unit(CObj* pobj)
{
	MYRECT<float>	collocate_rc;
	MYRECT<float>	vtx = pobj->GetVertex();

	int stepsize = 16;
	int loopcnt = 0;
	int widthcnt = 1;
	int heightcnt = 1;

	D3DXVECTOR2 vdrop_pos;
	vdrop_pos = m_pobj->GetPos();

	D3DXVECTOR2 collocate_pos[4];
	D3DXVECTOR2	temp_pos[4];
	D3DXVECTOR2	result_pos;

	bool bescape = false;

	int idx32 = 0;
	while(!bescape)
	{				
		collocate_pos[0].x = vdrop_pos.x - loopcnt*16; //밑줄 오른쪽방향
		collocate_pos[0].y = vdrop_pos.y + loopcnt*16;

		collocate_pos[1].x = vdrop_pos.x + loopcnt*16; //오른줄 위쪽방향
		collocate_pos[1].y = vdrop_pos.y + loopcnt*16;

		collocate_pos[2].x = vdrop_pos.x + loopcnt*16; //윗줄 왼쪽방향
		collocate_pos[2].y = vdrop_pos.y - loopcnt*16;

		collocate_pos[3].x = vdrop_pos.x - loopcnt*16; //왼줄 아래쪽방향
		collocate_pos[3].y = vdrop_pos.y - loopcnt*16;

		memcpy(temp_pos , collocate_pos , sizeof(D3DXVECTOR2)*4);

		for(int i = 0; i < widthcnt; ++i)
		{
			temp_pos[0].x = collocate_pos[0].x + i*stepsize;

			collocate_rc.left = temp_pos[0].x - vtx.left; 
			collocate_rc.right = temp_pos[0].x + vtx.right;
			collocate_rc.top = temp_pos[0].y - vtx.top;
			collocate_rc.bottom = temp_pos[0].y + vtx.bottom;

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , temp_pos[0] , collocate_rc ))
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

		for(int i = 0; i < heightcnt; ++i)
		{					
			temp_pos[1].y = collocate_pos[1].y - i*stepsize;;

			collocate_rc.left = temp_pos[1].x - vtx.left; 
			collocate_rc.right = temp_pos[1].x + vtx.right;
			collocate_rc.top = temp_pos[1].y - vtx.top;
			collocate_rc.bottom = temp_pos[1].y + vtx.bottom;

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , temp_pos[1] , collocate_rc ))
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

		for(int i = 0; i < widthcnt; ++i)
		{					
			temp_pos[2].x = collocate_pos[2].x - i*stepsize;;

			collocate_rc.left = temp_pos[2].x - vtx.left; 
			collocate_rc.right = temp_pos[2].x + vtx.right;
			collocate_rc.top = temp_pos[2].y - vtx.top;
			collocate_rc.bottom = temp_pos[2].y + vtx.bottom;

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , temp_pos[2] , collocate_rc ))
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

		for(int i = 0; i < heightcnt; ++i)
		{
			temp_pos[3].y = collocate_pos[3].y + i*stepsize;;

			collocate_rc.left = temp_pos[3].x - vtx.left; 
			collocate_rc.right = temp_pos[3].x + vtx.right;
			collocate_rc.top = temp_pos[3].y - vtx.top;
			collocate_rc.bottom = temp_pos[3].y + vtx.bottom;

			if(true == CArea_Mgr::GetInstance()->Collocate_check(pobj , temp_pos[3] , collocate_rc ))
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

		widthcnt += 2;
		heightcnt += 2;
		loopcnt += 1;
	}

	pobj->SetState(IDLE);
	pobj->SetOrder(ORDER_NONE);
	pobj->SetPos(result_pos);
	pobj->SetActive(true);
	pobj->unit_area_Initialize();

	//printf("배치완료 \n");
	CComponent* pcom = pobj->GetComponent(COM_PATHFINDE);

	if(NULL != pcom)
	{
		((CCom_Pathfind*)pcom)->ClearPath();
		((CCom_Pathfind*)pcom)->SetPathfindPause(true);
	}
}

void CCom_Transport::set_landing(bool is_landing)
{
	m_is_landing = is_landing;
}
