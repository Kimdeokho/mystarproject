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

//IMPLEMENT_SINGLETON(CAstar);
CAstar::CAstar(void)
{
	memset(m_terrain_idxopenlist , 0 , sizeof(m_terrain_idxopenlist));
	m_terrain_path.reserve(256);
	m_dummy_path.reserve(256);
	m_unit_path.reserve(256);

	m_terrain_openlist = new CMyHeapSort<PATH_NODE*>;
	m_openlist = new CMyHeapSort<PATH_NODE*>;
	//m_PathPool = new boost::pool<>(sizeof(PATH_NODE));

	m_releasecnt = 0;
	m_bPathUpdatestart = false;
	m_brelease_start = false;
	m_bdummy = false;
	m_brealpath = false;
	m_pObj = NULL;

	m_accumulate = 0;

	m_dummynode = NULL;

	m_fAstarTime = 0.f;
	m_fdummytime = 0.f;

	m_eight_weight[UP] = 32;
	m_eight_weight[DOWN] = 32;
	m_eight_weight[LEFT] = 32;
	m_eight_weight[RIGHT] = 32;
	m_eight_weight[LEFT_UP] = 45;
	m_eight_weight[RIGHT_UP] = 45;
	m_eight_weight[LEFT_DOWN] = 45;
	m_eight_weight[RIGHT_DOWN] = 45;

	m_funit_astartime = 0.f;

	m_areamgr_inst = CArea_Mgr::GetInstance();

	m_unitpath_pool = new PATH_NODE[MAXPATH_IDX];
	m_pathpool_idx = 0;
	m_objid = -1;
	m_unit_stepsize = 0;
	m_unit_diagonalstep = 0.f;

	m_bweightG = true;
}
CAstar::~CAstar(void)
{
	m_pObj = NULL;
	Release();
	Safe_Delete(m_terrain_openlist);
	Safe_Delete(m_openlist);
	//Safe_Delete(m_PathPool);

}
//for_each()
void CAstar::Initialize(CObj* pobj)
{
	m_pObj = pobj;
	m_ObjPos = m_pObj->GetReferencePos();
}
bool CAstar::Shortest_distance_check(const int& idx , PATH_NODE* pnode , const int& g)
{
	/*주위에 오픈리스트가 있을때 어느쪽이 시작점과 더 가까운지 검사한다.*/
	if(Check_TerrainOpenList(idx))
	{

		if(m_terrain_idxopenlist[idx]->G > pnode->G + g)
		{
			m_terrain_idxopenlist[idx]->G = pnode->G + g;
			m_terrain_idxopenlist[idx]->iCost = m_terrain_idxopenlist[idx]->G + m_terrain_idxopenlist[idx]->H;
			m_terrain_idxopenlist[idx]->pParent = pnode;			
		}

		return false;
	}
	else
		return true;
}
void CAstar::Make_TerrainNode(const int& idx , PATH_NODE* parent_node ,const int& g)
{
	PATH_NODE*	pnewnode = new PATH_NODE;//(PATH_NODE*)m_PathPool->malloc();

	pnewnode->pParent = parent_node;
	pnewnode->index = idx;

	//pnewnode->G = parent_node->G + g;
	//pnewnode->H = CMyMath::idx_distance(idx  , m_goalidx , m_tilecnt/*간격도 넣어야할듯*/);	
	pnewnode->iCost = CMyMath::pos_distance(m_vNode_pos , m_vGoal_pos);
	pnewnode->vPos = m_vNode_pos;

	m_terrain_openlist->push_node(pnewnode);
	m_terrain_idxopenlist[pnewnode->index] = pnewnode;
}
void CAstar::Make_UnitNode(PATH_NODE* parent_node ,const D3DXVECTOR2& vpos , const float& g_distance)
{

	int idx32 = CMyMath::Pos_to_index(vpos.x , vpos.y , 32);
	if( MOVE_NONE == CTileManager::GetInstance()->GetTileOption(idx32) || 
		m_pathpool_idx >= MAXPATH_IDX - 1)
		return;

	PATH_NODE* pnewnode = &m_unitpath_pool[m_pathpool_idx];
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
void CAstar::TerrainPath_calculation_Update(const D3DXVECTOR2& goalpos)
{
	Release_TerrainCloseList();

	if(false == m_bPathUpdatestart)
		return;

	m_tilecnt = 128;
	PATH_NODE* pnode = NULL;

	m_fAstarTime = 0;

	float tilesize = 32;

	while( m_fAstarTime <= 0.02f /*m_curloopcnt <= 10*/)
	{
		m_fAstarTime += GETTIME;
		
		++m_accumulate;


		pnode = m_terrain_openlist->pop_node();

		//if(m_fdummytime >= 1 && m_bdummy)
		//{
		//	m_bdummy = false;
		//	m_bPathUpdatestart = false;
		//	while(NULL != pnode)
		//	{
		//		m_dummy_path.push_back(pnode->vPos);
		//		pnode = pnode->pParent;
		//	}
		//	m_brealpath = false;
		//	m_pathidx = int(m_dummy_path.size() - 1);
		//	break; //여기 걸리면 그뒤로 A스타 그만해야함..
		//}

		if(NULL == pnode)
		{
			m_brelease_start = true;
			m_bPathUpdatestart = false;

			pnode = m_dummynode;
			while(NULL != pnode)
			{
				m_terrain_path.push_back(pnode->vPos);
				pnode = pnode->pParent;
			}
			m_brealpath = true;
			m_pathidx = int(m_terrain_path.size() - 1);
			break; //여기 걸리면 그뒤로 A스타 그만해야함..
		}

		//if(m_dummynode->H > pnode->H)
		//	m_dummynode = pnode;
		if(m_dummynode->iCost > pnode->iCost)
			m_dummynode = pnode;


		if(pnode->index == m_goalidx)
		{
			m_bdummy = false;
			m_brelease_start = true;
			m_bPathUpdatestart = false;

			while(NULL != pnode)
			{
				m_terrain_path.push_back(pnode->vPos);
				pnode = pnode->pParent;
			}
			m_brealpath = true;
			m_pathidx = int(m_terrain_path.size() - 1);
			break;
		}
		m_terrain_idxopenlist[pnode->index] = NULL;
		m_terrain_closelist.insert(make_pair(pnode->index , pnode));

		Init_eightidx(pnode->index);
		//업
		if(Check_TileOption(m_eight_idx[UP]) &&
			Check_TerrainCloseList(m_eight_idx[UP]))
		{
			InitPos(0 , -tilesize , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[UP] , pnode , m_eight_weight[UP]))
			{
				Make_TerrainNode(m_eight_idx[UP] , pnode , m_eight_weight[UP]/*대각은 45*/);
			}
		}
		//다운
		if(Check_TileOption(m_eight_idx[DOWN]) &&
			Check_TerrainCloseList(m_eight_idx[DOWN]))
		{
			InitPos(0 , tilesize , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[DOWN] , pnode , m_eight_weight[DOWN]))
			{
				Make_TerrainNode(m_eight_idx[DOWN] , pnode , m_eight_weight[DOWN]);
			}
		}
		//왼쪽
		if(Check_TileOption(m_eight_idx[LEFT]) &&
			Check_TerrainCloseList(m_eight_idx[LEFT]))
		{
			InitPos(-tilesize , 0 , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[LEFT] , pnode , m_eight_weight[LEFT]))
			{
				Make_TerrainNode(m_eight_idx[LEFT] , pnode , m_eight_weight[LEFT]);
			}
		}
		//오른쪽
		if(Check_TileOption(m_eight_idx[RIGHT]) &&
			Check_TerrainCloseList(m_eight_idx[RIGHT]))
		{
			InitPos(tilesize , 0 , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[RIGHT] , pnode , m_eight_weight[RIGHT]))
			{
				Make_TerrainNode(m_eight_idx[RIGHT] , pnode , m_eight_weight[RIGHT]);
			}
		}
		//위 왼
		if(Check_TileOption(m_eight_idx[LEFT_UP]) &&
			Check_TerrainCloseList(m_eight_idx[LEFT_UP]))
		{
			InitPos(-tilesize , -tilesize , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[LEFT_UP] , pnode , m_eight_weight[LEFT_UP]))
			{
				Make_TerrainNode(m_eight_idx[LEFT_UP] , pnode , m_eight_weight[LEFT_UP]);
			}
		}
		//위 오
		if(Check_TileOption(m_eight_idx[RIGHT_UP]) &&
			Check_TerrainCloseList(m_eight_idx[RIGHT_UP]))
		{
			InitPos(tilesize , -tilesize , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[RIGHT_UP] , pnode , m_eight_weight[RIGHT_UP]))
			{
				Make_TerrainNode(m_eight_idx[RIGHT_UP] , pnode , m_eight_weight[RIGHT_UP]);
			}
		}
		//아래 왼
		if(Check_TileOption(m_eight_idx[LEFT_DOWN]) &&
			Check_TerrainCloseList(m_eight_idx[LEFT_DOWN]))
		{
			InitPos(-tilesize , tilesize , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[LEFT_DOWN] , pnode , m_eight_weight[LEFT_DOWN]))
			{
				Make_TerrainNode(m_eight_idx[LEFT_DOWN] , pnode , m_eight_weight[LEFT_DOWN]);
			}
		}
		//아래 오
		if(Check_TileOption(m_eight_idx[RIGHT_DOWN]) &&
			Check_TerrainCloseList(m_eight_idx[RIGHT_DOWN]))
		{
			InitPos(tilesize , tilesize , pnode->vPos);
			if(Shortest_distance_check(m_eight_idx[RIGHT_DOWN] , pnode , m_eight_weight[RIGHT_DOWN]))
			{
				Make_TerrainNode(m_eight_idx[RIGHT_DOWN] , pnode , m_eight_weight[RIGHT_DOWN]);		
			}
		}
	}
	
	//printf("%d\n" , m_accumulate);
	//printf("%f\n" , m_fdummytime);
}
void CAstar::TerrainPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos)
{

	m_fdummytime = 0.f;
	m_fAstarTime = 0.f;

	m_vStart_pos = startpos;
	m_vGoal_pos = goalpos;
	m_dummynode = NULL;
	m_accumulate = 0;

	//m_bdummy = bdummypath;

	Release_TerrainPath();

	m_bPathUpdatestart = true;
	/*지형 경로를 구한다.*/
	m_tilecnt = 128;
	m_startindex = CMyMath::Pos_to_index(startpos.x , startpos.y ,32);
	m_goalidx = CMyMath::Pos_to_index(goalpos.x , goalpos.y ,32);

	PATH_NODE* pnode = new PATH_NODE;//(PATH_NODE*)m_PathPool->malloc();
	pnode->G = 0;
	pnode->H = CMyMath::pos_distance(m_vNode_pos , m_vGoal_pos);
	pnode->iCost = pnode->H;
	pnode->pParent = NULL;
	pnode->index = m_startindex;
	pnode->vPos = startpos;

	m_terrain_openlist->push_node(pnode);
	//m_terrain_openlist.insert(make_pair(pnode->iCost , pnode));
	m_terrain_idxopenlist[pnode->index] = pnode;
}
void CAstar::UnitPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , const int& stepsize , const int& objid , bool weightG)
{
	Release_unit_closelist();
	Release_UnitPath();

	m_bweightG = weightG;

	m_objid = objid;
	m_unit_stepsize = stepsize;
	m_unit_diagonalstep = (sqrtf((float)(stepsize*stepsize + stepsize*stepsize)));
	m_tilecnt = SQ_TILECNTX*(SQ_TILESIZEX/stepsize);

	m_accumulate = 0;

	
	m_vStart_pos = startpos;
	m_vGoal_pos = goalpos;

	m_startindex = CMyMath::Pos_to_index(startpos.x , startpos.y , m_unit_stepsize);
	m_goalidx = CMyMath::Pos_to_index(goalpos.x , goalpos.y , m_unit_stepsize);

	//PATH_NODE* pnode = new PATH_NODE;
	//PATH_NODE* pnode = (PATH_NODE*)m_PathPool->malloc();
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
void CAstar::UnitPath_calculation_Update(const MYRECT<float>& vertex, vector<D3DXVECTOR2>& vecpath)
{

	PATH_NODE* pnode;

	//MYRECT<float>	vtx;
	MYRECT<float> temprect;

	//vtx.left = (rect.right - rect.left)/2;
	//vtx.right = (rect.right - rect.left)/2;
	//vtx.bottom = (rect.bottom - rect.top)/2;
	//vtx.top = (rect.bottom - rect.top)/2;

	//vtx.left = vertex.left;
	//vtx.right = vertex.right;
	//vtx.bottom = vertex.bottom;
	//vtx.top = vertex.top;
	
	clock_t tbegin , tend;
	tbegin = clock();

	//pdummynode = m_openlist.begin()->second;
	//pdummynode = m_openlist->pop_node();

	//boost::unordered_map<int , PATH_NODE*>::iterator iter;


	while(true)
	{
		tend = clock();
		if(tend - tbegin >= 1)
			break;



		pnode = m_openlist->pop_node();
		//pnode = m_openlist.begin()->second;
		//m_openlist.erase(m_openlist.begin());

		if(NULL != pnode)
			m_closelist.insert(boost::unordered_map<int , PATH_NODE*>::value_type(pnode->index , pnode));

		if(NULL == pnode || m_pathpool_idx >= MAXPATH_IDX - 1)
		{
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

		float gdistance = 0;
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
				m_areamgr_inst->Check_Area(m_eight_rect[i] , m_eight_vpos[i] , m_eight_rect[CENTER] , m_eight_vpos[CENTER] , m_unit_stepsize , m_objid , (ASTAR_DIR)i))
			{
				Make_UnitNode(pnode , m_eight_vpos[i] , gdistance);	
			}
		}
	}
	//printf("지연시간 %d , OBJ_ID = %d \n" , m_accumulate , m_objid);
	//printf("pool 인덱스 %d , OBJ_ID = %d\n" , m_pathpool_idx , m_objid);

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

bool CAstar::Search_surrounding_units(const MYRECT<float>& rect ,D3DXVECTOR2& vpos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos)
{
	/*주위 유닛과 충돌이 일어나는지 검사한다*/

	return true;//m_areamgr_inst->Check_Area(rect , vpos , center_rc , vcenterpos);
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

	if(iter != m_idxopenlist.end())
	{
		//if( parentnode->G + g_distance < iter->second->G )
		//{
		//	iter->second->G = parentnode->G + g_distance;
		//	iter->second->iCost = iter->second->H + iter->second->G;
		//	iter->second->pParent = parentnode;

		//	multimap<int , PATH_NODE*>::iterator open_iter = m_openlist.begin();
		//	multimap<int , PATH_NODE*>::iterator open_iter_end = m_openlist.end();
		//	for( ; open_iter != open_iter_end; ++open_iter)
		//	{
		//		if(iter->second == open_iter->second)
		//		{
		//			m_openlist.erase(open_iter);
		//			break;
		//		}
		//	}
		//	m_openlist.insert(multimap<int, PATH_NODE*>::value_type(iter->second->iCost , iter->second));
		//}
		return false;
	}
	else
		return true;
}

bool CAstar::Check_TerrainOpenList(const int& idx)
{
	//if(true == m_terrain_idxopenlist[idx])
	//	return true;
	//else
	//	return false;

	if(NULL != m_terrain_idxopenlist[idx])
		return true;
	else
		return false;
}

bool CAstar::Check_TerrainCloseList(const int& idx)
{
	if(idx < 0)
		return false;

	if(!m_terrain_closelist.empty())
	{
		if(m_terrain_closelist.end() != m_terrain_closelist.find(idx))
			return false;
	}
	return true;
}

vector<D3DXVECTOR2>* CAstar::GetTerrain_Path(void)
{
	return &m_terrain_path;
}
vector<D3DXVECTOR2>* CAstar::Getdummy_Path(void)
{
	return &m_dummy_path;
}
PATH_NODE*	CAstar::GetNode(const int& idx)
{
	if(!m_terrain_closelist.empty())
	{
		boost::unordered_map<int , PATH_NODE*>::iterator iter = m_terrain_closelist.find(idx);
		if(m_terrain_closelist.end() != iter)
			return iter->second;
	}

	return NULL;
}
vector<D3DXVECTOR2>* CAstar::GetUnit_Path(void)
{
	return &m_unit_path;
}
void CAstar::Release(void)
{
	Release_TerrainPath();
	Release_unit_closelist();
	Release_UnitPath();

	delete[] m_unitpath_pool;
	m_unitpath_pool = NULL;
}
void CAstar::Release_TerrainCloseList(void)
{
	if(GETTIME > 0.008f)
		return;

	if(!m_brelease_start)
		return;

	if(!m_terrain_closelist.empty())
	{
		m_releasecnt = 0;

		boost::unordered_map<int , PATH_NODE*>::iterator iter = m_terrain_closelist.begin();
		boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_terrain_closelist.end();
		for( ; iter != iter_end;)
		{
			if(m_releasecnt > 10)
				break;

			delete iter->second;
			iter->second = NULL;

			iter = m_terrain_closelist.erase(iter);

			++m_releasecnt;
		}
	}
	else
		m_brelease_start = false;

	//m_terrain_openlist->Release(m_terrain_idxopenlist , m_PathPool);
	m_terrain_openlist->Release();

}
void CAstar::Release_TerrainPath(void)
{
	if(!m_terrain_closelist.empty())
	{
		boost::unordered_map<int , PATH_NODE*>::iterator iter = m_terrain_closelist.begin();
		boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_terrain_closelist.end();
		for( ; iter != iter_end; ++iter)
		{
			//m_PathPool->free(iter->second);
			delete iter->second;
			iter->second = NULL;
		}
		m_terrain_closelist.clear();
	}

	m_terrain_openlist->Release();

	if(!m_terrain_path.empty())
		m_terrain_path.clear();

	if(!m_dummy_path.empty())
		m_dummy_path.clear();
}
void CAstar::Release_unit_closelist(void)
{
	m_pathpool_idx = 0;
	if(!m_closelist.empty())
		m_closelist.clear();
}
void CAstar::Release_UnitPath(void)
{
	m_openlist->Release();
	//m_openlist.clear();

	if(!m_idxopenlist.empty())
		m_idxopenlist.clear();


	if(!m_unit_path.empty())
		m_unit_path.clear();
}

void CAstar::Path_Render(void)
{

	const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile8x8");

	D3DXMATRIX	tempmat;
	D3DXMatrixIdentity(&tempmat);

	if(!m_unit_path.empty())
	{
		for(size_t i = 0; i < m_unit_path.size(); ++i)
		{
			tempmat._41 = m_unit_path[i].x - CScrollMgr::m_fScrollX;
			tempmat._42 = m_unit_path[i].y - CScrollMgr::m_fScrollY;

			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			if(0 == i)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
			else if( i == m_unit_path.size() - 1)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
			else
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
		}

		//tempmat._41 = m_terrain_path[0].x - CScrollMgr::m_fScrollX;
		//tempmat._42 = m_terrain_path[0].y - CScrollMgr::m_fScrollY;
		//CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);
		//CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	}

	if(!m_closelist.empty())
	{
		boost::unordered_map<int , PATH_NODE*>::iterator iter = m_closelist.begin();
		boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_closelist.end();

		for( ; iter != iter_end; ++iter)
		{
			tempmat._41 = iter->second->vPos.x - CScrollMgr::m_fScrollX;
			tempmat._42 = iter->second->vPos.y - CScrollMgr::m_fScrollY;

			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,0));
		}
	}

	PATH_NODE* pnode = NULL;
	int idx = 0;
	while(true)
	{
		pnode = m_openlist->Render(idx);
		if(NULL == pnode)
			break;

		++idx;

		tempmat._41 = pnode->vPos.x - CScrollMgr::m_fScrollX;
		tempmat._42 = pnode->vPos.y - CScrollMgr::m_fScrollY;

		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

		CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
}