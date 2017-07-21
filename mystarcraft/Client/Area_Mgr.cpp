#include "StdAfx.h"
#include "Area_Mgr.h"
#include "Obj.h"

#include "MyMath.h"
#include "UnitMgr.h"
#include "MouseMgr.h"
#include "ScrollMgr.h"

IMPLEMENT_SINGLETON(CArea_Mgr)

CArea_Mgr::CArea_Mgr(void)
{
	m_objId = 0;
}

CArea_Mgr::~CArea_Mgr(void)
{
}
void CArea_Mgr::AutoSearch_target(CObj* pobj , CObj*& ptarget, const int& idx, const int& isearch_range , const int& areasize)
{
	//isearch_range는 픽셀단위이다.

	if(256 == areasize)
		Calculator_eightidx(idx , 16);
	else if(512 == areasize)
		Calculator_eightidx(idx , 8);


	int iminvalue = 1000000000;
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		if(m_idxdir[i] < 0)
			continue;

		if( 256 == areasize)
			Target_extract256(pobj , ptarget , m_idxdir[i] , isearch_range , iminvalue);
		else if( 512 == areasize)
			Target_extract512(pobj , ptarget , m_idxdir[i] , isearch_range , iminvalue);
	}
}
void CArea_Mgr::Target_extract512(CObj* pself , CObj*& ptarget, const int& idx , const int& irange, int& minvalue)
{
	//유닛 탐색범위에 있는 다른 유닛들(적)을 검사한다

	//유닛에따라 area512로 할지 256으로 할지 정해야할듯 싶다..

	if(!m_Area512[idx].empty())
	{
		//가장 가까운 적을 찾기로 하자.
		int idistance = 0;

		list<CObj*>::iterator iter = m_Area512[idx].begin();
		list<CObj*>::iterator iter_end = m_Area512[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(pself != (*iter))
			{
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

void CArea_Mgr::Target_extract256(CObj* pself , CObj*& ptarget, const int& idx , const int& irange, int& minvalue)
{
	//유닛 탐색범위에 있는 다른 유닛들(적)을 검사한다

	//유닛에따라 area512로 할지 256으로 할지 정해야할듯 싶다..

	if(!m_Area256[idx].empty())
	{
		//가장 가까운 적을 찾기로 하자.
		int idistance = 0;

		list<CObj*>::iterator iter = m_Area256[idx].begin();
		list<CObj*>::iterator iter_end = m_Area256[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(pself != (*iter))
			{
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
	Calculator_eightidx(idx , 16);

	//if(DummyCheck(idx , rc))
	//{
	//	/*드래그 한 영역에 선택된 유닛이 있는지 확인한다
	//	
	//	선택된 유닛이 있다면 기존에 선택된 유닛들을 해제시킨다*/
	//	CUnitMgr::GetInstance()->discharge_unit();
	//}
	//else
	//{
	//	/*선택된 유닛이 없다면 주위 8방향으로 검사해본다*/
	//	for(int i = 0; i < ASTAR_DIR_END; ++i)
	//	{
	//		if(DummyCheck(m_idxdir[i] , rc))
	//		{
	//			CUnitMgr::GetInstance()->discharge_unit();
	//			break;
	//		}
	//	}
	//}

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

	//if(true == Choice_unit(idx , vpos))
	//	return;

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
	if( !m_Area256[idx].empty() )
	{
		MYRECT<float> temprc;

		list<CObj*>::iterator iter = m_Area256[idx].begin();
		list<CObj*>::iterator iter_end = m_Area256[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(MyIntersectrect(&temprc , &(*iter)->GetMyRect() , &rc ))
			{
				(*iter)->SetSelect(true);
				CUnitMgr::GetInstance()->SetUnit((*iter));				
				/*아군만 선택*/
			}
		}
	}
}
bool CArea_Mgr::DummyCheck(const int& idx, const MYRECT<float>& rc)
{
	if( !m_Area256[idx].empty() )
	{
		MYRECT<float> temprc;

		list<CObj*>::iterator iter = m_Area256[idx].begin();
		list<CObj*>::iterator iter_end = m_Area256[idx].end();

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
		}
	}
}
bool CArea_Mgr::Collision_Area64Check(const int& idx , D3DXVECTOR2& vpos , const MYRECT<float>& myrc)
{
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
			/*공중이거나 자기 자신이거나(OBJ_ID) 움직이고 있는 상대는 거른다.*/
			if( TYPE_AIR == (*iter)->GetType() || m_objId == (*iter)->GetObjID() )
				continue;

			otherrc = (*iter)->GetMyRect();
			if( !MyPtInrect(vpos , &otherrc) &&
				MyIntersectrect(&out_rect , &otherrc , &myrc))
			{
				return true;
			}
		}
	}

	return false;
}
void CArea_Mgr::Collision_Area64(const int& idx , D3DXVECTOR2& vpos , const ASTAR_DIR& edir)
{
	//길찾기를할때 유닛을 피해서 이동하기 위함 함수이름 바꿀필요 있음

	if(idx < 0)
		return;

	if( !m_Area64[idx].empty() )
	{

		MYRECT<float> out_rect;

		float	fwidth = 0.f;
		float	fheight = 0.f;

		list<CObj*>::iterator iter;
		list<CObj*>::iterator iter_end;

		iter = m_Area64[idx].begin();
		iter_end = m_Area64[idx].end();

		MYRECT<float> otherrc;

		for( ; iter != iter_end; ++iter )
		{
			/*공중이거나 자기 자신이거나(OBJ_ID) 움직이고 있는 상대는 거른다.*/
			if( TYPE_AIR == (*iter)->GetType() || m_objId == (*iter)->GetObjID() )
				continue;

			otherrc = (*iter)->GetMyRect();
			if( !MyPtInrect(vpos , &otherrc) &&
				MyIntersectrect(&out_rect , &otherrc , &m_unitrc))
			{


				if(UP == edir || DOWN == edir)
				{
					if(m_unitrc.top > otherrc.top)
						fheight = out_rect.bottom - out_rect.top;
					else
						fheight = out_rect.top - out_rect.bottom;

					m_unitrc.bottom += fheight;
					m_unitrc.top += fheight;
					vpos.y += fheight;
				}
				else if( LEFT == edir || RIGHT == edir )
				{
					if(m_unitrc.right > otherrc.right)
						fwidth = out_rect.left - out_rect.right;
					else
						fwidth = out_rect.right - out_rect.left;

					m_unitrc.left += fwidth;
					m_unitrc.right += fwidth;
					vpos.x += fwidth;
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
bool CArea_Mgr::Check_Area(ASTAR_DIR edir , const int& idx ,const MYRECT<float>& rc , D3DXVECTOR2& vpos)
{
	m_unitrc = rc; // 복사되는지 유의

	Calculator_eightidx(idx , 64);

	//m_checkidx 원래 가야할 인덱스 이 인덱스를 벗어나면 무효처리

	int			stepcnt = 16;
	int			tilesize = 16;

	D3DXVECTOR2 vtemp;
	MYRECT<float> temprc;

	vtemp = vpos;
	temprc = rc;	

	if(UP == edir)
	{
		vpos.y -= stepcnt;

		m_unitrc.bottom -= stepcnt;
		m_unitrc.top -= stepcnt;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);

		for(int i = 0; i < ASTAR_DIR_END; ++i)
			Collision_Area64(m_idxdir[i] , vpos , UP);

		//값을 밀어냈는데 해당하는 방향의 인덱스와 같지않으면 무효처리
	}
	else if(DOWN == edir)
	{
		vpos.y += stepcnt;

		m_unitrc.bottom += stepcnt;
		m_unitrc.top += stepcnt;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);

		for(int i = 0; i < ASTAR_DIR_END; ++i)
			Collision_Area64(m_idxdir[i] , vpos , DOWN);
	}
	else if(LEFT == edir)
	{
		vpos.x -= stepcnt;

		m_unitrc.left -= stepcnt;
		m_unitrc.right -= stepcnt;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);

		for(int i = 0; i < ASTAR_DIR_END; ++i)
			Collision_Area64(m_idxdir[i] , vpos , LEFT);
	}
	else if(RIGHT == edir)
	{
		vpos.x += stepcnt;

		m_unitrc.left += stepcnt;
		m_unitrc.right += stepcnt;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);

		for(int i = 0; i < ASTAR_DIR_END; ++i)
			Collision_Area64(m_idxdir[i] , vpos , RIGHT);
	}
	else if(RIGHT_UP == edir)
	{
		vtemp.x += stepcnt;
		temprc.left += stepcnt;
		temprc.right += stepcnt;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}
		vtemp = vpos;
		temprc = m_unitrc;

		
		vtemp.y -= stepcnt;
		temprc.bottom -= stepcnt;
		temprc.top -= stepcnt;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}

		vpos.x += stepcnt;
		vpos.y -= stepcnt;
		m_unitrc.left += stepcnt;
		m_unitrc.right += stepcnt;
		m_unitrc.top -= stepcnt;
		m_unitrc.bottom -= stepcnt;

		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);

	}
	else if(RIGHT_DOWN == edir)
	{

		vtemp.x += stepcnt;
		temprc.left += stepcnt;
		temprc.right += stepcnt;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}
		vtemp = vpos;
		temprc = m_unitrc;

		vtemp.y += stepcnt;
		temprc.bottom += stepcnt;
		temprc.top += stepcnt;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}


		vpos.x += stepcnt;
		vpos.y += stepcnt;
		m_unitrc.left += stepcnt;
		m_unitrc.right += stepcnt;
		m_unitrc.top += stepcnt;
		m_unitrc.bottom += stepcnt;


		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);
	}
	else if(LEFT_UP == edir)
	{

		vtemp.x -= stepcnt;
		temprc.left -= stepcnt;
		temprc.right -= stepcnt;

		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}
		vtemp = vpos;
		temprc = m_unitrc;


		vtemp.y -= stepcnt;
		temprc.bottom -= stepcnt;
		temprc.top -= stepcnt;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}


		vpos.x -= stepcnt;
		vpos.y -= stepcnt;
		m_unitrc.left -= stepcnt;
		m_unitrc.right -= stepcnt;
		m_unitrc.top -= stepcnt;
		m_unitrc.bottom -= stepcnt;

		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);

	}
	else if(LEFT_DOWN == edir)
	{
		vtemp.x -= stepcnt;
		temprc.left -= stepcnt;
		temprc.right -= stepcnt;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}
		vtemp = vpos;
		temprc = m_unitrc;


		vtemp.y += stepcnt;
		temprc.bottom += stepcnt;
		temprc.top += stepcnt;
		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if(Collision_Area64Check(m_idxdir[i] , vtemp , temprc))
				return false;
		}


		vpos.x -= stepcnt;
		vpos.y += stepcnt;
		m_unitrc.left -= stepcnt;
		m_unitrc.right -= stepcnt;
		m_unitrc.top += stepcnt;
		m_unitrc.bottom += stepcnt;

		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , tilesize);

	}


	if( m_checkidx == CMyMath::Pos_to_index(vpos.x , vpos.y ,tilesize) )
	{
		MYRECT<float> outrc;

		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			list<CObj*>::iterator iter = m_Area64[ m_idxdir[i] ].begin();
			list<CObj*>::iterator iter_end = m_Area64[ m_idxdir[i] ].end();

			for( ; iter != iter_end; ++iter)
			{
				if( TYPE_AIR == (*iter)->GetType() || m_objId == (*iter)->GetObjID())
					continue;

				if(MyIntersectrect(&outrc , &m_unitrc , &(*iter)->GetMyRect()))
					return false;
			}
		}

		return true;
	}
	else
		return false;
}

void CArea_Mgr::SetObjId(const int& id)
{
	m_objId = id;
}

