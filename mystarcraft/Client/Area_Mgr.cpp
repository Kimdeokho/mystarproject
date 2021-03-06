#include "StdAfx.h"
#include "Area_Mgr.h"
#include "Obj.h"
#include "Workman.h"

#include "MyMath.h"
#include "UnitMgr.h"
#include "MouseMgr.h"
#include "ScrollMgr.h"
#include "FontMgr.h"
#include "TileManager.h"
#include "Mineral.h"
#include "Ingame_UIMgr.h"
#include "Building_Preview.h"

#include "TimeMgr.h"
#include "Session_Mgr.h"
#include "SoundDevice.h"

#include "Com_Pathfind.h"
IMPLEMENT_SINGLETON(CArea_Mgr)


CArea_Mgr::CArea_Mgr(void)
{

	m_stepsize = 32;

	for(int i = 0; i < 64; ++i)
	{
		for(int j = 0; j < 64; ++j)
		{
			m_areapos64.push_back(D3DXVECTOR2((float)j*64.f , (float)i*64.f));
		}
	}

	for(int i = 0; i < 16; ++i)
	{
		for(int j = 0; j < 16; ++j)
		{
			m_areapos256.push_back(D3DXVECTOR2((float)j*256.f , (float)i*256.f));
		}
	}

	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			m_areapos512.push_back(D3DXVECTOR2((float)j*512.f , (float)i*512.f));
		}
	}

	m_pchoice_target = NULL;

	m_vecmoveobj.reserve(400);
}

CArea_Mgr::~CArea_Mgr(void)
{
	for(int i = 0; i < 64*64; ++i)
	{
		if(!m_Area64[i].empty())
			m_Area64[i].clear();
	}

	list<CObj*>::iterator iter = m_obstacle_list.begin();
	list<CObj*>::iterator iter_end = m_obstacle_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(*iter);
	m_obstacle_list.clear();
}
CObj* CArea_Mgr::Melee_explore_target(CObj* pself , const int isearch_range , TARGET_SEARCH_TYPE esearchtype , const int path_size)
{
	if(NULL == pself)
		return NULL;

	D3DXVECTOR2 vstart_pos = pself->GetPos();
	D3DXVECTOR2 vpos = pself->GetPos();
	int stepsize = 64;
	int tilecnt = (isearch_range + stepsize)/ stepsize;

	int loopcnt = 2;
	CObj* ptarget = NULL;
	float fminvalue = 4096*4096;

	vector<CObj*> attack_list;
	attack_list.reserve(10);
	int listsize = 0;

	int path_distance = path_size * 16 * 16;

	MeleeTarget_extract(pself , vpos , fminvalue , ptarget, isearch_range , esearchtype , path_distance);

	for(int i = 1; i <= tilecnt; ++i)
	{
		vpos.x = vstart_pos.x - i * 64;
		vpos.y = vstart_pos.y - i * 64;

		//시계방향 탐색
		for(int j = 0; j < loopcnt; ++j)
		{
			//오른쪽 전진
			vpos.x += 64;
			MeleeTarget_extract(pself , vpos , fminvalue , ptarget, isearch_range , esearchtype , path_distance);
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//아래 전진
			vpos.y += 64;
			MeleeTarget_extract(pself , vpos , fminvalue , ptarget , isearch_range , esearchtype , path_distance);
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//왼쪽 전진
			vpos.x -= 64;
			MeleeTarget_extract(pself , vpos , fminvalue , ptarget , isearch_range , esearchtype , path_distance);
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//위쪽 전진
			vpos.y -= 64;
			MeleeTarget_extract(pself , vpos , fminvalue , ptarget , isearch_range , esearchtype , path_distance);
		}

		if(!attack_list.empty())
		{
			listsize = attack_list.size();
			ptarget = attack_list[listsize-1];

			vector<CObj*> temp;
			temp.swap(attack_list);

			return ptarget;
		}

		loopcnt += 2;
	}

	return ptarget;
}
CObj* CArea_Mgr::Auto_explore_target(CObj* pself , const int isearch_range ,const int att_range, TARGET_SEARCH_TYPE esearchtype)
{
	if(NULL == pself)
		return NULL;

	D3DXVECTOR2 vstart_pos = pself->GetPos();
	D3DXVECTOR2 vpos = pself->GetPos();
	int stepsize = 64;
	int tilecnt = (isearch_range + stepsize)/ stepsize;

	int loopcnt = 2;
	CObj* ptarget = NULL;
	vector<CObj*> attack_list;
	attack_list.reserve(10);
	int listsize = 0;
	float fminvalue = 4096*4096;

	if(RangeTarget_extract(pself , vpos ,fminvalue, attack_list, isearch_range ,att_range, esearchtype))
		return ptarget;


	for(int i = 1; i <= tilecnt; ++i)
	{
		vpos.x = vstart_pos.x - i * 64;
		vpos.y = vstart_pos.y - i * 64;

		//시계방향 탐색
		for(int j = 0; j < loopcnt; ++j)
		{
			//오른쪽 전진
			vpos.x += 64;
			if(RangeTarget_extract(pself , vpos ,fminvalue, attack_list,isearch_range ,att_range, esearchtype))
				return ptarget;
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//아래 전진
			vpos.y += 64;
			if(RangeTarget_extract(pself , vpos ,fminvalue, attack_list, isearch_range ,att_range, esearchtype))
				return ptarget;
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//왼쪽 전진
			vpos.x -= 64;
			if(RangeTarget_extract(pself , vpos ,fminvalue, attack_list, isearch_range ,att_range, esearchtype))
				return ptarget;
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//위쪽 전진
			vpos.y -= 64;
			if(RangeTarget_extract(pself , vpos ,fminvalue, attack_list, isearch_range ,att_range, esearchtype))
				return ptarget;
		}

		if(!attack_list.empty())
		{
			listsize = attack_list.size();

			ptarget = attack_list[rand()%listsize];

			vector<CObj*> temp;
			temp.swap(attack_list);

			return ptarget;
		}

		if(NULL != ptarget)
			return ptarget;

		loopcnt += 2;
	}

	return ptarget;
}
CObj* CArea_Mgr::Explore_medic_target(CObj* pself , const int& isearch_range )
{
	D3DXVECTOR2 vstart_pos = pself->GetPos();
	D3DXVECTOR2 vpos = pself->GetPos();
	int stepsize = 64;
	int tilecnt = (isearch_range + stepsize)/ stepsize;

	int loopcnt = 2;
	CObj* ptarget = NULL;
	float fminvalue = 4096*4096;

	MedicTarget_extract(pself , vpos ,fminvalue , ptarget, isearch_range );

	if(NULL != ptarget)
		return ptarget;

	for(int i = 1; i <= tilecnt; ++i)
	{
		vpos.x = vstart_pos.x - i * 64;
		vpos.y = vstart_pos.y - i * 64;

		//시계방향 탐색
		for(int j = 0; j < loopcnt; ++j)
		{
			//오른쪽 전진
			vpos.x += 64;
			MedicTarget_extract(pself , vpos ,fminvalue , ptarget, isearch_range );

		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//아래 전진
			vpos.y += 64;
			MedicTarget_extract(pself , vpos ,fminvalue , ptarget, isearch_range );
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//왼쪽 전진
			vpos.x -= 64;
			MedicTarget_extract(pself , vpos ,fminvalue , ptarget, isearch_range );
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//위쪽 전진
			vpos.y -= 64;
			MedicTarget_extract(pself , vpos ,fminvalue , ptarget, isearch_range );
		}

		loopcnt += 2;

		if(NULL != ptarget)
			return ptarget;
	}

	return ptarget;
}
bool CArea_Mgr::Search_Mineral(const int& idx , const int& isearch_range ,CObj* pself, CObj*& pmineral)
{
	D3DXVECTOR2 vstart_pos = pself->GetPos();
	D3DXVECTOR2 vpos = pself->GetPos();
	int stepsize = 64;
	int tilecnt = (isearch_range + stepsize)/ stepsize;

	int loopcnt = 2;

	if(Mineral_extractor(vpos , pself , pmineral))
		return true;

	for(int i = 1; i <= tilecnt; ++i)
	{
		vpos.x = vstart_pos.x - i * 64;
		vpos.y = vstart_pos.y - i * 64;

		//시계방향 탐색
		for(int j = 0; j < loopcnt; ++j)
		{
			//오른쪽 전진
			vpos.x += 64;
			if(Mineral_extractor(vpos , pself , pmineral))
				return true;
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//아래 전진
			vpos.y += 64;
			if(Mineral_extractor(vpos , pself , pmineral))
				return true;
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//왼쪽 전진
			vpos.x -= 64;
			if(Mineral_extractor(vpos , pself , pmineral))
				return true;
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//위쪽 전진
			vpos.y -= 64;
			if(Mineral_extractor(vpos , pself , pmineral))
				return true;
		}

		loopcnt += 2;
	}

	return false;
}
bool CArea_Mgr::Mineral_extractor(const D3DXVECTOR2& vpos ,CObj* pself, CObj*& pmineral)
{
	if(vpos.x <= 0 || vpos.x >= SQ_TILECNTX*32 ||
		vpos.y <= 0 || vpos.y >= SQ_TILECNTX*32)
		return false;

	float idistance = 0;
	int fogidx = 0;

	int idx = CMyMath::Pos_to_index(vpos , 64);

	if(!m_Area64[ idx ].empty())
	{
		list<CObj*>::iterator iter = m_Area64[idx ].begin();
		list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

		D3DXVECTOR2	vtargetpos;

		for( ; iter != iter_end; ++iter)
		{

			if(pself == (*iter))
				continue;

			if( OBJ_MINERAL != (*iter)->GetOBJNAME())
				continue;

			vtargetpos = (*iter)->GetPos();

			if( ((CMineral*)(*iter))->Getworkman_count() == 0)
			{
				pmineral = (*iter);
				return true;	
			}
		}
	}
	return false;
}
bool CArea_Mgr::Mineral_extractor(const int& idx ,CObj* pself, CObj*& pmineral)
{
	if(!m_Area64[ idx ].empty())
	{
		float idistance = 0;
		int fogidx = 0;

		list<CObj*>::iterator iter = m_Area64[idx ].begin();
		list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

		D3DXVECTOR2	vtargetpos;

		for( ; iter != iter_end; ++iter)
		{

			if(pself == (*iter))
				continue;

			if( CATEGORY_RESOURCE != (*iter)->GetCategory())
				continue;



			vtargetpos = (*iter)->GetPos();
			fogidx = CMyMath::Pos_to_index( vtargetpos , 32);

			if(OBJ_MINERAL == pmineral->GetOBJNAME())
			{
				if( ((CMineral*)(*iter))->Getworkman_count() >= 2)
					continue;

				pmineral = (*iter);
				return true;	
			}
		}
	}
	return false;
}
void CArea_Mgr::gas_extractor(CObj* pself , const D3DXVECTOR2& vpt)
{
	//int idx64 = pself->Getcuridx(64);

	CMyMath::Calculator_eight_idx(m_idxdir , pself->GetPos() , 64);


	CObj* pgas = NULL;
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(!m_Area64[ m_idxdir[i] ].empty())
		{
			float idistance = 0;
			int fogidx = 0;

			list<CObj*>::iterator iter = m_Area64[m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

			D3DXVECTOR2	vtargetpos;

			for( ; iter != iter_end; ++iter)
			{
				if(pself == (*iter))
					continue;

				if( CATEGORY_RESOURCE != (*iter)->GetCategory())
					continue;


				if( OBJ_GAS != (*iter)->GetOBJNAME() )
					continue;

				if(MyPtInrect( vpt , &((*iter)->GetMyRect()) ))
				{
					pgas = (*iter);
					pgas->SetActive(false);
				}
			}
		}
	}
}
bool CArea_Mgr::RangeTarget_extract(CObj* pself ,const D3DXVECTOR2& vpos ,float& fminvalue , vector<CObj*>& attlist,  const int isearch_range ,const int att_range , TARGET_SEARCH_TYPE esearchtype)
{
	if(vpos.x <= 0 || vpos.x >= SQ_TILECNTX*32 ||
		vpos.y <= 0 || vpos.y >= SQ_TILECNTX*32)
		return false;


	//가장 가까운 적을 찾기로 하자.

	float idistance = 0;
	int fogidx = 0;

	int idx = CMyMath::Pos_to_index(vpos , 64);
	list<CObj*>::iterator iter = m_Area64[idx ].begin();
	list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

	ATTACK_SEARCH_TYPE eself_attacktype = pself->GetUnitinfo().eAttackType;
	MOVE_TYPE etarget_movetype;
	D3DXVECTOR2	vtargetpos;

	TEAM_NUMBER eteam = pself->GetTeamNumber();

	for( ; iter != iter_end; ++iter)
	{

		if( CATEGORY_RESOURCE == (*iter)->GetCategory())
			continue;
		if( OBJ_NONE == (*iter)->GetOBJNAME())
			continue;

		if(pself == (*iter))
			continue;

		//공중만 공격 가능한 유닛은 공중만 검사하고
		//지상만 공격 가능한 유닛은 지상만 검사한다		

		if( SEARCH_ONLY_ENEMY == esearchtype)
		{
			if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
				continue;

			etarget_movetype = (*iter)->GetUnitinfo().eMoveType;

			if( ATTACK_ONLY_AIR == eself_attacktype)
			{
				if( MOVE_GROUND ==  etarget_movetype)
					continue;
			}
			else if( ATTACK_ONLY_GROUND == eself_attacktype )
			{
				if( MOVE_AIR ==  etarget_movetype)
					continue;
			}
		}
		else if(SEARCH_ONLY_ALLY == esearchtype)
		{
			if(pself->GetTeamNumber() != (*iter)->GetTeamNumber())
				continue;
		}

		if((*iter)->GetUnitinfo().detect[eteam] < 1)
			continue;

		vtargetpos = (*iter)->GetPos();
		fogidx = CMyMath::Pos_to_index( vtargetpos , 32);
		if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(fogidx , pself->GetTeamNumber()))
			continue;

		idistance = CMyMath::pos_distance(pself->GetPos() , vtargetpos);

		//지금 찾은 유닛과의 거리가 최소거리보다 작다면

		if(idistance <= isearch_range * isearch_range)
		{
			attlist.push_back((*iter));	
		}
	}

	return false;
}
void CArea_Mgr::MeleeTarget_extract(CObj* pself ,const D3DXVECTOR2& vpos, float& fminvalue, CObj*& ptarget,  const int isearch_range ,TARGET_SEARCH_TYPE esearchtype,const int path_distance)
{
	if(vpos.x <= 0 || vpos.x >= SQ_TILECNTX*32 ||
		vpos.y <= 0 || vpos.y >= SQ_TILECNTX*32)
		return;


	//가장 가까운 적을 찾기로 하자.

	float idistance = 0;
	int fogidx = 0;

	int idx = CMyMath::Pos_to_index(vpos , 64);
	list<CObj*>::iterator iter = m_Area64[idx ].begin();
	list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

	ATTACK_SEARCH_TYPE eself_attacktype = pself->GetUnitinfo().eAttackType;
	MOVE_TYPE etarget_movetype;
	D3DXVECTOR2	vtargetpos;

	TEAM_NUMBER eteam = pself->GetTeamNumber();

	for( ; iter != iter_end; ++iter)
	{

		if( CATEGORY_RESOURCE == (*iter)->GetCategory())
			continue;

		if(pself == (*iter))
			continue;

		//공중만 공격 가능한 유닛은 공중만 검사하고
		//지상만 공격 가능한 유닛은 지상만 검사한다		

		if( SEARCH_ONLY_ENEMY == esearchtype)
		{
			if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
				continue;

			etarget_movetype = (*iter)->GetUnitinfo().eMoveType;

			if( ATTACK_ONLY_AIR == eself_attacktype)
			{
				if( MOVE_GROUND ==  etarget_movetype)
					continue;
			}
			else if( ATTACK_ONLY_GROUND == eself_attacktype )
			{
				if( MOVE_AIR ==  etarget_movetype)
					continue;
			}
		}
		else if(SEARCH_ONLY_ALLY == esearchtype)
		{
			if(pself->GetTeamNumber() != (*iter)->GetTeamNumber())
				continue;
		}

		if((*iter)->GetUnitinfo().detect[eteam] < 1)
			continue;

		vtargetpos = (*iter)->GetPos();
		fogidx = CMyMath::Pos_to_index( vtargetpos , 32);
		if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(fogidx , pself->GetTeamNumber()))
			continue;

		idistance = CMyMath::pos_distance(pself->GetPos() , vtargetpos);

		//지금 찾은 유닛과의 거리가 최소거리보다 작다면

		if(idistance <= isearch_range * isearch_range)
		{
			//해당 유닛의 탐지범위에 적합한지 본다.
			//목표를 찾았다.
			
			if(0 != path_distance)
			{
				if( idistance < path_distance && idistance < fminvalue )
				{
					fminvalue = idistance;
					ptarget = (*iter);
				}
			}
			else
			{
				if( idistance < fminvalue )
				{
					fminvalue = idistance;						
					ptarget = (*iter);
				}
			}
		}
	}
}
void CArea_Mgr::MedicTarget_extract(CObj* pself ,const D3DXVECTOR2& vpos, float& fminvalue, CObj*& ptarget,  const int isearch_range)
{
	if(vpos.x <= 0 || vpos.x >= SQ_TILECNTX*32 ||
		vpos.y <= 0 || vpos.y >= SQ_TILECNTX*32)
		return;

	//가장 가까운 적을 찾기로 하자.

	float idistance = 0;
	int fogidx = 0;

	int idx = CMyMath::Pos_to_index(vpos , 64);
	list<CObj*>::iterator iter = m_Area64[idx ].begin();
	list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

	ATTACK_SEARCH_TYPE eself_attacktype = pself->GetUnitinfo().eAttackType;
	D3DXVECTOR2	vtargetpos;

	for( ; iter != iter_end; ++iter)
	{

		if( CATEGORY_RESOURCE == (*iter)->GetCategory())
			continue;

		if(pself == (*iter))
			continue;

		if(pself->GetTeamNumber() != (*iter)->GetTeamNumber())
			continue;
		
		if( OBJRACE_CREATURE == (*iter)->GetUnitinfo().erace)
		{
			vtargetpos = (*iter)->GetPos();
			idistance = CMyMath::pos_distance(pself->GetPos() , vtargetpos);

			//지금 찾은 유닛과의 거리가 최소거리보다 작다면

			if(idistance <= isearch_range * isearch_range)
			{
				if((*iter)->GetUnitinfo().maxhp > (*iter)->GetUnitinfo().hp)
				{
					if(fminvalue > idistance)
					{
						fminvalue = idistance;						
						ptarget = (*iter);
					}	
				}
			}
		}
	}
}
void CArea_Mgr::DragCheck( const MYRECT<float>& rc )
{
	//rc == 드래그 사각형

	D3DXVECTOR2 vstartpt = D3DXVECTOR2(rc.left - 64 , rc.top - 64);
	D3DXVECTOR2 vendpt = D3DXVECTOR2(rc.right + 64 , rc.bottom + 64);

	if(vendpt.x >= 4096)
		vendpt.x = 4095;
	else if(vendpt.x <= 0)
		vendpt.x = 1;

	if(vendpt.y >= 4096)
		vendpt.y = 4095;
	else if(vendpt.y <= 0)
		vendpt.y = 1;

	if(vstartpt.x >= 4096)
		vstartpt.x = 4095;
	else if(vstartpt.x <= 0)
		vstartpt.x = 1;

	if(vstartpt.y >= 4096)
		vstartpt.y = 4095;
	else if(vstartpt.y <= 0)
		vstartpt.y = 1;

	int startidx = CMyMath::Pos_to_index(vstartpt , 64);
	int endidx = CMyMath::Pos_to_index(vendpt , 64);


	int colcnt = (endidx/64 - startidx/64) + 1;
	int rowcnt = (endidx%64 - startidx%64) + 1;
	int curidx = 0;


	bool escape = false;
	for(int i = 0; i < colcnt; ++i)
	{
		for(int j = 0; j < rowcnt; ++j)
		{
			curidx = 64 * i + startidx  + j;

			//유닛이 선택된상태에서 허공에 드래그했을경우
			//SELECT_NONE이 되는걸 방지한다.
			if(true == DummyCheck( curidx , rc))
			{
				CUnitMgr::GetInstance()->discharge_unit();
				escape = true;
				break;
			}
		}
		if( true == escape)
			break;
	}

	for(int i = 0; i < colcnt; ++i)
	{
		for(int j = 0; j < rowcnt; ++j)
		{
			curidx = 64 * i + startidx  + j;

			if(!Choice_unit( curidx , rc))
				return;
		}
	}

	for(int i = 0; i < colcnt; ++i)
	{
		for(int j = 0; j < rowcnt; ++j)
		{
			curidx = 64 * i + startidx  + j;

			Choice_building( curidx , rc);
		}
	}

	if(!CUnitMgr::GetInstance()->Getcur_unitlist()->empty() && escape)
	{
		OBJID id = CUnitMgr::GetInstance()->Getcur_unitlist()->front()->GetOBJNAME();
		CSoundDevice::GetInstance()->SetUnitVoice(id);
	}
}
void CArea_Mgr::SelectCheck(const D3DXVECTOR2& vpos)
{
	//드래그가 아닌 기본 클릭 선택에대한 함수
	CMyMath::Calculator_eight_idx(m_idxdir , vpos , 64 );

	bool escape = false;
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;
		
		if(true == DummyCheck(m_idxdir[i] , vpos))
		{
			//선택된 유닛들이 있는상태에서 허공을 클릭했을때
			//선택해제가 되는걸 방지한다.
			//다른유닛이 선택되었을경우 기존에 선택되어있던 유닛을 선택 해제시킨다
			CUnitMgr::GetInstance()->discharge_unit();
			escape = true;
			break;
		}
	}


	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if(true == Choice_unit(m_idxdir[i] , vpos))
			return;
	}

	if(!CUnitMgr::GetInstance()->Getcur_unitlist()->empty() && escape)
	{
		OBJID id = CUnitMgr::GetInstance()->Getcur_unitlist()->front()->GetOBJNAME();
		CSoundDevice::GetInstance()->SetUnitVoice(id);
	}
}

CObj* CArea_Mgr::Collision_check(CObj* pself)
{
	//주위 충돌검사
	CMyMath::Calculator_eight_idx(m_idxdir , pself->GetPos() , 64 );

	MOVE_TYPE selfmovetype = pself->GetUnitinfo().eMoveType;
	MOVE_TYPE targetmovetype;

	ORDER eself_order = pself->GetUnitinfo().order;
	ORDER other_order = ORDER_NONE;

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if( !m_Area64[m_idxdir[i]].empty() )
		{
			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

			for( ; iter != iter_end; ++iter)
			{
				if((*iter) == pself)
					continue;

				other_order = (*iter)->GetUnitinfo().order;

				if(ORDER_GATHER == eself_order ||
					ORDER_RETURN_CARGO == eself_order)
				{
					//자원채취중일땐 자원과 건물만 충돌한다
					if( CATEGORY_RESOURCE != (*iter)->GetCategory() &&
						CATEGORY_BUILDING != (*iter)->GetCategory())
						continue;
				}

				if(ORDER_GATHER == other_order ||
					ORDER_RETURN_CARGO == other_order )
					continue;

				targetmovetype = (*iter)->GetUnitinfo().eMoveType;

				if( MOVE_GROUND == selfmovetype)
				{
					if(MOVE_AIR == targetmovetype)
						continue;
				}
				else if(MOVE_AIR == selfmovetype)
				{
					if(MOVE_GROUND == targetmovetype)
						continue;
				}

				if(ORDER_BUILDING == eself_order) //scv가 건물을 짓고있을때
					continue;
				if(BURROW == (*iter)->GetUnitinfo().state)
					continue;

				if(MyIntersectrect(&pself->GetMyRect() , &(*iter)->GetMyRect() ))
				{
					return *iter;
				}
			}
		}
	}

	return NULL;
}
bool CArea_Mgr::Collision_check(CObj* pself , CObj*& ptarget ,const MYRECT<float>& myrc)
{

	float idistance = 0;
	int fogidx = 0;

	ATTACK_SEARCH_TYPE eself_attacktype = pself->GetUnitinfo().eAttackType;
	MOVE_TYPE etarget_movetype;
	TARGET_SEARCH_TYPE esearchtype = SEARCH_ONLY_ENEMY;

	TEAM_NUMBER eteam = pself->GetTeamNumber();

	CMyMath::Calculator_eight_idx(m_idxdir , pself->GetPos() , 64 );

	MOVE_TYPE selfmovetype = pself->GetUnitinfo().eMoveType;


	D3DXVECTOR2 vtargetpos = D3DXVECTOR2(0,0);

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if( !m_Area64[m_idxdir[i]].empty() )
		{
			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

			for( ; iter != iter_end; ++iter)
			{
				if( CATEGORY_RESOURCE == (*iter)->GetCategory())
					continue;

				if(pself == (*iter))
					continue;

				//공중만 공격 가능한 유닛은 공중만 검사하고
				//지상만 공격 가능한 유닛은 지상만 검사한다		

				if( SEARCH_ONLY_ENEMY == esearchtype)
				{
					if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
						continue;

					etarget_movetype = (*iter)->GetUnitinfo().eMoveType;

					if( ATTACK_ONLY_AIR == eself_attacktype)
					{
						if( MOVE_GROUND ==  etarget_movetype)
							continue;
					}
					else if( ATTACK_ONLY_GROUND == eself_attacktype )
					{
						if( MOVE_AIR ==  etarget_movetype)
							continue;
					}
				}

				if((*iter)->GetUnitinfo().detect[eteam] < 1)
					continue;

				vtargetpos = (*iter)->GetPos();
				fogidx = CMyMath::Pos_to_index( vtargetpos , 32);
				if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(fogidx , pself->GetTeamNumber()))
					continue;

				if(MyIntersectrect(&myrc , &(*iter)->GetMyRect()))
				{
					ptarget = (*iter);
					return true;
				}
			}
		}
	}

	return false;
}
bool CArea_Mgr::Building_Collocate_check(CObj* pself , const D3DXVECTOR2& vpos ,MYRECT<float>& myrc )
{
	CMyMath::Calculator_eight_idx(m_idxdir , vpos , 64);

	MOVE_TYPE othermovetype;

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if( !m_Area64[m_idxdir[i]].empty() )
		{
			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

			for( ; iter != iter_end; ++iter)
			{
				if((*iter) == pself)
					continue;				

				othermovetype = (*iter)->GetUnitinfo().eMoveType;

				if(MOVE_AIR == othermovetype)
					continue;

				if(MyIntersectrect(&myrc , &(*iter)->GetMyRect() ))
					return false;
			}
		}
	}
	return true;
}
bool CArea_Mgr::Collocate_check(CObj* pself , const D3DXVECTOR2& vpos ,MYRECT<float>& myrc )
{
	//주위 충돌검사
	CMyMath::Calculator_eight_idx(m_idxdir , vpos , 64);

	MOVE_TYPE selfmovetype = pself->GetUnitinfo().eMoveType;
	MOVE_TYPE othermovetype;

	float	fwidth = 0;
	float	fheight = 0;
	bool	bescape = false;
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if( !m_Area64[m_idxdir[i]].empty() )
		{
			MYRECT<float> temp;
			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

			for( ; iter != iter_end; ++iter)
			{
				if((*iter) == pself)
					continue;				

				othermovetype = (*iter)->GetUnitinfo().eMoveType;

				if( MOVE_GROUND == selfmovetype)
				{
					if(MOVE_AIR == othermovetype)
						continue;
				}
				else if(MOVE_AIR == selfmovetype)
				{
					if(MOVE_GROUND == othermovetype)
						continue;
				}

				if(MyIntersectrect( &temp , &myrc , &(*iter)->GetMyRect() ))
				{
					return false;
				}
			}
		}
	}
	return true;
}
bool CArea_Mgr::MeleeAttack_Search(CObj* pself ,CObj*& ptarget, const MYRECT<float>& myrc ,TARGET_SEARCH_TYPE esearchtype)
{
	CMyMath::Calculator_eight_idx(m_idxdir , pself->GetPos() , 64);

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if( !m_Area64[m_idxdir[i]].empty() )
		{
			MYRECT<float> temp;
			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

			for( ; iter != iter_end; ++iter)
			{
				if( CATEGORY_RESOURCE == (*iter)->GetCategory())
					continue;

				if( SEARCH_ONLY_ENEMY == esearchtype)
				{
					if( pself->GetTeamNumber() == (*iter)->GetTeamNumber())
						continue;

					if( ATTACK_ONLY_AIR == pself->GetUnitinfo().eAttackType)
					{
						if(MOVE_GROUND == (*iter)->GetUnitinfo().eMoveType)
							continue;
					}
					else if( ATTACK_ONLY_GROUND == pself->GetUnitinfo().eAttackType)
					{
						if(MOVE_AIR == (*iter)->GetUnitinfo().eMoveType)
							continue;
					}
				}
				else if( SEARCH_ONLY_ALLY == esearchtype )
				{
					if( pself->GetTeamNumber() != (*iter)->GetTeamNumber())
						continue;

					if( ATTACK_ONLY_AIR == pself->GetUnitinfo().eAttackType)
					{
						if(MOVE_GROUND == (*iter)->GetUnitinfo().eMoveType)
							continue;
					}
					else if( ATTACK_ONLY_GROUND == pself->GetUnitinfo().eAttackType)
					{
						if(MOVE_AIR == (*iter)->GetUnitinfo().eMoveType)
							continue;
					}
				}

				if((*iter) != pself)
				{
					if(MyIntersectrect( &temp , &myrc , &(*iter)->GetMyRect() ))
					{
						ptarget = *iter;
						return true;
					}
				}

			}
		}
	}
	return false;
}
bool CArea_Mgr::Choice_Same_unit(const D3DXVECTOR2& vpos)
{
	//드래그가 아닌 기본 클릭 선택에대한 함수
	CMyMath::Calculator_eight_idx(m_idxdir , vpos , 64 );

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if(true == DummyCheck(m_idxdir[i] , vpos))
		{
			//선택된 유닛들이 있는상태에서 허공을 클릭했을때
			//선택해제가 되는걸 방지한다.
			//다른유닛이 선택되었을경우 기존에 선택되어있던 유닛을 선택 해제시킨다
			CUnitMgr::GetInstance()->discharge_unit();
			break;
		}
	}


	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
	int idx = 0;

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		idx = m_idxdir[i];
		if(idx < 0)
			continue;

		if( !m_Area64[idx].empty() )
		{
			list<CObj*>::iterator iter = m_Area64[idx].begin();
			list<CObj*>::iterator iter_end = m_Area64[idx].end();
			
			int idx = 0;

			for( ; iter != iter_end; ++iter)
			{
				if(OBJ_NONE == (*iter)->GetOBJNAME())
					continue;
				if( (*iter)->GetUnitinfo().detect[eteam] < 1)
					continue;

				idx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
				if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(idx , eteam))
					continue;

				if(MyPtInrect( vpos , &(*iter)->GetMyRect() ))
				{
					CUnitMgr::GetInstance()->discharge_unit();

					if( TEAM_NONE == (*iter)->GetTeamNumber())
						(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,255,255));
					else if( eteam != (*iter)->GetTeamNumber())
						(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,0,0));
					else
						(*iter)->SetSelect(GENERAL_SELECT);

					CUnitMgr::GetInstance()->SetUnit((*iter));

					if(eteam != (*iter)->GetTeamNumber())
						return true;
					if( CATEGORY_UNIT != (*iter)->GetCategory() )
						return true;

					const CObj* const pself = (*iter);
					OBJID eobjid = (*iter)->GetOBJNAME();
					D3DXVECTOR2 vpos = D3DXVECTOR2(CScrollMgr::m_fScrollX, CScrollMgr::m_fScrollY);
					D3DXVECTOR2 temppos = D3DXVECTOR2(0,0);

					for(int i = 0; i < BACKBUFFER_SIZEY; i += 64)
					{
						for(int j = 0; j < BACKBUFFER_SIZEX; j += 64)
						{
							temppos.x = vpos.x + j;
							temppos.y = vpos.y + i;

							int tempidx = CMyMath::Pos_to_index(temppos , 64);

							if(m_Area64[tempidx].empty())
								continue;

							iter = m_Area64[tempidx].begin();
							iter_end = m_Area64[tempidx].end();

							for( ; iter != iter_end; ++iter)
							{
								if(pself == (*iter))
									continue;
								if(eteam != (*iter)->GetTeamNumber())
									continue;
								if(eobjid == (*iter)->GetOBJNAME())
								{
									(*iter)->SetSelect(GENERAL_SELECT);
									CUnitMgr::GetInstance()->SetUnit((*iter));
								}
							}
						}
					}

					return true;
					/*아군만 선택*/
				}
			}
		}

	}

	return false;
}
bool CArea_Mgr::Choice_unit(const int& idx , const D3DXVECTOR2& vpos)
{

	//한마리만 선택할때
	if( !m_Area64[idx].empty() )
	{
		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
		int idx = 0;

		for( ; iter != iter_end; ++iter)
		{
			if( (*iter)->GetUnitinfo().detect[eteam] < 1)
				continue;
			if(OBJ_NONE == (*iter)->GetOBJNAME())
				continue;

			idx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
			if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(idx , eteam))
				continue;

			if(MyPtInrect( vpos , &(*iter)->GetMyRect() ))
			{
				CUnitMgr::GetInstance()->discharge_unit();

				if( TEAM_NONE == (*iter)->GetTeamNumber())
					(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,255,255));
				else if( eteam != 
					(*iter)->GetTeamNumber())
					(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,0,0));
				else
					(*iter)->SetSelect(GENERAL_SELECT);

				CUnitMgr::GetInstance()->SetUnit((*iter));

				return true;
				/*아군만 선택*/
			}
		}
	}

	return false;
}
bool CArea_Mgr::Choice_unit(const int& idx, const MYRECT<float>& rc )
{
	//다수 유닛 선택할때

	if( !m_Area64[idx].empty() )
	{
		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		CUI* pui = NULL;

		TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
		int idx = 0;

		for( ; iter != iter_end; ++iter)
		{
			if(OBJ_NONE == (*iter)->GetOBJNAME())
				continue;
			if(CATEGORY_NONE == (*iter)->GetCategory())
				continue;
			if( (*iter)->GetUnitinfo().detect[eteam] < 1)
				continue;

			idx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
			if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(idx , eteam))
				continue;

			if(MyIntersectrect(&(*iter)->GetMyRect() , &rc ) ||
				MyPtInrect((*iter)->GetPos() , &rc ))
			{
				if(CATEGORY_UNIT == (*iter)->GetCategory())
				{
					 if(OBJ_MINE == (*iter)->GetOBJNAME())
						 continue;

					if((*iter)->GetTeamNumber() == eteam)
					{
						CUnitMgr::GetInstance()->SetUnit((*iter));
						(*iter)->SetSelect(GENERAL_SELECT);
					}
				}
				/*아군만 선택*/
			}
		}
	}

	return true;
}
void CArea_Mgr::Choice_building(const int idx, const MYRECT<float>& rc)
{
	if( !m_Area64[idx].empty() )
	{

		if(CUnitMgr::GetInstance()->GetUnitlistempty())
		{
			list<CObj*>::iterator iter = m_Area64[idx].begin();
			list<CObj*>::iterator iter_end = m_Area64[idx].end();

			TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

			iter = m_Area64[idx].begin();
			iter_end = m_Area64[idx].end();

			int fogidx = 0;
			for( ; iter != iter_end; ++iter)
			{
				if(OBJ_NONE == (*iter)->GetOBJNAME())
					continue;
				if(CATEGORY_NONE == (*iter)->GetCategory())
					continue;

				fogidx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
				if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(fogidx , eteam))
					continue;

				if(MyIntersectrect(&(*iter)->GetMyRect() , &rc ))
				{
					if(CATEGORY_BUILDING == (*iter)->GetCategory())
					{
						CUnitMgr::GetInstance()->SetUnit((*iter));

						if((*iter)->GetTeamNumber() != CSession_Mgr::GetInstance()->GetTeamNumber())
							(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,0,0));
						else
							(*iter)->SetSelect(GENERAL_SELECT);

						return;
					}
					else
					{
						if(OBJ_MINE == (*iter)->GetOBJNAME())
						{
							CUnitMgr::GetInstance()->SetUnit((*iter));

							if((*iter)->GetTeamNumber() != CSession_Mgr::GetInstance()->GetTeamNumber())
								(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,0,0));
							else
								(*iter)->SetSelect(GENERAL_SELECT);

							return;
						}
						else
						{
							if((*iter)->GetTeamNumber() != CSession_Mgr::GetInstance()->GetTeamNumber())
							{
								CUnitMgr::GetInstance()->SetUnit((*iter));
								(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,0,0));
							}

							return;
						}
					}
				}
			}
		}
	}
}
void CArea_Mgr::TargetChoice(const D3DXVECTOR2& vpos)
{

	m_pchoice_target = NULL;
	int idx = 0;
	CMyMath::Calculator_eight_idx(m_idxdir , vpos , 64);

	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0 )
			continue;

		if( !m_Area64[ m_idxdir[i] ].empty() )
		{
			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();


			for( ; iter != iter_end; ++iter)
			{
				if(OBJ_NONE == (*iter)->GetOBJNAME())
					continue;
				if(CATEGORY_NONE == (*iter)->GetCategory())
					continue;
				if( (*iter)->GetUnitinfo().detect[eteam] < 1)
					continue;

				idx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
				if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(idx , eteam))
					continue;

				if(MyPtInrect( vpos , &(*iter)->GetMyRect() ))
				{
					if( TEAM_NONE == (*iter)->GetTeamNumber())
					{
						(*iter)->SetSelect(BLINK_SELECT , D3DCOLOR_ARGB(255,255,255,255));
					}
					else
					{
						(*iter)->SetSelect(BLINK_SELECT );
					}					

					m_pchoice_target = (*iter);
					return;
				}
			}
		}
	}
	//한마리만 선택할때
}
bool CArea_Mgr::DummyCheck(const int& idx, const MYRECT<float>& rc)
{
	if( !m_Area64[idx].empty() )
	{
		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
		int idx = 0;

		for( ; iter != iter_end; ++iter)
		{
			if( (*iter)->GetUnitinfo().detect[eteam] < 1)
				continue;

			if( CATEGORY_NONE == (*iter)->GetCategory() ||
				CATEGORY_RESOURCE == (*iter)->GetCategory())
				continue;

			idx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
			if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(idx , eteam))
				continue;

			if(MyIntersectrect( &(*iter)->GetMyRect() , &rc ) ||
				MyPtInrect( (*iter)->GetPos() , &rc ))
			{
				return true;
			}
		}
	}
	return false;
}
bool CArea_Mgr::DummyCheck(const int& idx, const D3DXVECTOR2& vpos)
{
	if( !m_Area64[idx].empty() )
	{
		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		int idx = 0;
		TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

		for( ; iter != iter_end; ++iter)
		{
			if( (*iter)->GetUnitinfo().detect[eteam] < 1)
				continue;
			if(OBJ_NONE == (*iter)->GetOBJNAME())
				continue;

			idx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
			if(FOG_ALPHA != CTileManager::GetInstance()->GetFogLight(idx , eteam))
				continue;

			if(MyPtInrect(vpos , &(*iter)->GetMyRect() ))
			{
				return true;
			}
		}
	}
	return false;
}

void CArea_Mgr::SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj )
{

	if( oldidx >= 0 && !m_Area64[oldidx].empty())
	{
		list<CObj*>::iterator iter = m_Area64[oldidx].begin();
		list<CObj*>::iterator iter_end = m_Area64[oldidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area64[oldidx].erase(iter);
				break;
			}
			else
				++iter;
		}
	}

	m_Area64[curidx].push_back(pobj);
}

bool CArea_Mgr::Check_Area(const MYRECT<float>& prerc , D3DXVECTOR2& vprepos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos, CObj* pself ,CObj* ptarget ,const int& stepsize)
{

	m_stepsize = stepsize;
	m_unitrc = prerc;//탐지할 노드의 렉트
	m_voripos = vcenterpos;
	m_orirc = center_rc;


	CMyMath::Calculator_eight_idx(m_idxdir , vprepos , 64);

	if(PathFind_Area( vprepos , pself , ptarget))
		return true;
	else
	{
		Make_rectline(m_voripos , vprepos , m_orirc , m_unitrc);

		if(OBB_Collision( pself ,ptarget ,m_unitrc))
			return false;
		else
			return true;
	}

}
bool CArea_Mgr::PathFind_Area(D3DXVECTOR2& vpos , CObj* pself , CObj* ptarget)
{

	int iflag = 0;
	int idx = 0;
	float widthvtx = pself->GetVertex().right;
	float heightvtx = pself->GetVertex().bottom;
	const int vtxcnt = 4;
	D3DXVECTOR2 temppos[vtxcnt];
	D3DXVECTOR2	opponent_pos;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

	for(int z = 0; z < ASTAR_DIR_END; ++z)
	{
		idx = m_idxdir[z];
		if( idx < 0)
			continue;
		if( m_Area64[idx].empty())
			continue;

		iter = m_Area64[idx].begin();
		iter_end = m_Area64[idx].end();

		MYRECT<float> otherrc;
		MYRECT<float> temprc;

		int preidx = CMyMath::Pos_to_index(vpos , m_stepsize);

		for( ; iter != iter_end; ++iter )
		{
			if( pself == (*iter))
				continue;

			m_target_unitinfo = (*iter)->GetUnitinfo();

			if(MOVE == m_target_unitinfo.state)
				continue;
			if(BURROW == m_target_unitinfo.state)
				continue;
			if( MOVE_AIR == m_target_unitinfo.eMoveType)
				continue;

			if(ORDER_GATHER == pself->GetUnitinfo().order ||
				ORDER_RETURN_CARGO == pself->GetUnitinfo().order)
			{
				if( CATEGORY_RESOURCE != (*iter)->GetCategory() &&
					CATEGORY_BUILDING != (*iter)->GetCategory())
					continue;
			}
			/*탐색 제외 유닛조건*/


			otherrc = (*iter)->GetMyRect();
			opponent_pos = (*iter)->GetPos();

			//LeftUp
			temppos[0].x = otherrc.left - widthvtx;
			temppos[0].y = otherrc.top - heightvtx;
			//RightUp
			temppos[1].x = otherrc.right + widthvtx;
			temppos[1].y = otherrc.top - heightvtx;
			//RightDown
			temppos[2].x = otherrc.right + widthvtx;
			temppos[2].y = otherrc.bottom + heightvtx;
			//LeftDown
			temppos[3].x = otherrc.left - widthvtx;
			temppos[3].y = otherrc.bottom + heightvtx;
			/*꼭지점을 만든다.*/

			for(int i = 0; i < vtxcnt; ++i)
			{
				if(preidx == CMyMath::Pos_to_index(temppos[i] , m_stepsize))
				{
					temprc.left = temppos[i].x - widthvtx;
					temprc.right = temppos[i].x + widthvtx;
					temprc.top = temppos[i].y - heightvtx;
					temprc.bottom = temppos[i].y + heightvtx;

					Make_rectline(m_voripos , temppos[i] , m_orirc , temprc);
					/*가상의 박스를 만들고*/

					if(OBB_Collision((*iter)) || OBB_Collision(pself , ptarget , temprc))
						continue;

					vpos = temppos[i];
					m_unitrc.left = vpos.x - widthvtx;
					m_unitrc.right = vpos.x + widthvtx;
					m_unitrc.top = vpos.y - heightvtx;
					m_unitrc.bottom = vpos.y + heightvtx;
					return true; //충돌검사까지 다 끝냈다 지나가도 되는 노드이다.
				}
			}
		}
	}

	return false;
}

void CArea_Mgr::Areasize_debugrender(const int& areasize , const int& areacnt)
{
	if(!CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_AREAGRID])
		return;

	int startX = (int)(CScrollMgr::m_fScrollX/areasize);
	int startY = (int)(CScrollMgr::m_fScrollY/areasize);

	int maxloopX = BACKBUFFER_SIZEX/areasize + startX + 1;
	int maxloopY = BACKBUFFER_SIZEY/areasize + startY + 1;

	int idx = 0;
	int isize = 0;

	float fX = 0;
	float fY = 0;

	for(int i = startY; i < maxloopY; ++i)
	{
		for(int j = startX; j < maxloopX; ++j)
		{
			idx = i*areacnt + j;
			if(64 == areasize)
			{
				fX = m_areapos64[idx].x - CScrollMgr::m_fScrollX + 30;
				fY = m_areapos64[idx].y - CScrollMgr::m_fScrollY + 30;
				isize = (int)(m_Area64[idx].size());
			}
			CFontMgr::GetInstance()->Setbatch_Font( L"%d" , isize , fX , fY);
		}
	}
}
int CArea_Mgr::overlap_prevention(const MYRECT<float>& prerc ,const MYRECT<float>& orirect , const D3DXVECTOR2& vpos , const D3DXVECTOR2& oripos ,CObj* pself , CObj* ptarget)
{
	CMyMath::Calculator_eight_idx(m_idxdir , vpos , 64);


	ORDER eself_order = pself->GetUnitinfo().order;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;
	float fdot = 0.f;

 
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{		
		if(m_idxdir[i] < 0)
			continue;

		if( !m_Area64[ m_idxdir[i] ].empty() )
		{
			iter = m_Area64[m_idxdir[i]].begin();
			iter_end = m_Area64[m_idxdir[i]].end();

			for( ; iter != iter_end; ++iter )
			{
				m_target_unitinfo = (*iter)->GetUnitinfo();

				if(ptarget == (*iter))
					continue;
				if( MOVE_AIR == m_target_unitinfo.eMoveType || pself == (*iter) )
					continue;
				if(BURROW == m_target_unitinfo.state)
					continue;

				if(ORDER_GATHER == eself_order ||
					ORDER_RETURN_CARGO == eself_order)
				{
					if( CATEGORY_RESOURCE != (*iter)->GetCategory() &&
						CATEGORY_BUILDING != (*iter)->GetCategory())
						continue;
				}
				if(OBJ_LARVA == (*iter)->GetOBJNAME())
					continue;

				if(MyIntersectrect(&(*iter)->GetMyRect() , &prerc))
				{
					//if( OBJ_NONE == (*iter)->GetOBJNAME())
					//	return PATHFIND_OPTION::IMMEDIATE_PATHFIND;
					//if( CATEGORY_BUILDING == (*iter)->GetCategory())
					//	return PATHFIND_OPTION::IMMEDIATE_PATHFIND;
					if(ATTACK == m_target_unitinfo.state)
						return PATHFIND_OPTION::IMMEDIATE_PATHFIND;
					else if(TRANSFORMING == m_target_unitinfo.state)
						return PATHFIND_OPTION::IMMEDIATE_PATHFIND;
					/*공격중이거나, 특수행동중인 유닛과 충돌하면 바로 길찾기한다*/

					D3DXVECTOR2 target_dir = (*iter)->GetcurDir();

					MYRECT<float> target_rc;
					MYRECT<float> target_vtx = (*iter)->GetVertex();
					D3DXVECTOR2 vtarget_pos = (*iter)->GetPos();
					float target_speed = (*iter)->GetUnitinfo().fspeed;


					vtarget_pos += target_dir * target_speed * GETTIME * 2;
					/*충돌 당한 유닛을 움직여본다*/

					target_rc.left = vtarget_pos.x - target_vtx.left;
					target_rc.right = vtarget_pos.x + target_vtx.right;
					target_rc.top = vtarget_pos.y - target_vtx.top;
					target_rc.bottom = vtarget_pos.y + target_vtx.bottom;
					/*바운딩 박스도 같이 움직여야 한다.*/

					if(MyIntersectrect(&orirect , &target_rc))/*충돌 검사*/
					{
						(*iter)->SetState(IDLE);
						return PATHFIND_OPTION::IMMEDIATE_PATHFIND;
					}
					else
						return PATHFIND_OPTION::WAIT;
				}
			}
		}
	}

	return PATHFIND_OPTION::NONE;
}

void CArea_Mgr::SetChoiceTarget(CObj* pobj)
{
	m_pchoice_target = pobj;
}
CObj* CArea_Mgr::GetChoiceTarget(void)
{
	return m_pchoice_target;
}
void CArea_Mgr::ReleaseObj_Area64(const int& curidx , CObj* pobj)
{
	if(!m_Area64[curidx].empty())
	{
		list<CObj*>::iterator iter = m_Area64[curidx].begin();
		list<CObj*>::iterator iter_end = m_Area64[curidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area64[curidx].erase(iter);
				break;
			}
			else
				++iter; //이걸 안해준게 문제였다 이걸 안해줘서 무한루프 돌았어...
		}
	}
}
CObj* CArea_Mgr::Search_Partbuilding(const D3DXVECTOR2& mainPos , const D3DXVECTOR2& partPos ,const OBJID eid)
{
	//Calculator_eightidx(areaidx , 64);
	//D3DXVECTOR2 vtemppos = CMyMath::index_to_Pos(areaidx , 64 , 64);
	CMyMath::Calculator_eight_idx(m_idxdir , partPos , 64);

	CObj* partBuilding = NULL;
	float minDis = 4096*4096.f;

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;
		if(m_Area64[ m_idxdir[i] ].empty())
			continue;

		list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
		list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

		for( ; iter != iter_end; ++iter) 
		{
			if( eid == (*iter)->GetOBJNAME() )
			{
				//if( searchidx == (*iter)->Getcuridx(32) )
				if(CMyMath::pos_distance(partPos , (*iter)->GetPos()) < 32*32)
				{
					minDis = CMyMath::pos_distance(partPos , (*iter)->GetPos());
					partBuilding = (*iter);
				}				
			}

		}
	}

	return partBuilding;
}

void CArea_Mgr::Setirradi_damage(CObj* pmyobj ,const int& damage , DAMAGE_TYPE edamagetype, const D3DXVECTOR2& vsplash_pos, const float* splash_range  , bool random_splash , list<CObj*>& injurelist)
{
	D3DXVECTOR2 start_pos = D3DXVECTOR2(vsplash_pos.x - splash_range[2] , vsplash_pos.y - splash_range[2]);
	D3DXVECTOR2 end_pos = D3DXVECTOR2(vsplash_pos.x + splash_range[2] , vsplash_pos.y + splash_range[2]); 

	if(start_pos.x <= 0)
		start_pos.x = 0;

	if(end_pos.x >= 4096)
		end_pos.x = 4096;

	if(start_pos.y <= 0)
		start_pos.y = 0;

	if(end_pos.y >= 4096)
		end_pos.y = 4096;

	int startidx = CMyMath::Pos_to_index(start_pos , 64);
	int endidx = CMyMath::Pos_to_index(end_pos , 64);
	int idx = 0;

	int row = endidx % 64 - startidx % 64 + 1;
	int col = endidx / 64 - startidx / 64 + 1;


	D3DXVECTOR2	vtarget_pos;
	float fdistance = 0.f;

	bool is_skip = false;
	for(int i = 0; i < col; ++i)
	{
		for(int j = 0; j < row; ++j)
		{			
			idx = startidx + i*64 + j;

			if(m_Area64[idx].empty())
				continue;

			list<CObj*>::iterator iter = m_Area64[ idx ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

			for( ; iter != iter_end; ++iter) 
			{				

				if( CATEGORY_RESOURCE == (*iter)->GetCategory())
					continue;

				if(OBJRACE_CREATURE != (*iter)->GetUnitinfo().erace)
					continue;

				if(false == random_splash)
				{
					if( pmyobj->GetTeamNumber() == (*iter)->GetTeamNumber() )
						continue;
				}

				is_skip = false;
				for( list<CObj*>::iterator injure_iter = injurelist.begin();
					injure_iter != injurelist.end(); ++injure_iter)
				{
					if( *iter == *injure_iter )
					{
						is_skip = true;
						break;
					}
				}
				if(true == is_skip)
					continue;

				vtarget_pos = (*iter)->GetPos();
				fdistance = CMyMath::pos_distance( vsplash_pos , vtarget_pos);

				if(fdistance <= splash_range[2] * splash_range[2])
				{
					if(fdistance <= splash_range[1] * splash_range[1])
					{
						if(fdistance <= splash_range[0] * splash_range[0])
						{
							injurelist.push_back((*iter));
							(*iter)->SetDamage(damage , edamagetype);
							continue;
						}
						injurelist.push_back((*iter));
						(*iter)->SetDamage(damage/2 , edamagetype);
						continue;
					}
					injurelist.push_back((*iter));
					(*iter)->SetDamage(damage/4 , edamagetype);
					continue;
				}
			}
		}
	}
}
void CArea_Mgr::Setsplash_damage(CObj* pmyobj ,const int& damage , DAMAGE_TYPE edamagetype, const D3DXVECTOR2& vsplash_pos, const float* splash_range  , bool random_splash , list<CObj*>& injurelist)
{
	//range는 반지름이다

	D3DXVECTOR2 start_pos = D3DXVECTOR2(vsplash_pos.x - splash_range[2] - 64 , vsplash_pos.y - splash_range[2] - 64);
	D3DXVECTOR2 end_pos = D3DXVECTOR2(vsplash_pos.x + splash_range[2] + 64, vsplash_pos.y + splash_range[2] + 64); 

	if(start_pos.x <= 0)
		start_pos.x = 0;

	if(end_pos.x >= 4096)
		end_pos.x = 4096;

	if(start_pos.y <= 0)
		start_pos.y = 0;

	if(end_pos.y >= 4096)
		end_pos.y = 4096;

	int startidx = CMyMath::Pos_to_index(start_pos , 64);
	int endidx = CMyMath::Pos_to_index(end_pos , 64);
	int idx = 0;

	int row = int((end_pos.x - start_pos.x) / 64) + 1;
	int col = int((end_pos.y - start_pos.y) / 64) + 1;


	ATTACK_SEARCH_TYPE att_search_type = pmyobj->GetUnitinfo().eAttackType;
	D3DXVECTOR2	vtarget_pos;
	float fdistance = 0.f;

	for(int i = 0; i < col; ++i)
	{
		for(int j = 0; j < row; ++j)
		{			
			idx = startidx + i * 64 + j;

			if(m_Area64[idx].empty())
				continue;

			list<CObj*>::iterator iter = m_Area64[ idx ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

			for( ; iter != iter_end; ++iter) 
			{
				if(OBJ_NONE == (*iter)->GetOBJNAME())
					continue;

				if( CATEGORY_RESOURCE == (*iter)->GetCategory())
					continue;

				if( ATTACK_ONLY_AIR == att_search_type)
				{
					if( MOVE_GROUND == (*iter)->GetUnitinfo().eMoveType)
						continue;
				}
				else if( ATTACK_ONLY_GROUND == att_search_type)
				{
					if( MOVE_AIR == (*iter)->GetUnitinfo().eMoveType)
						continue;
				}

				if(false == random_splash)
				{
					if( pmyobj->GetTeamNumber() == (*iter)->GetTeamNumber() )
						continue;
				}

				bool is_skip = false;
				for( list<CObj*>::iterator injure_iter = injurelist.begin();
					injure_iter != injurelist.end(); ++injure_iter)
				{
					if( *iter == *injure_iter )
					{
						is_skip = true;
						break;
					}
				}
				if(true == is_skip)
					continue;

				vtarget_pos = (*iter)->GetPos();
				fdistance = CMyMath::pos_distance( vsplash_pos , vtarget_pos);

				if(fdistance <= splash_range[2] * splash_range[2])
				{
					if(fdistance <= splash_range[1] * splash_range[1])
					{
						if(fdistance <= splash_range[0] * splash_range[0])
						{
							injurelist.push_back((*iter));
							(*iter)->SetDamage(damage , edamagetype);
							continue;
						}
						injurelist.push_back((*iter));
						(*iter)->SetDamage(damage/2 , edamagetype);
						continue;
					}
					injurelist.push_back((*iter));
					(*iter)->SetDamage(damage/4 , edamagetype);
					continue;
				}
			}
		}
	}
}

void CArea_Mgr::Make_rectline(const D3DXVECTOR2& oripos , const D3DXVECTOR2& vpos , const MYRECT<float>& orirect , const MYRECT<float>& prerc)
{
	float fx = oripos.x - vpos.x;
	float fy = oripos.y - vpos.y;

	//0번과 2번이 교차 할 라인이다 0번은 1시 방향

	//	int lineidx;
	if(fx < 0)
	{
		if(fy < 0)
		{
			//센터보다 오른쪽 아래에 있다.
			//0,2

			m_lineidx = 0;
			m_line[0].setpoint(orirect.right , orirect.top , prerc.right , prerc.top);
			m_line[1].setpoint(prerc.right , prerc.top , prerc.left, prerc.bottom);
			m_line[2].setpoint(prerc.left , prerc.bottom , orirect.left, orirect.bottom);
			m_line[3].setpoint(orirect.left , orirect.bottom , orirect.right, orirect.top);

		}
		else
		{
			//센터보다 오른쪽 위에 있다.
			//1 , 3
			m_lineidx = 1;
			m_line[0].setpoint(prerc.left , prerc.top , prerc.right, prerc.bottom);
			m_line[1].setpoint(prerc.right , prerc.bottom , orirect.right, orirect.bottom);
			m_line[2].setpoint(orirect.right , orirect.bottom , orirect.left, orirect.top);
			m_line[3].setpoint(orirect.left , orirect.top , prerc.left, prerc.top);
		}
	}
	else if(fabsf(fx) <= FLT_EPSILON)
	{
		if(fy < 0)
		{
			//센터보다 아래에 있다.
			m_lineidx = 1;
			m_line[0].setpoint(orirect.left , orirect.bottom , orirect.right, orirect.bottom);
			m_line[1].setpoint(orirect.right , orirect.bottom , prerc.right, prerc.top);
			m_line[2].setpoint(prerc.right , prerc.top , prerc.left, prerc.top);
			m_line[3].setpoint(prerc.left , prerc.top , orirect.left, orirect.bottom);
		}
		else
		{
			//센터보다 위에 있다.
			m_lineidx = 1;
			m_line[0].setpoint(prerc.left , prerc.bottom , prerc.right, prerc.bottom);
			m_line[1].setpoint(prerc.right , prerc.bottom , orirect.right, orirect.top);
			m_line[2].setpoint(orirect.right , orirect.top , orirect.left, orirect.top);
			m_line[3].setpoint(orirect.left , orirect.top , prerc.left, prerc.bottom);
		}
	}
	else
	{
		//센터보다 왼쪽에 있다.
		if(fy < 0)
		{
			//센터보다 왼쪽 아래에 있다.

			//1,3
			m_lineidx = 1;
			m_line[0].setpoint(orirect.left , orirect.top , orirect.right, orirect.bottom);
			m_line[1].setpoint(orirect.right , orirect.bottom , prerc.right, prerc.bottom);
			m_line[2].setpoint(prerc.right , prerc.bottom , prerc.left, prerc.top);
			m_line[3].setpoint(prerc.left , prerc.top , orirect.left, orirect.top);
		}
		else
		{
			//센터보다 왼쪽 위에 있다.
			//0,2
			m_lineidx = 0;
			m_line[0].setpoint(prerc.right , prerc.top , orirect.right, orirect.top);
			m_line[1].setpoint(orirect.right , orirect.top , orirect.left, orirect.bottom);
			m_line[2].setpoint(orirect.left , orirect.bottom , prerc.left, prerc.bottom);
			m_line[3].setpoint(prerc.left , prerc.bottom , prerc.right, prerc.top);
		}
	}
	m_line[4].setpoint(oripos.x , oripos.y , vpos.x, vpos.y);
}

bool CArea_Mgr::OBB_Collision( CObj* pself ,const CObj* ptarget ,const MYRECT<float>& pre_rc)
{
	//아...... 건물크기는 양옆,위아래가 짝짝이라서 OBB가 먹히지않는다
	//OBB의 길이는 서로 똑같은 반지름이어야 하는데....
	D3DXVECTOR2 vseparate[4];
	D3DXVECTOR2 vproj[4];
	D3DXVECTOR2 vcenter;
	float	fLength[4];
	float ra ,rb;
	float fd;
	D3DXVECTOR2 vpos , otherpos;
	MYRECT<float>	other_rc;


	vpos = D3DXVECTOR2((m_line[4].fx1 + m_line[4].fx2) / 2.f , (m_line[4].fy1 + m_line[4].fy2) / 2.f);

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//D3DXVECTOR2 vpre_pos = D3DXVECTOR2( (pre_rc.left + pre_rc.right) / 2.f , (pre_rc.top + pre_rc.bottom) / 2.f );
	//D3DXVECTOR2 voripos = D3DXVECTOR2(m_line[4].fx1 , m_line[4].fy1);

	//float fdistance = 0.f;
	//MYRECT<float> my_vtx = pself->GetVertex();
	//float my_radius = CMyMath::pos_distance(vpre_pos , voripos) + my_vtx.left * my_vtx.left + my_vtx.top * my_vtx.top;

	//
	//MYRECT<float> other_vtx;
	//float other_radius = 0.f;
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	for(int i = 0; i < 2; ++i)
	{
		vseparate[i] = D3DXVECTOR2(m_line[i].fx2 - m_line[i].fx1 , m_line[i].fy2 - m_line[i].fy1);
		fLength[i] = D3DXVec2Length(&vseparate[i]) / 2;
		D3DXVec2Normalize(&vseparate[i] , &vseparate[i]);
		vproj[i] = vseparate[i] * fLength[i];
	}


	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;
		if(m_Area64[ m_idxdir[i] ].empty())
			continue;

		list<CObj*>::iterator iter = m_Area64[m_idxdir[i]].begin();
		list<CObj*>::iterator iter_end = m_Area64[m_idxdir[i]].end();

		for( ; iter != iter_end; ++iter)
		{
			if( pself == (*iter))
				continue;
			if( ptarget == (*iter)) 
				continue;

			m_target_unitinfo = (*iter)->GetUnitinfo();

			if(	MOVE == m_target_unitinfo.state )
				continue;
			if( MOVE_AIR == m_target_unitinfo.eMoveType )
				continue;
			if(BURROW == m_target_unitinfo.state)
				continue;

			if(ORDER_GATHER == pself->GetUnitinfo().order ||
				ORDER_RETURN_CARGO == pself->GetUnitinfo().order)
			{
				if( CATEGORY_RESOURCE != (*iter)->GetCategory() &&
					CATEGORY_BUILDING != (*iter)->GetCategory())
					continue;
			}

			//내위치와 상대 거리를재서 멀면 예외 가깝다면 OBB
			
			other_rc = (*iter)->GetMyRect();

			if(MyIntersectrect(&pre_rc , &other_rc))
				return true;

			vseparate[2] = D3DXVECTOR2( float(other_rc.right - other_rc.left) , 0.f);
			vseparate[3] = D3DXVECTOR2(0.f , float(other_rc.bottom - other_rc.top));
			otherpos = D3DXVECTOR2(float(other_rc.right + other_rc.left) / 2.f , float(other_rc.bottom + other_rc.top) / 2.f);
			vcenter = ( vpos - otherpos );
			for(int i = 2; i < 4; ++i)
			{
				fLength[i] = D3DXVec2Length(&vseparate[i]) / 2;
				D3DXVec2Normalize(&vseparate[i] , &vseparate[i]);
				vproj[i] = vseparate[i] * fLength[i];
			}


			bool bcoll = false;
			for(int i = 0; i < 4; ++i)
			{
				ra = (fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[0])) + fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[1])));
				rb = (fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[2])) + fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[3])));

				fd = (fabsf(D3DXVec2Dot( &vseparate[i] , &vcenter)));

				if( fabsf(ra + rb - fd) < FLT_EPSILON )
				{
					bcoll = true;
					break;
				}
				else if(ra + rb < fd) //분리되어있다.
				{
					bcoll = true;
					break;
				}
			}

			if( false == bcoll )
				return true; //겹친다
		}
	}
	return false;
}
bool CArea_Mgr::OBB_Collision(CObj* ptarget)
{
	D3DXVECTOR2 vseparate[4];
	D3DXVECTOR2 vproj[4];
	D3DXVECTOR2 vcenter;
	float	fLength[4];
	float ra ,rb;
	float fd;
	D3DXVECTOR2 vpos , otherpos;
	MYRECT<float>	other_rc;

	vpos = D3DXVECTOR2((m_line[4].fx1 + m_line[4].fx2) / 2.f , (m_line[4].fy1 + m_line[4].fy2) / 2.f);

	for(int i = 0; i < 2; ++i)
	{
		vseparate[i] = D3DXVECTOR2(m_line[i].fx2 - m_line[i].fx1 , m_line[i].fy2 - m_line[i].fy1);
		fLength[i] = D3DXVec2Length(&vseparate[i]) / 2;
		D3DXVec2Normalize(&vseparate[i] , &vseparate[i]);
		vproj[i] = vseparate[i] * fLength[i];
	}


	other_rc = ptarget->GetMyRect();

	vseparate[2] = D3DXVECTOR2( float(other_rc.right - other_rc.left) , 0.f);
	vseparate[3] = D3DXVECTOR2(0.f , float(other_rc.bottom - other_rc.top));
	otherpos = D3DXVECTOR2(float(other_rc.right + other_rc.left) / 2.f , float(other_rc.bottom + other_rc.top) / 2.f);
	vcenter = ( vpos - otherpos );
	for(int i = 2; i < 4; ++i)
	{
		fLength[i] = D3DXVec2Length(&vseparate[i]) / 2;
		D3DXVec2Normalize(&vseparate[i] , &vseparate[i]);
		vproj[i] = vseparate[i] * fLength[i];
	}


	bool bcoll = false;
	for(int i = 0; i < 4; ++i)
	{
		ra = (fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[0])) + fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[1])));
		rb = (fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[2])) + fabsf(D3DXVec2Dot( &vseparate[i] , &vproj[3])));

		fd = (fabsf(D3DXVec2Dot( &vseparate[i] , &vcenter)));

		if( fabsf(ra + rb - fd) < FLT_EPSILON ) //분리된걸로 간주한다.
		{
			bcoll = true;
			break;
		}
		else if(ra + rb < fd) //분리되어있다.
		{
			bcoll = true;
			break;
		}
	}

	if( false == bcoll )
		return true; //겹친다

	return false;//겹치지 않는다
}
bool CArea_Mgr::PointLine_Overlap(const D3DXVECTOR2& _vpos, MYLINE* _line)
{
	for(int i = 0; i < 4; ++i)
	{
		if( fabsf(_line[i].fx1 - _vpos.x) < FLT_EPSILON)
		{
			if( fabsf(_line[i].fy1 - _vpos.y) < FLT_EPSILON)
				return true; //꼭지점에 위치
		}
	}

	if(_line[0].fx1 < _vpos.x && _vpos.x < _line[1].fx1)
	{
		if( fabsf(_line[0].fy1 - _vpos.y) < FLT_EPSILON)
			return true;//가로선
	}
	else if(_line[3].fx1 < _vpos.x && _vpos.x < _line[2].fx1)
	{
		if( fabsf(_line[3].fy1 - _vpos.y) < FLT_EPSILON)
			return true;//가로선
	}

	else if(_line[0].fy1 < _vpos.y && _vpos.y < _line[3].fy1)
	{
		if( fabsf(_line[0].fx1 - _vpos.x) < FLT_EPSILON)
			return true;//세로선
	}
	else if(_line[1].fy1 < _vpos.y && _vpos.y < _line[2].fy1)
	{
		if( fabsf(_line[1].fx1 - _vpos.x) < FLT_EPSILON)
			return true;//세로선
	}

	return false;
}

void CArea_Mgr::setobstacle(CObj* pobj)
{
	m_obstacle_list.push_back(pobj);
}

list<CObj*>* CArea_Mgr::GetArea64(void)
{
	return m_Area64;
}