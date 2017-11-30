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
	//memset(m_terrain_idxopenlist , 0 , sizeof(m_terrain_idxopenlist));
	//m_terrain_path.reserve(256);
	//m_dummy_path.reserve(256);
	//m_unit_path.reserve(256);

	//m_terrain_openlist = new CMyHeapSort<PATH_NODE*>;
	m_openlist = new CMyHeapSort<PATH_NODE*>;
		m_unitpath_pool = new PATH_NODE[MAXPATH_IDX];
	//m_PathPool = new boost::pool<>(sizeof(PATH_NODE));

	m_brealpath = false;
	m_pObj = NULL;

	m_accumulate = 0;

	m_dummynode = NULL;

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
	m_objid = -1;
	m_unit_stepsize = 0;
	m_unit_diagonalstep = 0.f;

	m_bweightG = true;

	m_targetid = 0;
}
CAstar::~CAstar(void)
{
	m_pObj = NULL;
	Release();
	//Safe_Delete(m_terrain_openlist);
	Safe_Delete(m_openlist);
}
//for_each()
void CAstar::Initialize(CObj* pobj)
{
	m_pObj = pobj;
}
void CAstar::Make_UnitNode(PATH_NODE* parent_node ,const D3DXVECTOR2& vpos , const float& g_distance)
{

	int idx32 = CMyMath::Pos_to_index(vpos.x , vpos.y , 32);
	if( MOVE_NONE == CTileManager::GetInstance()->GetTileOption(idx32) || 
		m_pathpool_idx >= MAXPATH_IDX - 1)
		return;

	PATH_NODE* pnewnode = &(m_unitpath_pool[m_pathpool_idx]);
	++m_pathpool_idx;

	pnewnode->pParent = parent_node;
	pnewnode->index = CMyMath::Pos_to_index(vpos , m_unit_stepsize);
	pnewnode->vPos = vpos;

	//D3DXVECTOR2 vnormal1 , vnormal2;
	//D3DXVec2Normalize( &vnormal1 , &(m_vGoal_pos - m_vStart_pos) );
	//D3DXVec2Normalize( &vnormal2 , &(vpos - m_vStart_pos) );
	//float fdot = D3DXVec2Dot(&vnormal1 , &vnormal2);	

	pnewnode->G = parent_node->G + g_distance;

	//pnewnode->G = parent_node->G + g_distance * -fdot;

	//float weight = g_distance*g_distance*(-fdot);
	//pnewnode->G = parent_node->G + weight;


	int hidx = CMyMath::Pos_to_index(vpos , m_unit_stepsize);
	pnewnode->H = CMyMath::idx_distance(hidx , m_goalidx , m_tilecnt , m_unit_stepsize);
	//pnewnode->H = CMyMath::pos_distance(vpos , m_vGoal_pos);
	pnewnode->iCost = pnewnode->G + pnewnode->H;
	
	m_openlist->push_node(pnewnode);

	m_idxopenlist.insert(boost::unordered_map<int , PATH_NODE*>::value_type(pnewnode->index , pnewnode));
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
void CAstar::InitPos(const float& addx , const float& addy , const D3DXVECTOR2& vpos)
{
	m_vNode_pos = vpos;
	m_vNode_pos.x += addx;
	m_vNode_pos.y += addy;
}
void CAstar::UnitPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , const int& stepsize  , bool weightG)
{
	//if(16 == stepsize)
	//{
	//	m_maxnodecnt = MAXPATH_IDX / 2;
	//}
	//else if(32 == stepsize)
	//{
	//	m_maxnodecnt = MAXPATH_IDX;
	//}

	Release_unit_closelist();
	Release_unit_openlist();

	m_bweightG = weightG;

	//m_objid = objid;
	m_unit_stepsize = stepsize;
	if(16 == m_unit_stepsize)
		m_unit_diagonalstep = 22.f;
	else if(32 == m_unit_stepsize)
		m_unit_diagonalstep = 45.f;

	m_tilecnt = int(SQ_TILECNTX*(float)SQ_TILESIZEX/(float)stepsize);

	m_accumulate = 0;

	
	m_vStart_pos = startpos;
	m_vGoal_pos = goalpos;

	m_startindex = CMyMath::Pos_to_index(startpos.x , startpos.y , m_unit_stepsize);
	m_goalidx = CMyMath::Pos_to_index(goalpos.x , goalpos.y , m_unit_stepsize);

	PATH_NODE* pnode = &m_unitpath_pool[m_pathpool_idx];
	++m_pathpool_idx;

	pnode->G = 0;
	pnode->H = CMyMath::idx_distance(m_startindex , m_goalidx , m_tilecnt , m_unit_stepsize);
	//pnode->H = CMyMath::pos_distance(startpos , goalpos);
	pnode->iCost = pnode->G + pnode->H;

	pnode->index = m_startindex;
	pnode->vPos = startpos;
	pnode->pParent = NULL;

	//m_openlist.insert(multimap<int , PATH_NODE*>::value_type(pnode->iCost , pnode));
	m_openlist->push_node(pnode);
	m_idxopenlist.insert(boost::unordered_map<int , PATH_NODE*>::value_type(pnode->index , pnode));


	m_dummynode = pnode;
}
void CAstar::UnitPath_calculation_Update(const MYRECT<float>& vertex, vector<D3DXVECTOR2>& vecpath , const CObj* ptarget)
{

	PATH_NODE* pnode;

	MYRECT<float> temprect;
	float gdistance = 0;
	

	m_ptarget = ptarget;

	clock_t tbegin , tend;
	tbegin = clock();
	while(true)
	{
		tend = clock();
		if(tend - tbegin >= 2)
			break;

		pnode = m_openlist->pop_node();

		if(NULL != pnode)
			m_closelist.insert(boost::unordered_map<int , PATH_NODE*>::value_type(pnode->index , pnode));

		if(NULL == pnode || m_pathpool_idx >= MAXPATH_IDX - 1)
		{
			m_pathpool_idx = 0;
			//printf("²ËÂü\n");
			while(NULL != m_dummynode)
			{
				vecpath.push_back(m_dummynode->vPos);
				m_dummynode = m_dummynode->pParent;
			}
			break;
		}
			

		if(pnode->index == m_goalidx)
		{
			pnode->vPos = CMyMath::index_to_Pos(m_goalidx , m_tilecnt , m_unit_stepsize);

			while(NULL != pnode)
			{
				vecpath.push_back(pnode->vPos);
				pnode = pnode->pParent;
			}
			break;
		}

		if(m_bweightG)
		{
			if(m_dummynode->G <= pnode->G)
				m_dummynode = pnode;
		}
		else
		{

			if(m_dummynode->H > pnode->H)
				m_dummynode = pnode;
		}

		temprect.left = pnode->vPos.x - vertex.left;
		temprect.right = pnode->vPos.x + vertex.right;
		temprect.top = pnode->vPos.y - vertex.top;
		temprect.bottom = pnode->vPos.y + vertex.bottom;

		Init_eightpos_rect(pnode->vPos , temprect , m_unit_stepsize);

		
		int idx = 0;
		for(int i = 1; i < ASTAR_DIR_END; ++i)
		{
			if( i >= 5)
				gdistance = m_unit_diagonalstep;
			else
				gdistance = (float)m_unit_stepsize;

			if( Check_TileOption(m_eight_vpos[i]) &&
				Check_CloseList(m_eight_vpos[i]) &&	
				Check_OpenList(m_eight_vpos[i] , pnode) &&
				m_areamgr_inst->Check_Area(m_eight_rect[i] , m_eight_vpos[i] , m_eight_rect[CENTER] , m_eight_vpos[CENTER] , m_unit_stepsize , m_pObj , m_ptarget ))
			{
				Make_UnitNode(pnode , m_eight_vpos[i] , gdistance);	
			}
		}
	}
	//printf("Áö¿¬½Ã°£ %d , OBJ_ID = %d \n" , m_accumulate , m_objid);
	//printf("pool ÀÎµ¦½º %d , OBJ_ID = %d\n" , m_pathpool_idx , m_objid);

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
bool CAstar::Check_CloseList(const D3DXVECTOR2& vpos)
{
	int idx = CMyMath::Pos_to_index(vpos , m_unit_stepsize);

	if( m_closelist.end() != m_closelist.find(idx) )
		return false;

	return true;
}
bool CAstar::Check_OpenList(const D3DXVECTOR2& idxpos , PATH_NODE* parentnode)
{

	int idx = CMyMath::Pos_to_index( idxpos , m_unit_stepsize );
	boost::unordered_map<int , PATH_NODE*>::iterator iter = m_idxopenlist.find(idx);
	//map<int , PATH_NODE*>::iterator iter = m_idxopenlist.find(idx);

	if(iter != m_idxopenlist.end())
	{
		return false;
	}
	else
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
	if(!m_closelist.empty())
		m_closelist.clear();
}
void CAstar::Release_unit_openlist(void)
{
	m_openlist->Release();
	//m_openlist.clear();

	if(!m_idxopenlist.empty())
		m_idxopenlist.clear();


}

void CAstar::Path_Render(void)
{

	//const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile8x8");

	//D3DXMATRIX	tempmat;
	//D3DXMatrixIdentity(&tempmat);

	//if(!m_unit_path.empty())
	//{
	//	for(size_t i = 0; i < m_unit_path.size(); ++i)
	//	{
	//		tempmat._41 = m_unit_path[i].x - CScrollMgr::m_fScrollX;
	//		tempmat._42 = m_unit_path[i].y - CScrollMgr::m_fScrollY;

	//		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	//		if(0 == i)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
	//		else if( i == m_unit_path.size() - 1)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
	//		else
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	//	}

	//	//tempmat._41 = m_terrain_path[0].x - CScrollMgr::m_fScrollX;
	//	//tempmat._42 = m_terrain_path[0].y - CScrollMgr::m_fScrollY;
	//	//CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);
	//	//CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	//}

	//if(!m_closelist.empty())
	//{
	//	boost::unordered_map<int , PATH_NODE*>::iterator iter = m_closelist.begin();
	//	boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_closelist.end();

	//	for( ; iter != iter_end; ++iter)
	//	{
	//		tempmat._41 = iter->second->vPos.x - CScrollMgr::m_fScrollX;
	//		tempmat._42 = iter->second->vPos.y - CScrollMgr::m_fScrollY;

	//		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	//		CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,0));
	//	}
	//}

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