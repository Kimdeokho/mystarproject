#include "StdAfx.h"
#include "Area_Mgr.h"
#include "Obj.h"

#include "MyMath.h"
IMPLEMENT_SINGLETON(CArea_Mgr)
CArea_Mgr::CArea_Mgr(void)
{
	m_objId = 0;
}

CArea_Mgr::~CArea_Mgr(void)
{
}
void CArea_Mgr::DragCheck(const int& idx , MYRECT<float>& rc)
{
	Calculator_eightidx(idx , 16);

}
void CArea_Mgr::Choice_unit(const int& idx)
{
	if( !m_Area256[idx].empty() )
	{

	}
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
			}
			else
				++iter;
		}
	}

	m_Area512[curidx].push_back(pobj);
}
void CArea_Mgr::Collision_Area64(const int& idx , D3DXVECTOR2& vpos)
{
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


		for( ; iter != iter_end; ++iter)
		{
			/*공중이거나 자기 자신이거나(OBJ_ID) 움직이고 있는 상대는 거른다.*/
			if(TYPE_AIR == (*iter)->GetType() ||
				m_objId == (*iter)->GetObjID())
				continue;

			if(!MyPtInrect(vpos , &(*iter)->GetMyRect()) &&
				MyIntersectrect(&out_rect , &(*iter)->GetMyRect() , &m_unitrc))
			{
				if(m_unitrc.right > (*iter)->GetMyRect().right)
					fwidth = out_rect.left - out_rect.right;
				else
					fwidth = out_rect.right - out_rect.left;

				if(m_unitrc.top > (*iter)->GetMyRect().top)
					fheight = out_rect.bottom - out_rect.top;
				else
					fheight = out_rect.top - out_rect.bottom;

				if( abs(fwidth) > abs(fheight) )
				{
					//y값을 밀어낸다
					m_unitrc.bottom += fheight;
					m_unitrc.top += fheight;
					vpos.y += fheight;
				}
				else
				{
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

	if(m_idx < 0)
		m_up_idx = -1;
	else
		m_up_idx = (idx - tilecnt);

	if(m_idx < 0 ||
		m_idx % tilecnt <= 0)
		m_leftup_idx = -1;
	else
		m_leftup_idx = idx - 1 - tilecnt;

	if(m_idx < 0 ||
		m_idx % tilecnt >= tilecnt - 1)
		m_rightup_idx = -1;
	else
		m_rightup_idx = idx + 1 - tilecnt;

	if(m_idx / tilecnt >= tilecnt-1)
		m_down_idx = -1;
	else
		m_down_idx = idx + tilecnt;

	if(m_idx % tilecnt <= 0)
		m_left_idx = -1;
	else
		m_left_idx = idx - 1;

	if(m_idx % tilecnt >= tilecnt - 1)
		m_right_idx = -1;
	else
		m_right_idx = idx + 1;

	if(m_idx / tilecnt >= tilecnt - 1 ||
		m_idx % tilecnt <= 0)
		m_leftdown_idx = -1;
	else
		m_leftdown_idx = idx - 1 + tilecnt;

	if(m_idx / tilecnt >= tilecnt - 1 ||
		m_idx % tilecnt >= tilecnt - 1)
		m_rightdown_idx = -1;
	else
		m_rightdown_idx = idx + 1 + tilecnt;
}
bool CArea_Mgr::Check_Area(ASTAR_DIR edir , const int& idx ,const MYRECT<float>& rc , D3DXVECTOR2& vpos)
{
	m_unitrc = rc; // 복사되는지 유의

	Calculator_eightidx(idx , 64);

	if(UP == edir)
	{
		vpos.y -= 8;

		m_unitrc.bottom -= 8;
		m_unitrc.top -= 8;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_up_idx , vpos);
		Collision_Area(m_leftup_idx , vpos);
		Collision_Area(m_rightup_idx , vpos);

		//값을 밀어냈는데 해당하는 방향의 인덱스와 같지않으면 무효처리
	}
	else if(DOWN == edir)
	{
		vpos.y += 8;

		m_unitrc.bottom += 8;
		m_unitrc.top += 8;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_down_idx , vpos);
		Collision_Area(m_leftdown_idx , vpos);
		Collision_Area(m_rightdown_idx , vpos);
	}
	else if(LEFT == edir)
	{
		vpos.x -= 8;

		m_unitrc.left -= 8;
		m_unitrc.right -= 8;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_left_idx , vpos);
		Collision_Area(m_leftup_idx , vpos);
		Collision_Area(m_leftdown_idx , vpos);
	}
	else if(RIGHT == edir)
	{
		vpos.x += 8;

		m_unitrc.left += 8;
		m_unitrc.right += 8;
		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_right_idx , vpos);
		Collision_Area(m_rightup_idx , vpos);
		Collision_Area(m_rightdown_idx , vpos);
	}
	else if(RIGHT_UP == edir)
	{
		vpos.x += 8;
		vpos.y -= 8;

		m_unitrc.left += 8;
		m_unitrc.right += 8;
		m_unitrc.top -= 8;
		m_unitrc.bottom -= 8;

		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_rightup_idx , vpos);
		Collision_Area(m_up_idx , vpos);
		Collision_Area(m_right_idx , vpos);
	}
	else if(RIGHT_DOWN == edir)
	{
		vpos.x += 8;
		vpos.y += 8;

		m_unitrc.left += 8;
		m_unitrc.right += 8;
		m_unitrc.top += 8;
		m_unitrc.bottom += 8;

		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_rightdown_idx , vpos);
		Collision_Area(m_right_idx , vpos);
		Collision_Area(m_down_idx , vpos);
	}
	else if(LEFT_UP == edir)
	{
		vpos.x -= 8;
		vpos.y -= 8;

		m_unitrc.left -= 8;
		m_unitrc.right -= 8;
		m_unitrc.top -= 8;
		m_unitrc.bottom -= 8;

		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_leftup_idx , vpos);
		Collision_Area(m_left_idx , vpos);
		Collision_Area(m_up_idx , vpos);
	}
	else if(LEFT_DOWN == edir)
	{
		vpos.x -= 8;
		vpos.y += 8;

		m_unitrc.left -= 8;
		m_unitrc.right -= 8;
		m_unitrc.top += 8;
		m_unitrc.bottom += 8;

		m_checkidx = CMyMath::Pos_to_index(vpos.x , vpos.y , 8);

		Collision_Area(m_idx , vpos);
		Collision_Area(m_leftdown_idx , vpos);
		Collision_Area(m_left_idx , vpos);
		Collision_Area(m_down_idx , vpos);
	}

	if( m_checkidx == CMyMath::Pos_to_index(vpos.x , vpos.y ,8) )
		return true;
	else
		return false;
}

void CArea_Mgr::SetObjId(const int& id)
{
	m_objId = id;
}
