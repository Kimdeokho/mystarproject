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
CObj* CArea_Mgr::Auto_explore_target(CObj* pself , const int& isearch_range , TARGET_SEARCH_TYPE esearchtype)
{
	D3DXVECTOR2 vstart_pos = pself->GetPos();
	D3DXVECTOR2 vpos = pself->GetPos();
	int stepsize = 64;
	int tilecnt = (isearch_range + stepsize)/ stepsize;

	int loopcnt = 2;
	CObj* ptarget = NULL;
	float fminvalue = 4096*4096;

	Target_extract(pself , vpos ,fminvalue , ptarget, isearch_range , esearchtype);

	//if(NULL != ptarget)
	//return ptarget;

	for(int i = 1; i <= tilecnt; ++i)
	{
		vpos.x = vstart_pos.x - i * 64;
		vpos.y = vstart_pos.y - i * 64;

		//시계방향 탐색
		for(int j = 0; j < loopcnt; ++j)
		{
			//오른쪽 전진
			vpos.x += 64;
			Target_extract(pself , vpos ,fminvalue , ptarget, isearch_range , esearchtype);

		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//아래 전진
			vpos.y += 64;
			Target_extract(pself , vpos ,fminvalue , ptarget, isearch_range , esearchtype);
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//왼쪽 전진
			vpos.x -= 64;
			Target_extract(pself , vpos ,fminvalue , ptarget, isearch_range , esearchtype);
		}

		for(int j = 0; j < loopcnt; ++j)
		{
			//위쪽 전진
			vpos.y -= 64;
			Target_extract(pself , vpos ,fminvalue , ptarget, isearch_range , esearchtype);
		}

		loopcnt += 2;

		if(NULL != ptarget)
			return ptarget;
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

	//CObj* ptarget = NULL;
	//float fminvalue = 4096*4096;

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


	//CObj* ptarget = NULL;
	//int stepsize = 64;
	//int tilecnt = (isearch_range + stepsize)/ stepsize;

	//int totalcnt = (tilecnt*2 + 1)*(tilecnt*2 + 1);	

	//int curcnt = 0; //현재 갯수
	//int curidx = idx + 1;
	//D3DXVECTOR2 startpos = CMyMath::index_to_Pos(curidx , 64 , stepsize);
	//ASTAR_DIR curdir = LEFT;
	//int advancecnt = 1; //한방향으로 움직일 전진 횟수
	//int cornercnt = 0;

	//int stepdir = -1;

	//while(curcnt < totalcnt)
	//{	

	//	for(int i = 0; i < advancecnt; ++i)
	//	{
	//		curidx += stepdir;

	//		if(UP == curdir)
	//			startpos.y -= stepsize;
	//		else if(DOWN == curdir)
	//			startpos.y += stepsize;
	//		else if(LEFT == curdir)
	//			startpos.x -= stepsize;
	//		else if(RIGHT == curdir)
	//			startpos.x += stepsize;

	//		if( startpos.y < 0 || startpos.y > 4096 ||
	//			startpos.x < 0 || startpos.x > 4096)
	//			continue;

	//		if(true == Mineral_extractor(curidx ,pself, pmineral))
	//			return true;

	//		++curcnt;
	//		if(curcnt >= totalcnt)
	//			break;

	//	}

	//	switch(curdir)
	//	{			
	//	case UP:
	//		{
	//			curdir = RIGHT;
	//			stepdir = 1;
	//			break;
	//		}
	//	case RIGHT:
	//		{
	//			curdir = DOWN;
	//			stepdir = stepsize;
	//			break;
	//		}
	//	case DOWN:
	//		{
	//			curdir = LEFT;
	//			stepdir = -1;
	//			break;
	//		}
	//	case LEFT:
	//		{
	//			curdir = UP;
	//			stepdir = -stepsize;
	//			break;
	//		}
	//	}

	//	if(cornercnt >= 2)
	//	{
	//		++advancecnt;
	//		cornercnt = 0;
	//	}
	//	++cornercnt;
	//}

	//return false;
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
			fogidx = CMyMath::Pos_to_index( vtargetpos , 32);
			if(false == CTileManager::GetInstance()->GetFogLight(fogidx))
				continue;

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
			if(false == CTileManager::GetInstance()->GetFogLight(fogidx))
				continue;

			if(OBJ_MINERAL == pmineral->GetOBJNAME())
			{
				if( ((CMineral*)(*iter))->Getworkman_count() >= 2)
					continue;

				pmineral = (*iter);
				return true;	
			}

			//((CMineral*)pmineral)->Setworkman(pself);
			//((CWorkman*)pself)->SetMineral_mark(pmineral);

		}
	}
	return false;
}
void CArea_Mgr::gas_extractor(CObj* pself , const D3DXVECTOR2& vpt)
{
	int idx64 = pself->Getcuridx(64);

	Calculator_eightidx(idx64 , 64);


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
void CArea_Mgr::Target_extract(CObj* pself ,const D3DXVECTOR2& vpos, float& fminvalue, CObj*& ptarget,  const int& isearch_range ,TARGET_SEARCH_TYPE esearchtype)
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

	for( ; iter != iter_end; ++iter)
	{

		if( CATEGORY_RESOURCE == (*iter)->GetCategory())
			continue;

		if(pself == (*iter))
			continue;

		//공중만 공격 가능한 유닛은 공중만 검사하고
		//지상만 공격 가능한 유닛은 지상만 검사한다

		etarget_movetype = (*iter)->GetUnitinfo().eMoveType;

		if( SEARCH_ONLY_ENEMY == esearchtype)
		{
			if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
				continue;

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

		vtargetpos = (*iter)->GetPos();
		fogidx = CMyMath::Pos_to_index( vtargetpos , 32);
		if(false == CTileManager::GetInstance()->GetFogLight(fogidx))
			continue;

		idistance = CMyMath::pos_distance(pself->GetPos() , vtargetpos);

		//지금 찾은 유닛과의 거리가 최소거리보다 작다면

		if(idistance <= isearch_range * isearch_range)
		{
			//해당 유닛의 탐지범위에 적합한지 본다.
			//목표를 찾았다.

			if( idistance < fminvalue )
			{
				fminvalue = idistance;						
				ptarget = (*iter);
			}
		}
	}
}
void CArea_Mgr::MedicTarget_extract(CObj* pself ,const D3DXVECTOR2& vpos, float& fminvalue, CObj*& ptarget,  const int& isearch_range)
{
	if(vpos.x <= 0 || vpos.x >= SQ_TILECNTX*32 ||
		vpos.y <= 0 || vpos.y >= SQ_TILECNTX*32)
		return;

	float minvalue = 4096*4096;


	//가장 가까운 적을 찾기로 하자.

	float idistance = 0;
	int fogidx = 0;

	int idx = CMyMath::Pos_to_index(vpos , 64);
	list<CObj*>::iterator iter = m_Area64[idx ].begin();
	list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

	ATTACK_SEARCH_TYPE eself_attacktype = pself->GetUnitinfo().eAttackType;
	//MOVE_TYPE etarget_movetype;
	OBJID eOBJ_NAME;
	D3DXVECTOR2	vtargetpos;

	for( ; iter != iter_end; ++iter)
	{

		if( CATEGORY_RESOURCE == (*iter)->GetCategory())
			continue;

		if(pself == (*iter))
			continue;

		//etarget_movetype = (*iter)->GetUnitinfo().eMoveType;

		if(pself->GetTeamNumber() != (*iter)->GetTeamNumber())
			continue;

		eOBJ_NAME = (*iter)->GetOBJNAME();

		if( OBJ_MARINE == eOBJ_NAME ||
			OBJ_SCV == eOBJ_NAME ||
			OBJ_FIREBAT == eOBJ_NAME ||
			OBJ_MEDIC == eOBJ_NAME)
		{
			vtargetpos = (*iter)->GetPos();
			idistance = CMyMath::pos_distance(pself->GetPos() , vtargetpos);

			//지금 찾은 유닛과의 거리가 최소거리보다 작다면

			if(idistance <= isearch_range * isearch_range)
			{
				if((*iter)->GetUnitinfo().maxhp > (*iter)->GetUnitinfo().hp)
				{
					if(minvalue > idistance)
					{
						minvalue = idistance;						
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

	D3DXVECTOR2 vstartpt = D3DXVECTOR2(rc.left , rc.top);
	D3DXVECTOR2 vendpt = D3DXVECTOR2(rc.right , rc.bottom);;

	if(vendpt.x >= 4096 || vendpt.x <= 0 ||
		vendpt.y <= 0 || vendpt.y >= 4096)
		return;

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


	//여기에 커맨드버튼 갱신
	//CUnitMgr::GetInstance()->Update_Cmdbtn();
}

void CArea_Mgr::SelectCheck(const int& idx , const D3DXVECTOR2& vpos)
{
	//드래그가 아닌 기본 클릭 선택에대한 함수
	Calculator_eightidx(idx , 64);


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


	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if(true == Choice_unit(m_idxdir[i] , vpos))
			break;
	}
}

CObj* CArea_Mgr::Collision_check(CObj* pself , const int& idx)
{
	//주위 충돌검사

	//int idx = pself->Getcuridx(64);
	Calculator_eightidx(idx , 64);

	MOVE_TYPE selfmovetype = pself->GetUnitinfo().eMoveType;
	MOVE_TYPE targetmovetype;

	ORDER eself_order = pself->GetUnitinfo().order;
	ORDER other_order;

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

				//if( BOARDING == (*iter)->GetUnitinfo().state)
				//{
				//	continue;
				//}

				if(MyIntersectrect( &temp , &pself->GetMyRect() , &(*iter)->GetMyRect() ))
				{
					return *iter;
				}
			}
		}
	}

	return NULL;
}
bool CArea_Mgr::Building_Collocate_check(CObj* pself , const int& idx ,MYRECT<float>& myrc )
{
	Calculator_eightidx(idx , 64);

	MOVE_TYPE othermovetype;

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

				if(MOVE_AIR == othermovetype)
					continue;

				if(MyIntersectrect( &temp , &myrc , &(*iter)->GetMyRect() ))
					return false;
			}
		}
	}
	return true;
}
bool CArea_Mgr::Collocate_check(CObj* pself , const int& idx ,MYRECT<float>& myrc )
{
	//주위 충돌검사

	//int idx = pself->Getcuridx(64);
	Calculator_eightidx(idx , 64);

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
	int idx = pself->Getcuridx(64);
	Calculator_eightidx(idx , 64);

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
bool CArea_Mgr::Choice_unit(const int& idx , const D3DXVECTOR2& vpos)
{
	//CUI* preview = CIngame_UIMgr::GetInstance()->GetPreview();
	//if(true == ((CBuilding_Preview*)preview)->GetActive() )
	//	return false;

	//한마리만 선택할때
	if( !m_Area64[idx].empty() )
	{
		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(MyPtInrect( vpos , &(*iter)->GetMyRect() ))
			{
				CUnitMgr::GetInstance()->discharge_unit();

				if( TEAM_NONE == (*iter)->GetTeamNumber())
					(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,255,255));
				else if( CSession_Mgr::GetInstance()->GetTeamNumber() != 
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
		for( ; iter != iter_end; ++iter)
		{
			if(MyIntersectrect(&(*iter)->GetMyRect() , &rc ))
			{
				if(CATEGORY_UNIT == (*iter)->GetCategory())
				{
					//if(CUnitMgr::GetInstance()->GetSelectunit_size() < 100)					
					CUnitMgr::GetInstance()->SetUnit((*iter));

					if((*iter)->GetTeamNumber() != CSession_Mgr::GetInstance()->GetTeamNumber())
					{
						(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,0,0));
						return false;
					}
					else
					{
						(*iter)->SetSelect(GENERAL_SELECT);
					}
				}
				/*아군만 선택*/
			}
			else
				(*iter)->SetSelect(NONE_SELECT);
		}
	}

	return true;
}
void CArea_Mgr::Choice_building(const int& idx, const MYRECT<float>& rc)
{
	if( !m_Area64[idx].empty() )
	{

		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		if(CUnitMgr::GetInstance()->GetUnitlistempty())
		{
			iter = m_Area64[idx].begin();
			iter_end = m_Area64[idx].end();

			for( ; iter != iter_end; ++iter)
			{
				if(MyIntersectrect(&(*iter)->GetMyRect() , &rc ))
				{
					if(CATEGORY_BUILDING == (*iter)->GetCategory())
					{
						CUnitMgr::GetInstance()->SetUnit((*iter));

						if((*iter)->GetTeamNumber() !=
							CSession_Mgr::GetInstance()->GetTeamNumber())
							(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,0,0));
						else
							(*iter)->SetSelect(GENERAL_SELECT);

						return;
					}
				}
				else
					(*iter)->SetSelect(NONE_SELECT);
			}
		}
	}
}
void CArea_Mgr::TargetChoice(const D3DXVECTOR2& vpos)
{

	m_pchoice_target = NULL;
	int idx = CMyMath::Pos_to_index(vpos , 64);
	Calculator_eightidx(idx , 64);

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
		MYRECT<float> temprc;

		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(MyIntersectrect(&temprc , &(*iter)->GetMyRect() , &rc ))
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

		for( ; iter != iter_end; ++iter)
		{
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

	//m_checkidx = CMyMath::Pos_to_index(vprepos , m_stepsize);
	int idx = CMyMath::Pos_to_index(vprepos , 64);

	Calculator_eightidx(idx , 64);

	//bool bcheck = true;
	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(m_idxdir[i] < 0)
	//		continue;
	//	if( m_Area64[ m_idxdir[i] ].empty())
	//		continue;

	//	if(true == PathFind_Area(m_idxdir[i] , vprepos , pself , ptarget))
	//	{
	//		//return true;
	//		bcheck = false;
	//		break;
	//	}
	//}

	//if(true == bcheck)
	//{
	//	for(int i = 0; i < ASTAR_DIR_END; ++i)
	//	{
	//		if(m_idxdir[i] < 0)
	//			continue;
	//		if(m_Area64[ m_idxdir[i] ].empty())
	//			continue;

	//		if(CollisionCheck_Area(m_idxdir[i] , pself , ptarget))
	//			return false;
	//	}
	//}	
	//Make_rectline(m_voripos , vprepos , m_orirc , m_unitrc);
	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(m_idxdir[i] < 0)
	//		continue;
	//	if(m_Area64[ m_idxdir[i] ].empty())
	//		continue;
	//	if(LineCross(m_idxdir[i] ,m_lineidx, pself , ptarget))
	//		return false;
	//}
	//if(OBB_Collision( pself ,ptarget ))
	//	return false;

	//return true;


	if(PathFind_Area( vprepos , pself , ptarget))
		return true;
	else
	{
		bool bfilter = false;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(m_idxdir[i] < 0)
				continue;
			if(m_Area64[ m_idxdir[i] ].empty())
				continue;

			if(CollisionCheck_Area(m_idxdir[i] , pself , ptarget))
			{
				bfilter = true;
				break;
			}
		}
		if(false == bfilter)
		{
			Make_rectline(m_voripos , vprepos , m_orirc , m_unitrc);
			if(OBB_Collision( pself ,ptarget ))
				return false;
			else
				return true;
		}
		else
			return false;
	}

	//bool bfilter = false;
	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(m_idxdir[i] < 0)
	//		continue;
	//	if(m_Area64[ m_idxdir[i] ].empty())
	//		continue;

	//	if(CollisionCheck_Area(m_idxdir[i] , pself , ptarget))
	//	{
	//		bfilter = true;
	//		break;
	//	}
	//}
	//if(true == bfilter)
	//{
	//	//rect충돌했으니까 OBB는 할 필요가 없다
	//	return PathFind_Area( vprepos , pself , ptarget); //주위 유닛들을 검사해본다
	//}
	//else
	//{
	//	//rect 충돌은 하지 않았지만 OBB충돌 가능성이 있다.
	//	Make_rectline(m_voripos , vprepos , m_orirc , m_unitrc);
	//	if(OBB_Collision( pself ,ptarget ))
	//	{
	//		//OBB가 충돌했어도 주위유닛을 피해갈 수 있는 가능성이 있다.
	//		return PathFind_Area( vprepos , pself , ptarget); //주위 유닛들을 검사해본다
	//	}
	//	else
	//		return true;
	//}
	//return false;



	//bool bfilter = false;
	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(m_idxdir[i] < 0)
	//		continue;
	//	if(m_Area64[ m_idxdir[i] ].empty())
	//		continue;

	//	if(CollisionCheck_Area(m_idxdir[i] , pself , ptarget))
	//	{
	//		bfilter = true;
	//		break;
	//	}
	//}
	//if(true == bfilter)
	//{
	//	//rect충돌했으니까 OBB는 할 필요가 없다
	//	return PathFind_Area( vprepos , pself , ptarget); //주위 유닛들을 검사해본다
	//}
	//else
	//{
	//	Make_rectline(m_voripos , vprepos , m_orirc , m_unitrc);
	//	for(int i = 0; i < ASTAR_DIR_END; ++i)
	//	{
	//		if(m_idxdir[i] < 0)
	//			continue;
	//		if(m_Area64[ m_idxdir[i] ].empty())
	//			continue;

	//		if(LineCross(m_idxdir[i] ,m_lineidx, pself , ptarget))
	//			return PathFind_Area( vprepos , pself , ptarget); //주위 유닛들을 검사해본다
	//	}
	//	return true;
	//}
	//return false;
}
bool CArea_Mgr::PathFind_Area(D3DXVECTOR2& vpos , CObj* pself , CObj* ptarget)
{
	//if(idx < 0)
	//	return false;

	int iflag = 0;
	int idx = 0;
	float widthvtx = pself->GetVertex().right;
	float heightvtx = pself->GetVertex().bottom;
	const int idxcnt = 8;
	D3DXVECTOR2 temppos[idxcnt];
	D3DXVECTOR2	opponent_pos;
	MYLINE templine[4];

	for(int z = 0; z < ASTAR_DIR_END; ++z)
	{
		idx = m_idxdir[z];
		if( idx < 0)
			continue;
		if( m_Area64[idx].empty())
			continue;

		list<CObj*>::iterator iter;
		list<CObj*>::iterator iter_end;

		iter = m_Area64[idx].begin();
		iter_end = m_Area64[idx].end();

		MYRECT<float> otherrc;
		MYRECT<float> temprc;

		int preidx32 = CMyMath::Pos_to_index(vpos , m_stepsize);


		for( ; iter != iter_end; ++iter )
		{
			/*같은그룹, 공중이거나 자기 자신이거나(OBJ_ID) 움직이고 있는 상대는 거른다.*/
			if( ptarget == (*iter))
				continue;
			if( pself == (*iter))
				continue;

			m_target_unitinfo = (*iter)->GetUnitinfo();
			if(MOVE == m_target_unitinfo.state)
				continue;
			if(BURROW == m_target_unitinfo.state)
				continue;
			if( MOVE_AIR == m_target_unitinfo.eMoveType)
				continue;
			if(ORDER_GATHER == m_target_unitinfo.order ||
				ORDER_RETURN_CARGO == m_target_unitinfo.order)
			{
				if( CATEGORY_RESOURCE != (*iter)->GetCategory() &&
					CATEGORY_BUILDING != (*iter)->GetCategory())
					continue;
			}
			//	//피해갈 라인을 만든다.


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

			////LEFT
			temppos[4].x = otherrc.left - widthvtx;
			temppos[4].y = vpos.y;
			////Right
			temppos[5].x = otherrc.right + widthvtx;
			temppos[5].y = vpos.y;
			////UP
			temppos[6].x = vpos.x;
			temppos[6].y = otherrc.top - heightvtx;
			////DOWN
			temppos[7].x = vpos.x;
			temppos[7].y = otherrc.bottom + heightvtx;

			
			for(int a = 0; a < 4; ++a)
			{
				templine[a].fx1 = temppos[a % 4].x;
				templine[a].fy1 = temppos[a % 4].y;
				templine[a].fx2 = temppos[(a+1) % 4].x;
				templine[a].fy2 = temppos[(a+1) % 4].y;
			}

			for(int i = 0; i < idxcnt; ++i)
			{
				//CFontMgr::GetInstance()->SetNoticeFont(L"d" , temppos[i].x , temppos[i].y);
				if(preidx32 == CMyMath::Pos_to_index(temppos[i] , m_stepsize) && 
					PointLine_Overlap(temppos[i] , templine))
				{

					temprc.left = temppos[i].x - widthvtx;
					temprc.right = temppos[i].x + widthvtx;
					temprc.top = temppos[i].y - heightvtx;
					temprc.bottom = temppos[i].y + heightvtx;



					Make_rectline(m_voripos , temppos[i] , m_orirc , temprc);

					if(OBB_Collision((*iter)) || OBB_Collision(pself , ptarget))
						continue;

					bool bfilter = true;
					for(int j = 0; j < ASTAR_DIR_END; ++j)
					{
						if( m_idxdir[j] < 0)
							continue;

						if( !m_Area64[ m_idxdir[j] ].empty() )
						{
							list<CObj*>::iterator tempiter = m_Area64[ m_idxdir[j] ].begin();
							list<CObj*>::iterator tempiter_end = m_Area64[ m_idxdir[j] ].end();

							for( ; tempiter != tempiter_end; ++tempiter)
							{
								if( ptarget == (*tempiter))
									continue;
								if( pself == (*tempiter))
									continue;

								m_target_unitinfo = (*tempiter)->GetUnitinfo();
								if(MOVE == m_target_unitinfo.state)
									continue;
								if(BURROW == m_target_unitinfo.state)
									continue;
								if( MOVE_AIR == m_target_unitinfo.eMoveType )
									continue;
								if(ORDER_GATHER == m_target_unitinfo.order ||
									ORDER_RETURN_CARGO == m_target_unitinfo.order)
								{
									if( CATEGORY_RESOURCE != (*tempiter)->GetCategory() &&
										CATEGORY_BUILDING != (*tempiter)->GetCategory())
										continue;
								}

								if(MyIntersectrect(&(*tempiter)->GetMyRect(), &temprc))
								{
									bfilter = false;
									break;
								}
							}
							if(!bfilter)
								break;
						}
					}
					if(true == bfilter)
					{
						vpos = temppos[i];
						m_unitrc.left = vpos.x - widthvtx;
						m_unitrc.right = vpos.x + widthvtx;
						m_unitrc.top = vpos.y - heightvtx;
						m_unitrc.bottom = vpos.y + heightvtx;
						return true; //충돌검사까지 다 끝냈다
						//진짜 가도 되는곳이다 
					}
					//break;
				}
			}
		}
	}


	return false;

	//return iflag; 
	//0번이면 주위에 피해갈 유닛이 없다는소리다 가던길로 OBB, COLL 작업 전부 하고 
	/*1번이면 누군가와 접촉은 있었는데 유닛충돌떄문에 못갔다는 소리다
	그냥 가던길로 한번 더 작업해본다,*/
	//2번이면 충돌검사 안해도 된다.
}
bool CArea_Mgr::CollisionCheck_Area(const int& idx, CObj* pself , const CObj* ptarget)
{
	if(idx < 0)
		return false;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

	iter = m_Area64[idx].begin();
	iter_end = m_Area64[idx].end();


	for( ; iter != iter_end; ++iter )
	{
		/*공중이거나 자기 자신이거나(OBJ_ID) 거른다.*/

		if( ptarget == (*iter))
			continue;

		m_target_unitinfo = (*iter)->GetUnitinfo();
		if(MOVE == m_target_unitinfo.state)
			continue;
		if(BURROW == m_target_unitinfo.state)
			continue;
		if( MOVE_AIR == m_target_unitinfo.eMoveType || pself == (*iter) )
			continue;
		if(ORDER_GATHER == m_target_unitinfo.order ||
			ORDER_RETURN_CARGO == m_target_unitinfo.order)
		{
			if( CATEGORY_RESOURCE != (*iter)->GetCategory() &&
				CATEGORY_BUILDING != (*iter)->GetCategory())
				continue;
		}

		//otherrc = (*iter)->GetMyRect();
		if(MyIntersectrect(&((*iter)->GetMyRect()) , &m_unitrc))
			return true;
	}
	return false;
}
bool CArea_Mgr::diamond_check(const int& idx , CObj* pself ,const CObj* ptarget)
{

	if(idx < 0)
		return false;


	D3DXVECTOR2 vtx[5];
	float a[4];
	float b[4];

	list<CObj*>::iterator iter		= m_Area64[idx].begin();
	list<CObj*>::iterator iter_end	= m_Area64[idx].end();

	for( ; iter != iter_end; ++iter)
	{
		if(ptarget == (*iter))
			continue;
		if(pself == (*iter))
			continue;

		m_target_unitinfo = (*iter)->GetUnitinfo();
		if(MOVE == m_target_unitinfo.state)
			continue;
		if(BURROW == m_target_unitinfo.state)
			continue;
		if( MOVE_AIR == m_target_unitinfo.eMoveType )
			continue;
		if(ORDER_GATHER == m_target_unitinfo.order ||
			ORDER_RETURN_CARGO == m_target_unitinfo.order)
		{
			if( CATEGORY_RESOURCE != (*iter)->GetCategory() &&
				CATEGORY_BUILDING != (*iter)->GetCategory())
				continue;
		}

		bool bfilter = false;
		for(int i = 0; i < 4; ++i)
		{
			//직선 4개 만드는 작업
			//b = y1 - ax1
			if( fabsf(m_line[i].fx2 - m_line[i].fx1) <= FLT_EPSILON)
			{
				a[i] = 0;
				b[i] = 0;
				bfilter = true;
				continue;
			}

			a[i] = (m_line[i].fy2 - m_line[i].fy1) / (m_line[i].fx2 - m_line[i].fx1);//fy[i]/fx[i]; 기울기
			b[i] = m_line[i].fy1 - a[i] * m_line[i].fx1;
		}

		//D3DXVECTOR2 vtx = (*iter)->GetPos();
		MYRECT<float> temprc = (*iter)->GetMyRect();

		//꼭지점 설정
		vtx[0] = (*iter)->GetPos();

		vtx[1].x = temprc.left;
		vtx[1].y = temprc.top;

		vtx[2].x = temprc.right;
		vtx[2].y = temprc.top;

		vtx[3].x = temprc.right;
		vtx[3].y = temprc.bottom;

		vtx[4].x = temprc.left;
		vtx[4].y = temprc.bottom;


		for(int i = 0; i < 5; ++i)
		{
			if(vtx[i].x * a[0] + b[0] < vtx[i].y &&
				vtx[i].x * a[1] + b[1] > vtx[i].y &&
				vtx[i].x * a[2] + b[2] > vtx[i].y &&
				vtx[i].x * a[3] + b[3] < vtx[i].y)
			{
				return true;
			}
		}
	}
	return false;
}
bool CArea_Mgr::LineCross(CObj* ptarget)
{
	MYRECT<float>		temprc;
	MYLINE	templine[4];
	temprc = ptarget->GetMyRect();

	templine[0].setpoint(temprc.left , temprc.top , temprc.right , temprc.top);
	templine[1].setpoint(temprc.right , temprc.top , temprc.right , temprc.bottom);
	templine[2].setpoint(temprc.right , temprc.bottom , temprc.left , temprc.bottom);
	templine[3].setpoint(temprc.left , temprc.bottom , temprc.left , temprc.top);

	for(int j = 0; j < 4; ++j)
	{
		if(CMyMath::linecross_check(m_line[m_lineidx] , templine[j]))
			return true;
		if(CMyMath::linecross_check(m_line[m_lineidx+2] , templine[j]))
			return true;
		if(CMyMath::linecross_check(m_line[4] , templine[j]))
			return true;
	}

	return false;
}
bool CArea_Mgr::LineCross(const int& idx , const int& lineidx , CObj* pself ,const CObj* ptarget)
{
	if( !m_Area64[idx].empty() )
	{
		MYRECT<float>		temprc;
		MYLINE	templine[4];
		list<CObj*>::iterator iter		= m_Area64[idx].begin();
		list<CObj*>::iterator iter_end	= m_Area64[idx].end();

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

			temprc = (*iter)->GetMyRect();

			templine[0].setpoint(temprc.left , temprc.top , temprc.right , temprc.top);
			templine[1].setpoint(temprc.right , temprc.top , temprc.right , temprc.bottom);
			templine[2].setpoint(temprc.right , temprc.bottom , temprc.left , temprc.bottom);
			templine[3].setpoint(temprc.left , temprc.bottom , temprc.left , temprc.top);


			for(int j = 0; j < 4; ++j)
			{
				if(CMyMath::linecross_check(m_line[lineidx] , templine[j]))
				{
					return true;
				}
				if(CMyMath::linecross_check(m_line[lineidx+2] , templine[j]))
				{
					return true;
				}
				if(CMyMath::linecross_check(m_line[4] , templine[j]))
				{
					return true;
				}
			}
		}
	}

	return false;
}
void CArea_Mgr::Calculator_eightidx(const int& idx , const int& tilecnt)
{
	m_idx    = idx;

	m_idxdir[CENTER] = m_idx;

	if(m_idx < 0)
		m_idxdir[UP] = -1;
	else
		m_idxdir[UP] = (idx - tilecnt);

	if(m_idx < 0 ||
		m_idx % tilecnt <= 0)
		m_idxdir[LEFT_UP] = -1;
	else
		m_idxdir[LEFT_UP] = idx - 1 - tilecnt;

	if(m_idx < 0 ||
		m_idx % tilecnt >= tilecnt - 1)
		m_idxdir[RIGHT_UP] = -1;
	else
		m_idxdir[RIGHT_UP] = idx + 1 - tilecnt;

	if(m_idx / tilecnt >= tilecnt - 1)
		m_idxdir[DOWN] = -1;
	else
		m_idxdir[DOWN] = idx + tilecnt;

	if(m_idx % tilecnt <= 0)
		m_idxdir[LEFT] = -1;
	else
		m_idxdir[LEFT] = idx - 1;

	if(m_idx % tilecnt >= tilecnt - 1)
		m_idxdir[RIGHT] = -1;
	else
		m_idxdir[RIGHT] = idx + 1;

	if(m_idx / tilecnt >= tilecnt - 1 ||
		m_idx % tilecnt <= 0)
		m_idxdir[LEFT_DOWN] = -1;
	else
		m_idxdir[LEFT_DOWN] = idx - 1 + tilecnt;

	if(m_idx / tilecnt >= tilecnt - 1 ||
		m_idx % tilecnt >= tilecnt - 1)
		m_idxdir[RIGHT_DOWN] = -1;
	else
		m_idxdir[RIGHT_DOWN] = idx + 1 + tilecnt;
}



void CArea_Mgr::Areasize_debugrender(const int& areasize , const int& areacnt)
{
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
				fX = m_areapos64[idx].x - CScrollMgr::m_fScrollX;
				fY = m_areapos64[idx].y - CScrollMgr::m_fScrollY;
				isize = (int)(m_Area64[idx].size());
			}
			CFontMgr::GetInstance()->Setbatch_Font( L"%d" , isize , fX , fY);
		}
	}
}
int CArea_Mgr::overlap_prevention(const MYRECT<float>& rc ,const MYRECT<float>& orirect , const D3DXVECTOR2& vpos , const D3DXVECTOR2& oripos ,CObj* pself)
{
	int idx = CMyMath::Pos_to_index(vpos , 64);
	Calculator_eightidx(idx , 64);


	ORDER eself_order = pself->GetUnitinfo().order;
	//MYRECT<float> otherrc;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;
	float fdot = 0.f;

	//Make_rectline(oripos , vpos , orirect , rc); //prepos로 강제이동하기 때문에 라인체크 안해도된다, 
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
				/*공중이거나 자기 자신이거나(OBJ_ID) 거른다.*/
				m_target_unitinfo = (*iter)->GetUnitinfo();

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

				//otherrc = (*iter)->GetMyRect();
				if(MyIntersectrect(&(*iter)->GetMyRect() , &rc))
				{
					//1번 리턴은 상대가 자리를 비킬 여지가 있을경우
					//2번 리턴은 상대가 비킬여지가 없다 바로 길찾기 재실행
					//3번리턴은 오래기다리면 상대가 비켜준다
					//4
					if(ATTACK == m_target_unitinfo.state)
						return 2;//곧바로 길찾기
					if(TRANSFORMING == m_target_unitinfo.state)
						return 2;//곧바로 길찾기


					D3DXVECTOR2 self_dir = pself->GetcurDir();
					D3DXVECTOR2 target_dir = (*iter)->GetcurDir();

					float fdot = D3DXVec2Dot( &self_dir , &OFFSET_DIRVEC);
					float fdgree = CMyMath::scala_to_dgree(fdot);

					D3DXVECTOR2 vtemp_dir;
					if(fdgree < 45.f)
					{
						//위쪽 방향으로 진행중
						vtemp_dir = D3DXVECTOR2(0 , -1);
					}
					else if( fdgree > 135.f)
					{
						//아래쪽 방향으로 진행중
						vtemp_dir = D3DXVECTOR2(0 , 1);
					}
					else
					{
						if(self_dir.x > 0)
						{
							//오른쪽 방향으로 진행중
							vtemp_dir = D3DXVECTOR2(1 , 0);
						}
						else
						{
							//왼쪽 방향으로 진행중
							vtemp_dir = D3DXVECTOR2(-1 , 0);
						}
					}

					if( D3DXVec2Dot(&target_dir , &vtemp_dir) < 0)
					{
						//90도보다 클때
						if(true == pself->GetUnitinfo().is_wait &&
							true == m_target_unitinfo.is_wait)
						{							
							(*iter)->SetState(IDLE);
							(*iter)->SetWait(true);
							return 2;
						}
						else if(true == pself->GetUnitinfo().is_wait)
							return 3;

						(*iter)->SetState(IDLE);
						(*iter)->SetWait(true);
						return 2;
					}
					else
					{
						D3DXVECTOR3 v3self_dir = D3DXVECTOR3(self_dir.x , self_dir.y , 0);
						D3DXVECTOR3 v3target_dir = D3DXVECTOR3(target_dir.x , target_dir.y , 0);
						D3DXVECTOR3 v3offset = D3DXVECTOR3(vtemp_dir.x , vtemp_dir.y , 0);
						D3DXVECTOR3 vcross1, vcross2;

						D3DXVec3Cross(&vcross1 , &v3offset , &v3self_dir);
						D3DXVec3Cross(&vcross2 , &v3offset , &v3target_dir);

						if( vcross1.z * vcross2.z < 0)
						{
							if(true == pself->GetUnitinfo().is_wait &&
								true == m_target_unitinfo.is_wait)
							{
								(*iter)->SetState(IDLE);
								(*iter)->SetWait(true);
								return 2;
							}
							else if(true == pself->GetUnitinfo().is_wait)
								return 3;

							(*iter)->SetState(IDLE);
							(*iter)->SetWait(true);
							return 2;
						}
						else
						{						

							if(true == m_target_unitinfo.is_wait)					
							{
								(*iter)->SetState(IDLE);
								return 2;
							}

							if(MOVE != m_target_unitinfo.state)
								return 4;

							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
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
CObj* CArea_Mgr::Search_Partbuilding(const int& areaidx , const int& searchidx , OBJID eid)
{
	Calculator_eightidx(areaidx , 64);

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_Area64[ m_idxdir[i] ].empty())
			continue;

		list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
		list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

		for( ; iter != iter_end; ++iter) 
		{
			if( eid == (*iter)->GetOBJNAME() )
			{
				if( searchidx == (*iter)->Getcuridx(32) )
				{
					return (*iter);
				}				
			}

		}
	}

	return NULL;
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


	ATTACK_SEARCH_TYPE search_type = pmyobj->GetUnitinfo().eAttackType;
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

				if( ATTACK_ONLY_AIR == search_type)
				{
					if( MOVE_GROUND == (*iter)->GetUnitinfo().eMoveType)
						continue;
				}
				if( ATTACK_ONLY_GROUND == search_type)
				{
					if( MOVE_AIR == (*iter)->GetUnitinfo().eMoveType)
						continue;
				}

				if(false == random_splash)
				{
					if( pmyobj->GetTeamNumber() == (*iter)->GetTeamNumber() )
						continue;
				}

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

bool CArea_Mgr::OBB_Collision( CObj* pself ,const CObj* ptarget)
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

			other_rc = (*iter)->GetMyRect();
			//otherpos = (*iter)->GetPos();

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

	for(int i = 0; i < 2; ++i)
	{
		vseparate[i] = D3DXVECTOR2(m_line[i].fx2 - m_line[i].fx1 , m_line[i].fy2 - m_line[i].fy1);
		fLength[i] = D3DXVec2Length(&vseparate[i]) / 2;
		D3DXVec2Normalize(&vseparate[i] , &vseparate[i]);
		vproj[i] = vseparate[i] * fLength[i];
	}


	other_rc = ptarget->GetMyRect();
	//otherpos = (*iter)->GetPos();

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
