#include "StdAfx.h"
#include "Astar.h"

#include "MyMath.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "FontMgr.h"
#include "TimeMgr.h"
#include "MyHeapSort.h"

#include "Device.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Obj.h"
#include "ObjMgr.h"
//IMPLEMENT_SINGLETON(CAstar);
CAstar::CAstar(void)
{
	m_ptarget = NULL;

	m_openlist = new CMyHeapSort<PATH_NODE*>(512);
	//m_RVopenlist = new CMyHeapSort<PATH_NODE*>;
	m_unitpath_pool = new PATH_NODE[MAXPATH_IDX];

	m_pObj = NULL;

	m_dummynodeH = NULL;
	m_dummynodeX = NULL;

	m_fdummytime = 0.f;

	m_eight_weight[UP] = 32;
	m_eight_weight[DOWN] = 32;
	m_eight_weight[LEFT] = 32;
	m_eight_weight[RIGHT] = 32;
	m_eight_weight[LEFT_UP] = 45;
	m_eight_weight[RIGHT_UP] = 45;
	m_eight_weight[LEFT_DOWN] = 45;
	m_eight_weight[RIGHT_DOWN] = 45;

	m_areamgr_inst = CArea_Mgr::GetInstance();

	m_pathpool_idx = 0;
	m_unit_stepsize = 0;
	m_unit_diagonalstep = 0.f;

	m_bweightG = true;

	m_is_escape = false;


	m_maxnodecnt = MAXPATH_IDX;

	//memset(m_idxfind , 0 , sizeof(m_idxfind));
}
CAstar::~CAstar(void)
{
	m_pObj = NULL;
	Release();
	Safe_Delete(m_openlist);
	//Safe_Delete(m_RVopenlist);
}
void CAstar::Initialize(CObj* pobj)
{
	m_pObj = pobj;
	m_vertex = m_pObj->GetVertex();
	m_range = max(m_pObj->GetUnitinfo().attack_range , m_pObj->GetUnitinfo().air_attack_range) - 32;
	if(m_range < 0)
		m_range = 32;
	m_terrain_end = false;

	memset(m_findidx , 0 , sizeof(m_findidx));
	m_mask[0] = 1;
	for(int i = 1; i < 32; ++i)
	{
		m_mask[i] = m_mask[i-1] << 1;
	}
}

void CAstar::Init_eightpos_rect(const D3DXVECTOR2& vpos , const MYRECT<float>& rc, const int& stepsize)
{
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		m_eight_vpos[i] = vpos;
		m_eight_rect[i] = rc;
	}


	if(vpos.y >= stepsize)
		m_eight_vpos[UP].y = m_eight_vpos[CENTER].y - stepsize;

	if(vpos.y <= 4096 - stepsize)
		m_eight_vpos[DOWN].y = m_eight_vpos[CENTER].y + stepsize;

	if(vpos.x >= stepsize)
		m_eight_vpos[LEFT].x = m_eight_vpos[CENTER].x - stepsize;

	if(vpos.x <= 4096 - stepsize)
		m_eight_vpos[RIGHT].x = m_eight_vpos[CENTER].x + stepsize;


	if(vpos.x <= 4096 - stepsize &&
		vpos.y >= stepsize )
	{
		m_eight_vpos[RIGHT_UP].x = m_eight_vpos[CENTER].x + stepsize;
		m_eight_vpos[RIGHT_UP].y = m_eight_vpos[CENTER].y - stepsize;
	}
	

	if(vpos.x <= 4096 - stepsize &&
		vpos.y <= 4096 - stepsize )
	{
		m_eight_vpos[RIGHT_DOWN].x = m_eight_vpos[CENTER].x + stepsize;
		m_eight_vpos[RIGHT_DOWN].y = m_eight_vpos[CENTER].y + stepsize;
	}

	if(vpos.x >= stepsize &&
		vpos.y <= 4096 - stepsize)
	{
		m_eight_vpos[LEFT_DOWN].x = m_eight_vpos[CENTER].x - stepsize;
		m_eight_vpos[LEFT_DOWN].y = m_eight_vpos[CENTER].y + stepsize;
	}

	if(vpos.x >= stepsize &&
		vpos.y >= stepsize)
	{
		m_eight_vpos[LEFT_UP].x = m_eight_vpos[CENTER].x - stepsize;
		m_eight_vpos[LEFT_UP].y = m_eight_vpos[CENTER].y - stepsize;
	}

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	if(rc.top >= stepsize)
	{
		m_eight_rect[UP].top = m_eight_rect[CENTER].top - stepsize;
		m_eight_rect[UP].bottom = m_eight_rect[CENTER].bottom - stepsize;
	}

	m_eight_rect[DOWN].top = m_eight_rect[CENTER].top + stepsize;
	m_eight_rect[DOWN].bottom = m_eight_rect[CENTER].bottom + stepsize;

	m_eight_rect[LEFT].left = m_eight_rect[CENTER].left - stepsize;
	m_eight_rect[LEFT].right = m_eight_rect[CENTER].right - stepsize;

	m_eight_rect[RIGHT].left = m_eight_rect[CENTER].left + stepsize;
	m_eight_rect[RIGHT].right = m_eight_rect[CENTER].right + stepsize;

	m_eight_rect[LEFT_UP].left = m_eight_rect[CENTER].left - stepsize;
	m_eight_rect[LEFT_UP].right = m_eight_rect[CENTER].right - stepsize;
	m_eight_rect[LEFT_UP].top = m_eight_rect[CENTER].top - stepsize;
	m_eight_rect[LEFT_UP].bottom = m_eight_rect[CENTER].bottom - stepsize;

	m_eight_rect[LEFT_DOWN].left = m_eight_rect[CENTER].left - stepsize;
	m_eight_rect[LEFT_DOWN].right = m_eight_rect[CENTER].right - stepsize;
	m_eight_rect[LEFT_DOWN].top = m_eight_rect[CENTER].top + stepsize;
	m_eight_rect[LEFT_DOWN].bottom = m_eight_rect[CENTER].bottom + stepsize;

	m_eight_rect[RIGHT_DOWN].left = m_eight_rect[CENTER].left + stepsize;
	m_eight_rect[RIGHT_DOWN].right = m_eight_rect[CENTER].right + stepsize;
	m_eight_rect[RIGHT_DOWN].top = m_eight_rect[CENTER].top + stepsize;
	m_eight_rect[RIGHT_DOWN].bottom = m_eight_rect[CENTER].bottom + stepsize;

	m_eight_rect[RIGHT_UP].left = m_eight_rect[CENTER].left + stepsize;
	m_eight_rect[RIGHT_UP].right = m_eight_rect[CENTER].right + stepsize;
	m_eight_rect[RIGHT_UP].top = m_eight_rect[CENTER].top - stepsize;
	m_eight_rect[RIGHT_UP].bottom = m_eight_rect[CENTER].bottom - stepsize;

}
void CAstar::Init_eightidx(const int& idx)
{
	m_eight_idx[CENTER] = idx;

	if(idx - m_tilecnt < 0)
		m_eight_idx[UP] = -1;
	else
		m_eight_idx[UP] = idx - m_tilecnt;

	if(idx % m_tilecnt <= 0 ||
		idx - m_tilecnt < 0)
		m_eight_idx[LEFT_UP] = -1;
	else
		m_eight_idx[LEFT_UP] = idx - m_tilecnt - 1;

	if(idx % m_tilecnt >= m_tilecnt - 1 ||
		idx - m_tilecnt < 0)
		m_eight_idx[RIGHT_UP] = -1;
	else
		m_eight_idx[RIGHT_UP] = idx - m_tilecnt + 1;

	if(idx % m_tilecnt >= m_tilecnt - 1)
		m_eight_idx[RIGHT] = -1;
	else
		m_eight_idx[RIGHT] = idx  + 1;

	if(idx % m_tilecnt <= 0)
		m_eight_idx[LEFT] = -1;
	else
		m_eight_idx[LEFT] = idx  - 1;

	if(idx + m_tilecnt >= m_tilecnt*m_tilecnt)
		m_eight_idx[DOWN] = -1;
	else
		m_eight_idx[DOWN] = idx + m_tilecnt;

	if(idx + m_tilecnt >= m_tilecnt*m_tilecnt ||
		idx % m_tilecnt >= m_tilecnt - 1)
		m_eight_idx[RIGHT_DOWN] = -1;
	else
		m_eight_idx[RIGHT_DOWN] = idx + m_tilecnt + 1;

	if(idx + m_tilecnt >= m_tilecnt*m_tilecnt ||
		idx % m_tilecnt <= 0)
		m_eight_idx[LEFT_DOWN] = -1;
	else
		m_eight_idx[LEFT_DOWN] = idx + m_tilecnt - 1;
}
void CAstar::Make_UnitNode( PATH_NODE* parent_node ,const D3DXVECTOR2& vpos , const float& g_distance)
{

	if(	m_pathpool_idx >= m_maxnodecnt - 1)
		return;
	if( !m_openlist->node_full())
		return;

	PATH_NODE* pnewnode = &(m_unitpath_pool[m_pathpool_idx]);
	++m_pathpool_idx;

	pnewnode->pParent = parent_node;
	pnewnode->index = CMyMath::Pos_to_index(vpos , m_unit_stepsize);
	pnewnode->vPos = vpos;





	m_openlist->push_node(pnewnode);

	if( D3DXVec2Dot(&m_destdir , &(pnewnode->vPos - parent_node->vPos) ) >= 0)
	{
		//90������ �۴�
		pnewnode->G = parent_node->G + g_distance*g_distance;
		pnewnode->X = parent_node->X + g_distance*g_distance*2;
	}
	else
	{
		pnewnode->G = parent_node->G + g_distance*g_distance*2;
		pnewnode->X = parent_node->X + g_distance*g_distance;
	}

	pnewnode->H = CMyMath::pos_distance(vpos , m_vGoal_pos);
	pnewnode->iCost = pnewnode->G + pnewnode->H;

	//m_idxfind[pnewnode->index] = true;


	m_irow = int(pnewnode->vPos.x / (4096 / m_widthcnt) );
	m_icol = int(pnewnode->vPos.y / m_unit_stepsize);
	m_maskidx = int(pnewnode->vPos.x / m_unit_stepsize) % 32;
	m_findidx[m_icol * m_widthcnt + m_irow] |= m_mask[m_maskidx];
}
void CAstar::UnitPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , const int& stepsize, const vector<D3DXVECTOR2>& terrain_path , const int& terrain_idx)
{	
	memset(m_findidx , 0 , sizeof(m_findidx));
	//memset(m_idxfind , 0 , sizeof(m_idxfind));
	Release_unit_closelist();
	Release_unit_openlist();

	if(!terrain_path.empty())
	{
		if( terrain_idx == terrain_path.size() - 1)
			m_terrain_end = true;
		else
			m_terrain_end = false;
	}

	m_is_escape = false;

	
	m_unit_stepsize = 32;
	//m_unit_stepsize = stepsize;
	m_tilecnt = int(SQ_TILECNTX*(float)SQ_TILESIZEX/(float)m_unit_stepsize);
	
	if(8 == m_unit_stepsize)
		m_unit_diagonalstep = 11.3f;
	else if(16 == m_unit_stepsize)
		m_unit_diagonalstep = 22.f;
	else if(32 == m_unit_stepsize)
		m_unit_diagonalstep = 45.f;

	m_vStart_pos = startpos;
	m_vGoal_pos = goalpos;

	m_startindex = CMyMath::Pos_to_index(startpos.x , startpos.y , m_unit_stepsize);
	m_goalidx = CMyMath::Pos_to_index(goalpos.x , goalpos.y , m_unit_stepsize);

	PATH_NODE* pnode = &m_unitpath_pool[m_pathpool_idx];
	++m_pathpool_idx;
	pnode->X = 0;
	pnode->G = 0;	
	pnode->H = CMyMath::pos_distance(startpos , goalpos);
	pnode->iCost = pnode->G + pnode->H;
	pnode->index = m_startindex;
	pnode->vPos = startpos;
	pnode->pParent = NULL;
	m_openlist->push_node(pnode);


	//m_idxfind[pnode->index] = true;
	m_widthcnt = (4096 / m_unit_stepsize)/32;
	m_irow = int(pnode->vPos.x / (4096 / m_widthcnt) );
	m_icol = int(pnode->vPos.y / m_unit_stepsize);
	m_maskidx = int(pnode->vPos.x / m_unit_stepsize) % 32;
	m_findidx[m_icol * m_widthcnt + m_irow] |= m_mask[m_maskidx];

	m_dummynodeH = pnode;
	m_dummynodeX = pnode;

	m_destdir = goalpos - startpos;
}
void CAstar::UnitPath_calculation_Update(vector<D3DXVECTOR2>& vecpath , const CObj* ptarget)
{

	PATH_NODE* pnode;

	MYRECT<float> temprect;
	float gdistance = 0;
	int range = 0;

	m_ptarget = ptarget;

	int irow , icol , maskidx = 0; 
	while(true)
	{
		pnode = m_openlist->pop_node();

		if(NULL != pnode)
		{
			//��Ʈ�������� ���ܻ����� Ʈ�� �޽�����,,,
			//m_idxfind[pnode->index] = true;
		
			irow = int(pnode->vPos.x / (4096 / m_widthcnt));
			icol = int(pnode->vPos.y / m_unit_stepsize);
			maskidx = int(pnode->vPos.x / m_unit_stepsize) % 32;
			m_findidx[icol * m_widthcnt + irow] |= m_mask[maskidx];

			if(NULL != m_ptarget)
			{
				range = m_range;
				if( CMyMath::pos_distance( pnode->vPos , m_vGoal_pos) < range*range)
				{
					if(CTileManager::GetInstance()->Bresenham_Tilecheck(pnode->vPos , m_vGoal_pos))
					{
						m_pathpool_idx = 0;

						while(NULL != pnode)
						{
							vecpath.push_back(pnode->vPos);
							pnode = pnode->pParent;
						}					
						break;
					}
				}
			}
			else
			{
				range = 200;
				if( !m_terrain_end && CMyMath::pos_distance( pnode->vPos , m_vGoal_pos) < range*range)
				{
					if(CTileManager::GetInstance()->Bresenham_Tilecheck(pnode->vPos , m_vGoal_pos))
					{
						m_pathpool_idx = 0;

						while(NULL != pnode)
						{
							vecpath.push_back(pnode->vPos);
							pnode = pnode->pParent;
						}					
						break;
					}
				}
			}
		}

		if(NULL == pnode)
		{
			//dummyH
			while(NULL != m_dummynodeH)
			{
				vecpath.push_back(m_dummynodeH->vPos);
				m_dummynodeH = m_dummynodeH->pParent;
			}
			break;
		}	

		if(GETTIME > 0.016f)
		{
			m_pathpool_idx = 0;
			//������ pnode
			if(!m_terrain_end)
			{
				while(NULL != m_dummynodeX)
				{
					vecpath.push_back(m_dummynodeX->vPos);
					m_dummynodeX = m_dummynodeX->pParent;
				}					
				break;
			}
			else
			{
				while(NULL != m_dummynodeH)
				{
					vecpath.push_back(m_dummynodeH->vPos);
					m_dummynodeH = m_dummynodeH->pParent;
				}					
				break;
			}

			//if(NULL == m_ptarget)
			//{
			//	if( true == m_terrain_end)
			//	{
			//		while(NULL != m_dummynodeH)
			//		{
			//			vecpath.push_back(m_dummynodeH->vPos);
			//			m_dummynodeH = m_dummynodeH->pParent;
			//		}		
			//	}
			//	break;
			//}
			//else
			//{
			//	m_pathpool_idx = 0;
			//	//������ pnode
			//	while(NULL != m_dummynodeX)
			//	{
			//		vecpath.push_back(m_dummynodeX->vPos);
			//		m_dummynodeX = m_dummynodeX->pParent;
			//	}					
			//	break;
			//}
		}

		if( true == m_is_escape )
		{
			m_is_escape = false;
			m_pathpool_idx = 0;

			while(NULL != pnode)
			{
				vecpath.push_back(pnode->vPos);
				pnode = pnode->pParent;
			}
			break;
		}

		if(m_pathpool_idx >= m_maxnodecnt - 1)
		{
			//dummyG
			if(!m_terrain_end)
			{
				while(NULL != m_dummynodeX)
				{
					vecpath.push_back(m_dummynodeX->vPos);
					m_dummynodeX = m_dummynodeX->pParent;
				}
			}
			else
			{
				while(NULL != m_dummynodeH)
				{
					vecpath.push_back(m_dummynodeH->vPos);
					m_dummynodeH = m_dummynodeH->pParent;
				}
			}

			//m_dummynode�� NULL�� �������� �������� NULL�� ���Թ�����
			//�ٽ� start�� ��ĥ��� m_dummynode�� NULL�� �����ϱ⶧���� ������ ����
			break;
		}			

		if(pnode->index == m_goalidx)
		{
			pnode->vPos = m_vGoal_pos;
			while(NULL != pnode)
			{
				vecpath.push_back(pnode->vPos);
				pnode = pnode->pParent;
			}
			break;
		}

		if(m_dummynodeX->X <= pnode->X) //���������� �հ�����
			m_dummynodeX = pnode;

		if(m_dummynodeH->H >= pnode->H)// �������� ����������
			m_dummynodeH = pnode;

		temprect.left = pnode->vPos.x - m_vertex.left;
		temprect.right = pnode->vPos.x + m_vertex.right;
		temprect.top = pnode->vPos.y - m_vertex.top;
		temprect.bottom = pnode->vPos.y + m_vertex.bottom;

		Init_eightpos_rect(pnode->vPos , temprect , m_unit_stepsize);

		
		int idx = 0;
		for(int i = 1; i < ASTAR_DIR_END; ++i)
		{
			if( i >= 5)
				gdistance = m_unit_diagonalstep;
			else
				gdistance = (float)m_unit_stepsize;

			if( Check_TileOption(m_eight_vpos[i]) &&
				/*Check_CloseList(m_eight_vpos[i]) &&
				Check_OpenList(m_eight_vpos[i] , pnode)*/
				Check_idx(m_eight_vpos[i]))
			{
				if( m_areamgr_inst->Check_Area(m_eight_rect[i] , m_eight_vpos[i] , m_eight_rect[CENTER] , m_eight_vpos[CENTER] , m_unit_stepsize , m_pObj , m_ptarget ) )
					Make_UnitNode(pnode , m_eight_vpos[i] , gdistance);	
				else
				{
					if( CMyMath::Pos_to_index( m_eight_vpos[i] , m_unit_stepsize) == m_goalidx)
					{
						m_is_escape = true;
						break;
					}
				}
			}
		}
	}
}
bool CAstar::Check_TileOption(const int& idx)
{
	if(idx < 0)
		return false;

	BYTE option = CTileManager::GetInstance()->GetTileOption(idx);

	if(MOVE_NONE == option)
		return false;
	else
		return true;

}
bool CAstar::Check_TileOption(const D3DXVECTOR2& vpos)
{
	int idx = CMyMath::Pos_to_index(vpos.x , vpos.y , 32);
	BYTE option = CTileManager::GetInstance()->GetTileOption(idx);

	if(MOVE_NONE == option)
		return false;
	else
		return true;

}
bool CAstar::Check_idx(const D3DXVECTOR2& vpos)
{
	//int idx = CMyMath::Pos_to_index(vpos , m_unit_stepsize);

	//if( true == m_idxfind[idx])
	//	return false;

	//return true;

	m_irow = int(vpos.x / (4096 / m_widthcnt));
	m_icol = int(vpos.y / m_unit_stepsize);
	m_maskidx = int(vpos.x / m_unit_stepsize) % 32;
	
	if(m_findidx[m_icol * m_widthcnt + m_irow] & m_mask[m_maskidx])
		return false;
	else
		return true;
;
}
bool CAstar::Check_CloseList(const D3DXVECTOR2& vpos)
{
	int idx = CMyMath::Pos_to_index(vpos , m_unit_stepsize);

	//if( m_closelist.end() != m_closelist.find(idx) )
		return false;


	return true;
}
bool CAstar::Check_OpenList(const D3DXVECTOR2& idxpos , PATH_NODE* parentnode)
{

	int idx = CMyMath::Pos_to_index( idxpos , m_unit_stepsize );

	//if(m_idxopenlist.end() != m_idxopenlist.find(idx))
		return false;

		return true;
}

void CAstar::Release(void)
{
	Release_unit_closelist();
	Release_unit_openlist();

	delete[] m_unitpath_pool;
	m_unitpath_pool = NULL;
}
void CAstar::Release_unit_closelist(void)
{
	m_pathpool_idx = 0;
	//if(!m_closelist.empty())
		//m_closelist.clear();
}
void CAstar::Release_unit_openlist(void)
{
	m_openlist->Release();

	//if(!m_idxopenlist.empty())
		//m_idxopenlist.clear();

}

void CAstar::Path_Render(void)
{

	//const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile8x8");

	//D3DXMATRIX	tempmat;
	//D3DXMatrixIdentity(&tempmat);

	////if(!m_unit_path.empty())
	////{
	////	for(size_t i = 0; i < m_unit_path.size(); ++i)
	////	{
	////		tempmat._41 = m_unit_path[i].x - CScrollMgr::m_fScrollX;
	////		tempmat._42 = m_unit_path[i].y - CScrollMgr::m_fScrollY;

	////		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	////		if(0 == i)
	////			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
	////		else if( i == m_unit_path.size() - 1)
	////			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
	////		else
	////			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	////	}

	////	//tempmat._41 = m_terrain_path[0].x - CScrollMgr::m_fScrollX;
	////	//tempmat._42 = m_terrain_path[0].y - CScrollMgr::m_fScrollY;
	////	//CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);
	////	//CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	////}

	////if(!m_closelist.empty())
	////{
	////	boost::unordered_map<int , PATH_NODE*>::iterator iter = m_closelist.begin();
	////	boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_closelist.end();

	////	for( ; iter != iter_end; ++iter)
	////	{
	////		tempmat._41 = iter->second->vPos.x - CScrollMgr::m_fScrollX;
	////		tempmat._42 = iter->second->vPos.y - CScrollMgr::m_fScrollY;

	////		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	////		CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,0));
	////	}
	////}

	//PATH_NODE* pnode = NULL;
	//int idx = 0;
	//while(true)
	//{
	//	pnode = m_openlist->Render(idx);
	//	if(NULL == pnode)
	//		break;

	//	++idx;

	//	tempmat._41 = pnode->vPos.x - CScrollMgr::m_fScrollX;
	//	tempmat._42 = pnode->vPos.y - CScrollMgr::m_fScrollY;

	//	CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	//	CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
	//}
}