#include "StdAfx.h"
#include "Area_Mgr.h"
#include "Obj.h"
#include "Unit.h"

#include "MyMath.h"
#include "UnitMgr.h"
#include "MouseMgr.h"
#include "ScrollMgr.h"
#include "FontMgr.h"
#include "TileManager.h"

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

	m_vecmoveobj.reserve(400);
}

CArea_Mgr::~CArea_Mgr(void)
{
	for(int i = 0; i < 64*64; ++i)
	{
		if(!m_Area64[i].empty())
			m_Area64[i].clear();
	}

	for(int i = 0; i < 16*16; ++i)
	{
		if(!m_Area256[i].empty())
			m_Area256[i].clear();
	}

	for(int i = 0; i < 8*8; ++i)
	{
		if(!m_Area512[i].empty())
			m_Area512[i].clear();
	}
}
CObj* CArea_Mgr::AutoSearch_target(CObj* pobj , const int& idx, const int& isearch_range , const int& areasize , TARGET_SEARCH_TYPE etargettype)
{
	//isearch_range는 픽셀단위이다.

	if(256 == areasize)
		Calculator_eightidx(idx , 16);
	else if(512 == areasize)
		Calculator_eightidx(idx , 8);

	CObj* ptarget = NULL;
	float iminvalue = 1000000000;
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if( 256 == areasize)
			Target_extract256(pobj , ptarget , m_idxdir[i] , isearch_range , iminvalue ,etargettype);
		else if( 512 == areasize)
			Target_extract512(pobj , ptarget , m_idxdir[i] , isearch_range , iminvalue ,etargettype);
	}

	return ptarget;
}
void CArea_Mgr::Target_extract512(CObj* pself ,CObj*& ptarget , const int& idx , const int& irange, float& minvalue ,TARGET_SEARCH_TYPE esearchtype)
{
	//유닛 탐색범위에 있는 다른 유닛들(적)을 검사한다

	//유닛에따라 area512로 할지 256으로 할지 정해야할듯 싶다..


	if(!m_Area512[idx].empty())
	{
		//가장 가까운 적을 찾기로 하자.
		
		float idistance = 0;
		int fogidx = 0;

		list<CObj*>::iterator iter = m_Area512[idx].begin();
		list<CObj*>::iterator iter_end = m_Area512[idx].end();

		for( ; iter != iter_end; ++iter)
		{

			//공중만 공격 가능한 유닛은 공중만 검사하고
			//지상만 공격 가능한 유닛은 지상만 검사한다
			if( SEARCH_ONLY_ENEMY == esearchtype)
			{
				if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
					continue;
			}
			else if(SEARCH_ONLY_ALLY == esearchtype)
			{
				if(pself->GetTeamNumber() != (*iter)->GetTeamNumber())
					continue;
			}
			else
			{

			}
			if(pself != (*iter))
			{
				fogidx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
				if(false == CTileManager::GetInstance()->GetFogLight(fogidx))
					continue;

				idistance = CMyMath::pos_distance(pself->GetPos() , (*iter)->GetPos());


				//지금 찾은 유닛과의 거리가 최소거리보다 작다면

				if(idistance <= irange*irange)
				{
					//해당 유닛의 탐지범위에 적합한지 본다.
					//목표를 찾았다.					

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

void CArea_Mgr::Target_extract256(CObj* pself ,CObj*& ptarget, const int& idx , const int& irange, float& minvalue ,TARGET_SEARCH_TYPE esearchtype)
{
	//유닛 탐색범위에 있는 다른 유닛들(적)을 검사한다

	//유닛에따라 area512로 할지 256으로 할지 정해야할듯 싶다..


	if(!m_Area256[idx].empty())
	{
		//가장 가까운 적을 찾기로 하자.
		float idistance = 0;
		int fogidx = 0;

		list<CObj*>::iterator iter = m_Area256[idx].begin();
		list<CObj*>::iterator iter_end = m_Area256[idx].end();

		for( ; iter != iter_end; ++iter)
		{

			//공중만 공격 가능한 유닛은 공중만 검사하고
			//지상만 공격 가능한 유닛은 지상만 검사한다
			if( SEARCH_ONLY_ENEMY == esearchtype)
			{
				if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
					continue;
			}
			else if(SEARCH_ONLY_ALLY == esearchtype)
			{
				if(pself->GetTeamNumber() != (*iter)->GetTeamNumber())
					continue;
			}
			else
			{

			}
			if(pself != (*iter))
			{
				fogidx = CMyMath::Pos_to_index((*iter)->GetPos() , 32);
				if(false == CTileManager::GetInstance()->GetFogLight(fogidx))
					continue;

				idistance = CMyMath::pos_distance(pself->GetPos() , (*iter)->GetPos());


				//지금 찾은 유닛과의 거리가 최소거리보다 작다면

				if(idistance <= irange*irange)
				{
					//해당 유닛의 탐지범위에 적합한지 본다.
					//목표를 찾았다.					

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
void CArea_Mgr::DragCheck(const int& idx , const MYRECT<float>& rc )
{
	//rc == 드래그 사각형
	Calculator_eightidx(idx , 8);


	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(DummyCheck(m_idxdir[i] , rc))
		{
			CUnitMgr::GetInstance()->discharge_unit();
			break;
		}
	}


	//주위 8방향을 검사하여 유닛을 선택한다

	for(int i = 0; i < ASTAR_DIR_END; ++i)
		Choice_unit(m_idxdir[i] , rc);

}
bool CArea_Mgr::Collision_check(const int& idx,CObj* pself, CObj*& ptarget)
{
	Calculator_eightidx(idx , 64);

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if(Collision_unit(m_idxdir[i] , pself , ptarget))
			return true;
	}

	return false;
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
bool CArea_Mgr::Collision_unit(const int& idx ,CObj* pself, CObj*& ptarget)
{
	//유닛끼리의 겹침현상이 일어날때

	if( !m_Area64[idx].empty() )
	{
		MYRECT<float> temp;
		list<CObj*>::iterator iter = m_Area64[idx].begin();
		list<CObj*>::iterator iter_end = m_Area64[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if((*iter) != pself)
			{
				if(MyIntersectrect( &temp , &pself->GetMyRect() , &(*iter)->GetMyRect() ))
				{
					ptarget = (*iter);
					return true;
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

		for( ; iter != iter_end; ++iter)
		{
			if(MyPtInrect( vpos , &(*iter)->GetMyRect() ))
			{
				CUnitMgr::GetInstance()->discharge_unit();

				(*iter)->SetSelect(true);
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
	if( !m_Area512[idx].empty() )
	{
		MYRECT<float> temprc;

		list<CObj*>::iterator iter = m_Area512[idx].begin();
		list<CObj*>::iterator iter_end = m_Area512[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(MyIntersectrect(&temprc , &(*iter)->GetMyRect() , &rc ))
			{
				if(UNIT == (*iter)->GetKategorie())
				{
					(*iter)->SetSelect(true);
					CUnitMgr::GetInstance()->SetUnit((*iter));				
				}
				/*아군만 선택*/
			}
		}

		if(CUnitMgr::GetInstance()->GetUnitlistempty())
		{
			iter = m_Area512[idx].begin();
			iter_end = m_Area512[idx].end();

			for( ; iter != iter_end; ++iter)
			{
				if(MyIntersectrect(&temprc , &(*iter)->GetMyRect() , &rc ))
				{
					if(BUILDING == (*iter)->GetKategorie())
					{
						(*iter)->SetSelect(true);
						CUnitMgr::GetInstance()->SetUnit((*iter));				
					}
				}
			}
		}
	}
}
bool CArea_Mgr::DummyCheck(const int& idx, const MYRECT<float>& rc)
{
	if( !m_Area512[idx].empty() )
	{
		MYRECT<float> temprc;

		list<CObj*>::iterator iter = m_Area512[idx].begin();
		list<CObj*>::iterator iter_end = m_Area512[idx].end();

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

void CArea_Mgr::SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj)
{
	if(!m_Area64[oldidx].empty())
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
void CArea_Mgr::SetObj_Area256(const int& curidx , const int& oldidx , CObj* pobj)
{
	if(!m_Area256[oldidx].empty())
	{
		list<CObj*>::iterator iter = m_Area256[oldidx].begin();
		list<CObj*>::iterator iter_end = m_Area256[oldidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area256[oldidx].erase(iter);
				break;
			}
			else
				++iter;
		}
	}

	m_Area256[curidx].push_back(pobj);
}
void CArea_Mgr::SetObj_Area512(const int& curidx , const int& oldidx , CObj* pobj)
{
	if(!m_Area512[oldidx].empty())
	{
		list<CObj*>::iterator iter = m_Area512[oldidx].begin();
		list<CObj*>::iterator iter_end = m_Area512[oldidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area512[oldidx].erase(iter);
				break;
			}
			else
				++iter;
		}
	}

	m_Area512[curidx].push_back(pobj);
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
void CArea_Mgr::ReleaseObj_Area256(const int& curidx , CObj* pobj)
{
	if(!m_Area256[curidx].empty())
	{
		list<CObj*>::iterator iter = m_Area256[curidx].begin();
		list<CObj*>::iterator iter_end = m_Area256[curidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area256[curidx].erase(iter);
				break;
			}
			else
				++iter;//이걸 안해준게 문제였다 이걸 안해줘서 무한루프 돌았어...
		}
	}
}
void CArea_Mgr::ReleaseObj_Area512(const int& curidx , CObj* pobj)
{
	if(!m_Area512[curidx].empty())
	{
		list<CObj*>::iterator iter = m_Area512[curidx].begin();
		list<CObj*>::iterator iter_end = m_Area512[curidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area512[curidx].erase(iter);
				break;
			}
			else
				++iter;//이걸 안해준게 문제였다 이걸 안해줘서 무한루프 돌았어...
		}
	}
}
bool CArea_Mgr::Collision_Area64Check(const int& idx, const MYRECT<float>& myrc , const int& objid)
{
	if(idx < 0)
		return false;

	if( !m_Area64[idx].empty() )
	{
		list<CObj*>::iterator iter;
		list<CObj*>::iterator iter_end;

		iter = m_Area64[idx].begin();
		iter_end = m_Area64[idx].end();

		MYRECT<float> otherrc;
		MYRECT<float> out_rect;

		for( ; iter != iter_end; ++iter )
		{
			/*공중이거나 자기 자신이거나(OBJ_ID) 거른다.*/

			if( MOVE == (*iter)->GetState())
				continue;

			if( MOVE_AIR == (*iter)->GetType() || objid == (*iter)->GetObjID() )
				continue;

			otherrc = (*iter)->GetMyRect();
			if(MyIntersectrect(&out_rect , &otherrc , &myrc))
				return true;
		}
	}
	return false;
}
void CArea_Mgr::Collision_Area64(const int& idx ,D3DXVECTOR2& vpos , const int& objid)
{
	//길찾기를할때 유닛을 피해서 이동하기 위함 함수이름 바꿀필요 있음

	if(idx < 0)
		return;

	if( !m_Area64[idx].empty() )
	{

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
		for( ; iter != iter_end; ++iter )
		{
			/*같은그룹, 공중이거나 자기 자신이거나(OBJ_ID) 움직이고 있는 상대는 거른다.*/

			if(MOVE == (*iter)->GetState())
				continue;

			if( MOVE_AIR == (*iter)->GetType() || objid == (*iter)->GetObjID())
				continue;

			//피해갈 꼭지점을 만든다.
			otherrc = (*iter)->GetMyRect();

			float widthvtx = (m_unitrc.right - m_unitrc.left)/2;
			float heightvtx = (m_unitrc.bottom - m_unitrc.top)/2;
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
						if( !m_Area64[ m_idxdir[j] ].empty() )
						{
							list<CObj*>::iterator tempiter = m_Area64[ m_idxdir[j] ].begin();
							list<CObj*>::iterator tempiter_end = m_Area64[ m_idxdir[j] ].end();

							for( ; tempiter != tempiter_end; ++tempiter)
							{
								if(MOVE == (*tempiter)->GetState())
									continue;

								if( MOVE_AIR == (*tempiter)->GetType() || objid == (*tempiter)->GetObjID())
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
						break; //32스텝으로 할땐 여기로 해야할듯싶다..
					}
					//break; //16스텝
				}
			}
		}
	}
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
bool CArea_Mgr::empty_area64(void)
{
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(!m_Area64[ m_idxdir[i] ].empty())
			return false;
	}
	return true;
}
bool CArea_Mgr::Check_Area(const MYRECT<float>& rc , D3DXVECTOR2& vpos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos , const int& stepsize , const int& objid , ASTAR_DIR edirflag)
{
	m_stepsize = stepsize;

	m_checkidx = CMyMath::Pos_to_index(vpos , m_stepsize);
	int idx = CMyMath::Pos_to_index(vpos , 64);

	Calculator_eightidx(idx , 64);

	m_unitrc = rc;
	D3DXVECTOR2 oripos = vcenterpos;
	MYRECT<float> orirect = center_rc;

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		Collision_Area64(m_idxdir[i] , vpos , objid);
	}

	if(m_checkidx !=  CMyMath::Pos_to_index(vpos , m_stepsize))
		return false;


	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(Collision_Area64Check(m_idxdir[i] , m_unitrc , objid))
			return false;
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

	//step32일땐 필요할듯 싶다..
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(LineCross(m_idxdir[i] , lineidx , objid , true))
			return false;
	}

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(diamond_check(m_idxdir[i] , objid ))
			return false;
	}
	return true;
}
bool CArea_Mgr::LineCross(const int& idx , const int& lineidx , const int& objid ,bool movecheck)
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
			if(true == movecheck && MOVE == (*iter)->GetState() )
				continue;

			if( MOVE_AIR == (*iter)->GetType() || objid == (*iter)->GetObjID() )
				continue;

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
bool CArea_Mgr::diamond_check(const int& idx , const int& objid)
{

	if(idx < 0)
		return false;

	if( !m_Area64[idx].empty() )
	{
		D3DXVECTOR2 vtx[4];
		float a[4];
		float b[4];

		list<CObj*>::iterator iter		= m_Area64[idx].begin();
		list<CObj*>::iterator iter_end	= m_Area64[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if( MOVE == (*iter)->GetState() )
				continue;

			if( MOVE_AIR == (*iter)->GetType() || objid == (*iter)->GetObjID() )
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


			/*
			직선은 0번이 우상 1번이 우하 .. 3번까지 시계방향으로 직선의 순서가 중요하다
			*/

			//if(true == bfilter)
			//{
			//	vtx[4].x = (*iter)->GetPos().x;
			//	vtx[4].y = (*iter)->GetPos().y;

			//	for(int i = 0; i < 5; ++i)
			//	{
			//		if(vtx[i].x * a[0] + b[0] < vtx[i].y &&
			//			vtx[i].x  < m_line[1].fx1 &&
			//			vtx[i].x * a[2] + b[2] > vtx[i].y &&
			//			vtx[i].x  > m_line[3].fx1)
			//		{
			//			return true;
			//		}
			//	}
			//}
			//else
			{
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
		}
	}
	return false;
}
void CArea_Mgr::Areasize_debugrender(const int& areasize , const int& areacnt)
{;
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
		else if(256 == areasize)
		{
			fX = m_areapos256[idx].x - CScrollMgr::m_fScrollX;
			fY = m_areapos256[idx].y - CScrollMgr::m_fScrollY;
			isize = (int)(m_Area256[idx].size());
		}
		else if(512 == areasize)
		{
			fX = m_areapos512[idx].x - CScrollMgr::m_fScrollX;
			fY = m_areapos512[idx].y - CScrollMgr::m_fScrollY;
			isize = (int)(m_Area512[idx].size());
		}
		CFontMgr::GetInstance()->Setbatch_Font( L"%d" , isize , fX , fY);
	}
}
}
bool CArea_Mgr::waypoint_check(const MYRECT<float>& rc ,const D3DXVECTOR2& vpos , const int& objid)
{
	int idx = CMyMath::Pos_to_index(vpos , 64);
	Calculator_eightidx(idx , 64);

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(Collision_Area64Check(m_idxdir[i] , rc , objid))
			return false;
	}
	return true;
}
bool CArea_Mgr::overlap_prevention(const MYRECT<float>& rc ,const MYRECT<float>& orirect , const D3DXVECTOR2& vpos , const D3DXVECTOR2& oripos ,  const D3DXVECTOR2& vmydir, const int& objid)
{
	int idx = CMyMath::Pos_to_index(vpos , 64);
	Calculator_eightidx(idx , 64);

	MYRECT<float> otherrc;
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
				if( MOVE_AIR == (*iter)->GetType() || objid == (*iter)->GetObjID() )
					continue;

				otherrc = (*iter)->GetMyRect();
				if(MyIntersectrect(&outrect , &otherrc , &rc))
				{
					//내가 가고자 하는 방향과 상대와의 벡터가 둔각이면 갈 수 있다.

					//D3DXVECTOR2 tempvec = (*iter)->GetPos() - vpos;

					//D3DXVec2Normalize(&tempvec , &tempvec);
					//float fdot = D3DXVec2Dot(&tempvec , &vmydir);

					//if(fdot >= 0.5f) //예각이면 못가는거다
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
		if(LineCross(m_idxdir[i] , lineidx , objid , false ))
			return true;
	}

	//for(int i = 0; i < ASTAR_DIR_END; ++i)
	//{
	//	if(diamond_check(m_idxdir[i] , objid))
	//		return true;
	//}

	return false;

}
void CArea_Mgr::waypoint_modify(MYRECT<float>& rc , D3DXVECTOR2& vpos , const int& objid)
{
	int idx64 = CMyMath::Pos_to_index(vpos , 64);

	Calculator_eightidx(idx64 , 64);

	MYRECT<float> outrect;
	float fwidth;
	float fheight;

	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if( !m_Area64[ m_idxdir[i] ].empty() )
		{
			list<CObj*>::iterator iter = m_Area64[m_idxdir[i]].begin();
			list<CObj*>::iterator iter_end = m_Area64[m_idxdir[i]].end();

			for( ; iter != iter_end; ++iter)
			{
				if( MOVE == (*iter)->GetState() )
					continue;

				if( MOVE_AIR == (*iter)->GetType() || objid == (*iter)->GetObjID() )
					continue;

				if(MyIntersectrect(&outrect , &(*iter)->GetMyRect() , &rc))
				{
					fwidth	= (outrect.right - outrect.left)*2;
					fheight = (outrect.bottom - outrect.top)*2;

					if(vpos.x < (*iter)->GetPos().x)
						fwidth = -fwidth;

					if(vpos.y < (*iter)->GetPos().y)
						fheight = -fheight;

					if(abs(fwidth) < abs(fheight))
					{
						//가로로 밀어낸다
						vpos.x += fwidth;
						rc.left += fwidth;
						rc.right += fwidth;
					}
					else
					{
						//세로로 밀어낸다
						vpos.y += fheight;
						rc.top += fheight;
						rc.bottom += fheight;
					}

				}
			}
		}
	}

}