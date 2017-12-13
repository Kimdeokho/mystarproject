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
#include "ComanderMgr.h"
#include "Building_Preview.h"

IMPLEMENT_SINGLETON(CArea_Mgr)


CArea_Mgr::CArea_Mgr(void)
{

	m_stepsize = 16;

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
}
CObj* CArea_Mgr::AutoSearch_target(CObj* pself , const int& isearch_range , TARGET_SEARCH_TYPE esearchtype)
{
	D3DXVECTOR2 vstart_pos = pself->GetPos();
	D3DXVECTOR2 vpos = pself->GetPos();
	int stepsize = 64;
	int tilecnt = (isearch_range + stepsize)/ stepsize;

	int loopcnt = 2;
	CObj* ptarget = NULL;
	float fminvalue = 4096*4096;

	Target_extract(pself , vpos ,fminvalue , ptarget, isearch_range , esearchtype);

	if(NULL != ptarget)
		return ptarget;

	for(int i = 1; i <= tilecnt; ++i)
	{
		vpos.x = vstart_pos.x - i * 64;
		vpos.y = vstart_pos.y - i * 64;

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

	//*******************************************************************************

	// iattack_range 단위는 픽셀이다.
	//CObj* ptarget = NULL;
	//int stepsize = 64;
	//int tilecnt = (isearch_range + stepsize)/ stepsize;

	//int totalcnt = (tilecnt*2 + 1)*(tilecnt*2 + 1);	
	//
	//int curcnt = 0; //현재 갯수
	//int curidx = pself->Getcuridx(stepsize) + 1;
	//D3DXVECTOR2 startpos = CMyMath::index_to_Pos(curidx , 64 , stepsize);
	//ASTAR_DIR curdir = LEFT;
	//int advancecnt = 1; //한방향으로 움직일 전진 횟수
	//int cornercnt = 0;

	//int stepdir = -1;

	//int inumber = 0;
	//while(curcnt < totalcnt)
	//{	

	//	for(int i = 0; i < advancecnt; ++i)
	//	{			

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

	//		curidx += stepdir;

	//		D3DXVECTOR2 vpos = CMyMath::index_to_Pos(curidx , 64 , 64);
	//		vpos.x -= CScrollMgr::m_fScrollX;
	//		vpos.y -= CScrollMgr::m_fScrollY;
	//		CFontMgr::GetInstance()->Setbatch_Font(L"@%d" , inumber, vpos.x , vpos.y);
	//		++inumber;

	//		//원거리 일경우
	//		if(!m_Area64[curidx].empty())
	//		{
	//			if(OBJ_MEDIC == pself->GetOBJNAME())
	//			{
	//				ptarget = MedicTarget_extract(pself ,curidx, isearch_range , esearchtype);
	//			}
	//			else
	//				ptarget = Target_extract(pself ,curidx, isearch_range , esearchtype);
	//		}		

	//		if(NULL != ptarget)
	//			return ptarget;


	//		++curcnt;
	//		if(curcnt >= totalcnt)
	//			break;
	//	}
	//	
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
	//			stepdir = 64;
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
	//			stepdir = -64;
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

	//return NULL;
}
void CArea_Mgr::Search_Mineral(const int& idx , const int& isearch_range ,CObj* pself, CObj*& pmineral)
{
	CObj* ptarget = NULL;
	int stepsize = 64;
	int tilecnt = (isearch_range + stepsize)/ stepsize;

	int totalcnt = (tilecnt*2 + 1)*(tilecnt*2 + 1);	

	int curcnt = 0; //현재 갯수
	int curidx = idx + 1;
	D3DXVECTOR2 startpos = CMyMath::index_to_Pos(curidx , 64 , stepsize);
	ASTAR_DIR curdir = LEFT;
	int advancecnt = 1; //한방향으로 움직일 전진 횟수
	int cornercnt = 0;

	int stepdir = -1;

	while(curcnt < totalcnt)
	{	

		for(int i = 0; i < advancecnt; ++i)
		{
			curidx += stepdir;

			if(UP == curdir)
				startpos.y -= stepsize;
			else if(DOWN == curdir)
				startpos.y += stepsize;
			else if(LEFT == curdir)
				startpos.x -= stepsize;
			else if(RIGHT == curdir)
				startpos.x += stepsize;

			if( startpos.y < 0 || startpos.y > 4096 ||
				startpos.x < 0 || startpos.x > 4096)
				continue;

			if(true == Mineral_extractor(curidx ,pself, pmineral))
				return;

			++curcnt;
			if(curcnt >= totalcnt)
				break;

		}

		switch(curdir)
		{			
		case UP:
			{
				curdir = RIGHT;
				stepdir = 1;
				break;
			}
		case RIGHT:
			{
				curdir = DOWN;
				stepdir = stepsize;
				break;
			}
		case DOWN:
			{
				curdir = LEFT;
				stepdir = -1;
				break;
			}
		case LEFT:
			{
				curdir = UP;
				stepdir = -stepsize;
				break;
			}
		}

		if(cornercnt >= 2)
		{
			++advancecnt;
			cornercnt = 0;
		}
		++cornercnt;
	}
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

			if( RESOURCE != (*iter)->GetCategory())
				continue;

			if( NULL != ((CMineral*)(*iter))->Getworkman() )
				continue;

			vtargetpos = (*iter)->GetPos();
			fogidx = CMyMath::Pos_to_index( vtargetpos , 32);
			if(false == CTileManager::GetInstance()->GetFogLight(fogidx))
				continue;
			
			pmineral = (*iter);
			((CMineral*)pmineral)->Setworkman(pself);
			((CWorkman*)pself)->SetMineral_mark(pmineral);
			return true;
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

				if( RESOURCE != (*iter)->GetCategory())
					continue;

				
				if( OBJ_GAS != (*iter)->GetOBJNAME() )
					continue;

				if(MyPtInrect( vpt , &((*iter)->GetMyRect()) ))
				{
					pgas = (*iter);
					pgas->SetState(BOARDING);
				}
			}
		}
	}
}
void CArea_Mgr::Target_extract(CObj* pself ,const D3DXVECTOR2& vpos, float& fminvalue, CObj*& ptarget,  const int& isearch_range ,TARGET_SEARCH_TYPE esearchtype)
{
	if(vpos.x < 0 || vpos.x > SQ_TILECNTX*32 ||
		vpos.y < 0 || vpos.y > SQ_TILECNTX*32)
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

		if( RESOURCE == (*iter)->GetCategory())
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

			if(fminvalue > idistance)
			{
				fminvalue = idistance;						
				ptarget = (*iter);
			}
		}
	}
}
CObj* CArea_Mgr::MedicTarget_extract(CObj* pself ,const int& idx, const int& isearch_range ,TARGET_SEARCH_TYPE esearchtype)
{
	CObj* ptarget = NULL;
	float minvalue = 4096*4096;


		//가장 가까운 적을 찾기로 하자.

		float idistance = 0;
		int fogidx = 0;

		list<CObj*>::iterator iter = m_Area64[idx ].begin();
		list<CObj*>::iterator iter_end = m_Area64[ idx ].end();

		ATTACK_SEARCH_TYPE eself_attacktype = pself->GetUnitinfo().eAttackType;
		MOVE_TYPE etarget_movetype;
		OBJID eOBJ_NAME;
		D3DXVECTOR2	vtargetpos;

		for( ; iter != iter_end; ++iter)
		{

			if( RESOURCE == (*iter)->GetCategory())
				continue;

			if(pself == (*iter))
				continue;

			//공중만 공격 가능한 유닛은 공중만 검사하고
			//지상만 공격 가능한 유닛은 지상만 검사한다

			
			etarget_movetype = (*iter)->GetUnitinfo().eMoveType;
			
			if(pself->GetTeamNumber() != (*iter)->GetTeamNumber())
				continue;

			eOBJ_NAME = (*iter)->GetOBJNAME();

			if( OBJ_MARINE != eOBJ_NAME &&
				OBJ_SCV != eOBJ_NAME &&
				OBJ_FIREBAT != eOBJ_NAME &&
				OBJ_MEDIC != eOBJ_NAME)
				continue;
			
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

	return ptarget;
}
void CArea_Mgr::DragCheck( const MYRECT<float>& rc )
{
	//rc == 드래그 사각형

	D3DXVECTOR2 vstartpt = D3DXVECTOR2(rc.left , rc.top);
	D3DXVECTOR2 vendpt = D3DXVECTOR2(rc.right , rc.bottom);;

	int startidx = CMyMath::Pos_to_index(vstartpt , 64);
	int endidx = CMyMath::Pos_to_index(vendpt , 64);

	int colcnt = (endidx/64 - startidx/64) + 1;
	int rowcnt = (endidx%64 - startidx%64) + 1;
	int curidx = 0;

	bool descape = false;
	for(int i = 0; i < colcnt; ++i)
	{
		for(int j = 0; j < rowcnt; ++j)
		{
			curidx = 64 * i + startidx  + j;

			if(DummyCheck( curidx , rc))
			{
				CUnitMgr::GetInstance()->discharge_unit();
				descape = true;
				break;
			}
		}
		if( true == descape)
			break;
	}

	for(int i = 0; i < colcnt; ++i)
	{
		for(int j = 0; j < rowcnt; ++j)
		{
			curidx = 64 * i + startidx  + j;

			Choice_unit( curidx , rc);
		}
	}
}

void CArea_Mgr::SelectCheck(const int& idx , const D3DXVECTOR2& vpos)
{
	//드래그가 아닌 기본 클릭 선택에대한 함수
	Calculator_eightidx(idx , 64);


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

	ORDER eself_order = pself->GetUnitinfo().eorder;
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
				other_order = (*iter)->GetUnitinfo().eorder;

				if(ORDER_GATHER == eself_order ||
					ORDER_RETURN_CARGO == eself_order)
				{
					//자원채취중일땐 자원과 건물만 충돌한다
					if( RESOURCE != (*iter)->GetCategory() &&
						BUILDING != (*iter)->GetCategory())
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

				if(ORDER_BUILDING == eself_order)
					continue;
				//if( BOARDING == (*iter)->GetUnitinfo().estate)
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


	//MOVE_TYPE selfmovetype = pself->GetUnitinfo().eMoveType;
	//MOVE_TYPE othermovetype;

	//float	fwidth = 0;
	//float	fheight = 0;
	//bool	bescape = false;
	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(m_idxdir[i] < 0)
	//		continue;

	//	if( !m_Area64[m_idxdir[i]].empty() )
	//	{
	//		MYRECT<float> temp;
	//		list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
	//		list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

	//		for( ; iter != iter_end; ++iter)
	//		{
	//			if((*iter) == pself)
	//				continue;				

	//			othermovetype = (*iter)->GetUnitinfo().eMoveType;

	//			if( MOVE_GROUND == selfmovetype)
	//			{
	//				if(MOVE_AIR == othermovetype)
	//					continue;
	//			}
	//			else if(MOVE_AIR == selfmovetype)
	//			{
	//				if(MOVE_GROUND == othermovetype)
	//					continue;
	//			}

	//			if(MyIntersectrect( &temp , &myrc , &(*iter)->GetMyRect() ))
	//			{
	//				fwidth = temp.right - temp.left;
	//				fheight = temp.bottom - temp.top;
	//				if( fwidth > fheight )
	//				{
	//					//세로로 민다
	//					if( vpos.y  > (*iter)->GetPos().y)
	//					{
	//						//부딪힌상대보다 아래에 있다면
	//						vpos.y += fheight;
	//						myrc.bottom += fheight;
	//						myrc.top += fheight;
	//					}
	//					else
	//					{
	//						vpos.y -= fheight;
	//						myrc.bottom -= fheight;
	//						myrc.top -= fheight;
	//					}
	//				}
	//				else
	//				{
	//					//가로로 민다
	//					if( vpos.x  > (*iter)->GetPos().x)
	//					{
	//						//부딪힌상대보다 오른쪽에 있다면
	//						vpos.x += fwidth;
	//						myrc.left += fwidth;
	//						myrc.right += fwidth;
	//					}
	//					else
	//					{
	//						vpos.x -= fwidth;
	//						myrc.left -= fwidth;
	//						myrc.right -= fwidth;
	//					}
	//				}
	//				bescape = true;
	//				break;
	//			}
	//		}
	//	}

	//	if(true == bescape)
	//		break;
	//}


	//if(true == bescape)
	//{
	//	for(int i = 0; i < ASTAR_DIR_END; ++i)
	//	{
	//		if( !m_Area64[m_idxdir[i]].empty() )
	//		{
	//			MYRECT<float> temp;
	//			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
	//			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

	//			for( ; iter != iter_end; ++iter)
	//			{
	//				if((*iter) == pself)
	//					continue;				

	//				othermovetype = (*iter)->GetUnitinfo().eMoveType;

	//				if( MOVE_GROUND == selfmovetype)
	//				{
	//					if(MOVE_AIR == othermovetype)
	//						continue;
	//				}
	//				else if(MOVE_AIR == selfmovetype)
	//				{
	//					if(MOVE_GROUND == othermovetype)
	//						continue;
	//				}

	//				if(MyIntersectrect( &temp , &myrc , &(*iter)->GetMyRect() ))
	//				{
	//					return false;
	//				}
	//			}
	//		}
	//	}
	//	return true;
	//}
	//else
	//	return false;

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
				if( RESOURCE == (*iter)->GetCategory())
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
	//CUI* preview = CComanderMgr::GetInstance()->GetPreview();
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
				{
					(*iter)->SetSelect(GENERAL_SELECT , D3DCOLOR_ARGB(255,255,255,255));
				}
				else
				{
					(*iter)->SetSelect(GENERAL_SELECT);
				}
				
				CUnitMgr::GetInstance()->SetUnit((*iter));

				return true;
				/*아군만 선택*/
			}
		}
	}

	return false;
}
void CArea_Mgr::Choice_unit(const int& idx, const MYRECT<float>& rc)
{
	//다수 유닛 선택할때
	if( !m_Area64[idx].empty() )
	{
		MYRECT<float> temprc;

		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(MyIntersectrect(&temprc , &(*iter)->GetMyRect() , &rc ))
			{
				if(UNIT == (*iter)->GetCategory())
				{
					(*iter)->SetSelect(GENERAL_SELECT);
					CUnitMgr::GetInstance()->SetUnit((*iter));				
				}
				/*아군만 선택*/
			}
		}

		//if(CUnitMgr::GetInstance()->GetUnitlistempty())
		//{
		//	iter = m_Area64[idx].begin();
		//	iter_end = m_Area64[idx].end();

		//	for( ; iter != iter_end; ++iter)
		//	{
		//		if(MyIntersectrect(&temprc , &(*iter)->GetMyRect() , &rc ))
		//		{
		//			if(BUILDING == (*iter)->GetCategory())
		//			{
		//				(*iter)->SetSelect(GENERAL_SELECT);
		//				CUnitMgr::GetInstance()->SetUnit((*iter));
		//			}
		//		}
		//	}
		//}
	}
}
void CArea_Mgr::TargetChoice(const D3DXVECTOR2& vpos)
{

	m_pchoice_target = NULL;
	int idx = CMyMath::Pos_to_index(vpos , 64);
	Calculator_eightidx(idx , 64);

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
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

bool CArea_Mgr::Check_Area(const MYRECT<float>& rc , D3DXVECTOR2& vpos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos , const int& stepsize , CObj* pself ,const CObj* ptarget)
{

	m_stepsize = stepsize;

	m_checkidx = CMyMath::Pos_to_index(vpos , m_stepsize);
	int idx = CMyMath::Pos_to_index(vpos , 64);

	Calculator_eightidx(idx , 64);

	m_unitrc = rc;//탐지할 노드의 렉트
	D3DXVECTOR2 oripos = vcenterpos;
	MYRECT<float> orirect = center_rc;

	bool bcheck = true;
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_Area64[ m_idxdir[i] ].empty())
			continue;

		if(true == PathFind_Area(m_idxdir[i] , vpos , pself , ptarget))
		{
			bcheck = false;
			break;
		}
	}

	if(m_checkidx !=  CMyMath::Pos_to_index(vpos , m_stepsize))
		return false;


	if(true == bcheck)
	{
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(m_Area64[ m_idxdir[i] ].empty())
				continue;

			if(CollisionCheck_Area(m_idxdir[i] , pself , ptarget))
				return false;
		}
	}


	float fx = oripos.x - vpos.x;
	float fy = oripos.y - vpos.y;

	//0번과 2번이 교차 할 라인이다 0번은 1시 방향

	int lineidx;
	if(fx < 0)
	{
		//센터보다 오른쪽에 있다.
		if(fy < 0)
		{
			//센터보다 아래에 있다.
			//0,2

			lineidx = 0;
			m_line[0].setpoint(orirect.right , orirect.top , m_unitrc.right , m_unitrc.top);
			m_line[1].setpoint(m_unitrc.right , m_unitrc.top , m_unitrc.left, m_unitrc.bottom);
			m_line[2].setpoint(m_unitrc.left , m_unitrc.bottom , orirect.left, orirect.bottom);
			m_line[3].setpoint(orirect.left , orirect.bottom , orirect.right, orirect.top);

		}
		else
		{
			//센터보다 위에 있다.
			//1 , 3
			lineidx = 1;
			m_line[0].setpoint(m_unitrc.left , m_unitrc.top , m_unitrc.right, m_unitrc.bottom);
			m_line[1].setpoint(m_unitrc.right , m_unitrc.bottom , orirect.right, orirect.bottom);
			m_line[2].setpoint(orirect.right , orirect.bottom , orirect.left, orirect.top);
			m_line[3].setpoint(orirect.left , orirect.top , m_unitrc.left, m_unitrc.top);
		}
	}
	else if(0 == fx)
	{
		if(fy < 0)
		{
			//센터보다 아래에 있다.
			lineidx = 1;
			m_line[0].setpoint(orirect.left , orirect.bottom , orirect.right, orirect.bottom);
			m_line[1].setpoint(orirect.right , orirect.bottom , m_unitrc.right, m_unitrc.top);
			m_line[2].setpoint(m_unitrc.right , m_unitrc.top , m_unitrc.left, m_unitrc.top);
			m_line[3].setpoint(m_unitrc.left , m_unitrc.top , orirect.left, orirect.bottom);
		}
		else
		{
			//센터보다 위에 있다.
			lineidx = 1;
			m_line[0].setpoint(m_unitrc.left , m_unitrc.bottom , m_unitrc.right, m_unitrc.bottom);
			m_line[1].setpoint(m_unitrc.right , m_unitrc.bottom , orirect.right, orirect.top);
			m_line[2].setpoint(orirect.right , orirect.top , orirect.left, orirect.top);
			m_line[3].setpoint(orirect.left , orirect.top , m_unitrc.left, m_unitrc.bottom);
		}
	}
	else
	{
		//센터보다 왼쪽에 있다.
		if(fy < 0)
		{
			//센터보다 아래에 있다.

			//1,3
			lineidx = 1;
			m_line[0].setpoint(orirect.left , orirect.top , orirect.right, orirect.bottom);
			m_line[1].setpoint(orirect.right , orirect.bottom , m_unitrc.right, m_unitrc.bottom);
			m_line[2].setpoint(m_unitrc.right , m_unitrc.bottom , m_unitrc.left, m_unitrc.top);
			m_line[3].setpoint(m_unitrc.left , m_unitrc.top , orirect.left, orirect.top);
		}
		else
		{
			//센터보다 위에 있다.
			//0,2
			lineidx = 0;
			m_line[0].setpoint(m_unitrc.right , m_unitrc.top , orirect.right, orirect.top);
			m_line[1].setpoint(orirect.right , orirect.top , orirect.left, orirect.bottom);
			m_line[2].setpoint(orirect.left , orirect.bottom , m_unitrc.left, m_unitrc.bottom);
			m_line[3].setpoint(m_unitrc.left , m_unitrc.bottom , m_unitrc.right, m_unitrc.top);
		}
	}

	////step32일땐 필요할듯 싶다..
	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(LineCross(m_idxdir[i] , lineidx , pself, true))
	//		return false;
	//}

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_Area64[ m_idxdir[i] ].empty())
			continue;

		if(diamond_check( m_idxdir[i] , pself ,ptarget))
			return false;
	}
	return true;
}
bool CArea_Mgr::PathFind_Area(const int& idx ,D3DXVECTOR2& vpos , CObj* pself , const CObj* ptarget)
{
	//길찾기를할때 유닛을 피해서 이동하기 위함 함수이름 바꿀필요 있음

	if(idx < 0)
		return false;

	float	fwidth = 0.f;
	float	fheight = 0.f;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

	iter = m_Area64[idx].begin();
	iter_end = m_Area64[idx].end();

	MYRECT<float> out_rect;
	MYRECT<float> otherrc;
	MYRECT<float> temprc;

	D3DXVECTOR2 temppos[4];

	float widthvtx = (m_unitrc.right - m_unitrc.left)/2;
	float heightvtx = (m_unitrc.bottom - m_unitrc.top)/2;
	for( ; iter != iter_end; ++iter )
	{
		/*같은그룹, 공중이거나 자기 자신이거나(OBJ_ID) 움직이고 있는 상대는 거른다.*/

		if(MOVE == (*iter)->GetUnitinfo().estate)
			continue;
		if( MOVE_AIR == (*iter)->GetUnitinfo().eMoveType || pself == (*iter) )
			continue;
		if(ORDER_GATHER == pself->GetUnitinfo().eorder ||
			ORDER_RETURN_CARGO == pself->GetUnitinfo().eorder)
		{
			if( RESOURCE != (*iter)->GetCategory() &&
				BUILDING != (*iter)->GetCategory())
				continue;
		}
		if( ptarget == (*iter))
			continue;


		//피해갈 꼭지점을 만든다.


		otherrc = (*iter)->GetMyRect();
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



		for(int i = 0; i < 4; ++i)
		{
			//꼭지점을 순회한다.
			if(CMyMath::Pos_to_index(temppos[i] , m_stepsize) ==
				CMyMath::Pos_to_index(vpos , m_stepsize) )
			{
				temprc.left = temppos[i].x - widthvtx;
				temprc.right = temppos[i].x + widthvtx;
				temprc.top = temppos[i].y - heightvtx;
				temprc.bottom = temppos[i].y + heightvtx;

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
							if(MOVE == (*tempiter)->GetUnitinfo().estate)
								continue;
							else if( MOVE_AIR == (*tempiter)->GetUnitinfo().eMoveType || pself == (*tempiter))
								continue;

							if(MyIntersectrect(&out_rect , &(*tempiter)->GetMyRect(), &temprc))
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
					//break; //32스텝으로 할땐 여기로 해야할듯싶다..

					return true;
				}
				//break; //16스텝
			}
		}
	}
	return false;
}
bool CArea_Mgr::CollisionCheck_Area(const int& idx, CObj* pself , const CObj* ptarget)
{
	if(idx < 0)
		return false;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

	iter = m_Area64[idx].begin();
	iter_end = m_Area64[idx].end();

	MYRECT<float> out_rect;

	for( ; iter != iter_end; ++iter )
	{
		/*공중이거나 자기 자신이거나(OBJ_ID) 거른다.*/


		if( MOVE == (*iter)->GetUnitinfo().estate)
			continue;
		if( MOVE_AIR == (*iter)->GetUnitinfo().eMoveType || pself == (*iter) )
			continue;
		if(ORDER_GATHER == pself->GetUnitinfo().eorder ||
			ORDER_RETURN_CARGO == pself->GetUnitinfo().eorder)
		{
			if( RESOURCE != (*iter)->GetCategory() &&
				BUILDING != (*iter)->GetCategory())
				continue;
		}
		if( ptarget == (*iter))
			continue;

		//otherrc = (*iter)->GetMyRect();
		if(MyIntersectrect(&out_rect , &((*iter)->GetMyRect()) , &m_unitrc))
			return true;
	}
	return false;
}
bool CArea_Mgr::diamond_check(const int& idx , CObj* pself ,const CObj* ptarget)
{

	if(idx < 0)
		return false;


	D3DXVECTOR2 vtx[4];
	float a[4];
	float b[4];

	list<CObj*>::iterator iter		= m_Area64[idx].begin();
	list<CObj*>::iterator iter_end	= m_Area64[idx].end();

	for( ; iter != iter_end; ++iter)
	{

		if( MOVE == (*iter)->GetUnitinfo().estate )
			continue;
		if( MOVE_AIR == (*iter)->GetUnitinfo().eMoveType || pself == (*iter) )
			continue;

		if(ORDER_GATHER == pself->GetUnitinfo().eorder ||
			ORDER_RETURN_CARGO == pself->GetUnitinfo().eorder)
		{
			if( RESOURCE != (*iter)->GetCategory() &&
				BUILDING != (*iter)->GetCategory())
				continue;
		}
		if(ptarget == (*iter))
			continue;

		bool bfilter = false;
		for(int i = 0; i < 4; ++i)
		{
			//직선 4개 만드는 작업
			//b = y1 - ax1
			if( 0 == m_line[i].fx2 - m_line[i].fx1 )
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
		vtx[0].x = temprc.left;
		vtx[0].y = temprc.top;

		vtx[1].x = temprc.right;
		vtx[1].y = temprc.top;

		vtx[2].x = temprc.right;
		vtx[2].y = temprc.bottom;

		vtx[3].x = temprc.left;
		vtx[3].y = temprc.bottom;

		for(int i = 0; i < 4; ++i)
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
bool CArea_Mgr::LineCross(const int& idx , const int& lineidx , CObj* pself , bool movecheck)
{
	if(idx < 0)
		return false;

	if( !m_Area64[idx].empty() )
	{
		MYRECT<float>		temprc;
		MYLINE	templine[4];
		list<CObj*>::iterator iter		= m_Area64[idx].begin();
		list<CObj*>::iterator iter_end	= m_Area64[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(true == movecheck/*움직이는 대상을 체크할꺼냐*/ && 
				MOVE == (*iter)->GetUnitinfo().estate )
				continue;

			if( MOVE_AIR == (*iter)->GetUnitinfo().eMoveType || pself == (*iter) )
				continue;

			if(ORDER_GATHER == pself->GetUnitinfo().eorder ||
				ORDER_RETURN_CARGO == pself->GetUnitinfo().eorder)
			{
				if( RESOURCE != (*iter)->GetCategory() &&
					BUILDING != (*iter)->GetCategory())
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
					return true;
			}
			for(int j = 0; j < 4; ++j)
			{
				if(CMyMath::linecross_check(m_line[lineidx + 2] , templine[j]))
					return true;
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
bool CArea_Mgr::overlap_prevention(const MYRECT<float>& rc ,const MYRECT<float>& orirect , const D3DXVECTOR2& vpos , const D3DXVECTOR2& oripos ,CObj* pself)
{
	int idx = CMyMath::Pos_to_index(vpos , 64);
	Calculator_eightidx(idx , 64);


	ORDER eself_order = pself->GetUnitinfo().eorder;
	//MYRECT<float> otherrc;
	MYRECT<float> outrect;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

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
				if( MOVE_AIR == (*iter)->GetUnitinfo().eMoveType || pself == (*iter) )
					continue;

				if(ORDER_GATHER == eself_order ||
					ORDER_RETURN_CARGO == eself_order)
				{
					if( RESOURCE != (*iter)->GetCategory() &&
						BUILDING != (*iter)->GetCategory())
						continue;
				}

				//otherrc = (*iter)->GetMyRect();
				if(MyIntersectrect(&outrect , &(*iter)->GetMyRect() , &rc))
				{
					return true;
				}
			}
		}
	}


	float fx = oripos.x - vpos.x;
	float fy = oripos.y - vpos.y;

	//0번과 2번이 교차 할 라인이다 0번은 1시 방향

	int lineidx;
	if(fx < 0)
	{
		//센터보다 오른쪽에 있다.
		if(fy < 0)
		{
			//센터보다 아래에 있다.
			//0,2

			lineidx = 0;
			m_line[0].setpoint(orirect.right , orirect.top , rc.right , rc.top);
			m_line[1].setpoint(rc.right , rc.top , rc.left, rc.bottom);
			m_line[2].setpoint(rc.left , rc.bottom , orirect.left, orirect.bottom);
			m_line[3].setpoint(orirect.left , orirect.bottom , orirect.right, orirect.top);

		}
		else
		{
			//센터보다 위에 있다.
			//1 , 3
			lineidx = 1;
			m_line[0].setpoint(rc.left , rc.top , rc.right, rc.bottom);
			m_line[1].setpoint(rc.right , rc.bottom , orirect.right, orirect.bottom);
			m_line[2].setpoint(orirect.right , orirect.bottom , orirect.left, orirect.top);
			m_line[3].setpoint(orirect.left , orirect.top , rc.left, rc.top);
		}
	}
	else
	{
		//센터보다 왼쪽에 있다.
		if(fy < 0)
		{
			//센터보다 아래에 있다.

			//1,3
			lineidx = 1;
			m_line[0].setpoint(orirect.left , orirect.top , orirect.right, orirect.bottom);
			m_line[1].setpoint(orirect.right , orirect.bottom , rc.right, rc.bottom);
			m_line[2].setpoint(rc.right , rc.bottom , rc.left, rc.top);
			m_line[3].setpoint(rc.left , rc.top , orirect.left, orirect.top);
		}
		else
		{
			//센터보다 위에 있다.
			//0,2
			lineidx = 0;
			m_line[0].setpoint(rc.right , rc.top , orirect.right, orirect.top);
			m_line[1].setpoint(orirect.right , orirect.top , orirect.left, orirect.bottom);
			m_line[2].setpoint(orirect.left , orirect.bottom , rc.left, rc.bottom);
			m_line[3].setpoint(rc.left , rc.bottom , rc.right, rc.top);
		}
	}

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(LineCross(m_idxdir[i] , lineidx , pself  ,false ))
			return true;
	}

	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(diamond_check(m_idxdir[i] , objid))
	//		return true;
	//}

	return false;

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

void CArea_Mgr::Setsplash_damage(CObj* pmyobj ,const WEAPON_INFO& weaponinfo, const D3DXVECTOR2& vsplash_pos, const float& range1 , const float& range2, const float& range3 , bool random_splash)
{
	D3DXVECTOR2 start_pos = D3DXVECTOR2(vsplash_pos.x - range3 , vsplash_pos.y - range3);
	D3DXVECTOR2 end_pos = D3DXVECTOR2(vsplash_pos.x + range3 , vsplash_pos.y + range3); 

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

	int damage = weaponinfo.damage;
	DAMAGE_TYPE edamage_type = weaponinfo.eDamageType;

	D3DXVECTOR2	vtarget_pos;
	float fdistance = 0.f;

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
				if(false == random_splash)
				{
					if( pmyobj->GetTeamNumber() == (*iter)->GetTeamNumber() )
						continue;
				}

				vtarget_pos = (*iter)->GetPos();
				fdistance = CMyMath::pos_distance( vsplash_pos , vtarget_pos);

				if(fdistance <= range3*range3*2)
				{
					if(fdistance <= range2*range2*2)
					{
						if(fdistance <= range1*range1*2)
						{
							(*iter)->SetDamage(damage , edamage_type);
							continue;
						}
						(*iter)->SetDamage(damage/2 , edamage_type);
						continue;
					}
					(*iter)->SetDamage(damage/4 , edamage_type);
					continue;
				}
			}
		}
	}
}
