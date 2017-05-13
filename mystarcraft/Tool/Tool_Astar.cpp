#include "StdAfx.h"
#include "Tool_Astar.h"

#include "Tool_MyMath.h"
#include "TileMgr.h"
#include "Tool_MyHeapSort.h"

#include "Device.h"
#include "TextureMgr.h"
//IMPLEMENT_SINGLETON(CAstar);
CTool_Astar::CTool_Astar(void)
{
	memset(m_terrain_idxopenlist , 0 , sizeof(m_terrain_idxopenlist));
	memset(m_pathidx , 0 , sizeof(m_pathidx));

	m_terrain_path.reserve(256);
	m_unit_path.reserve(256);

	m_terrain_openlist = new CTool_MyHeapSort;
	m_PathPool = new boost::pool<>(sizeof(PATH_NODE));

	m_releasecnt = 0;
	m_bPathUpdatestart = false;
	m_brelease_start = false;
	m_bdummy = false;

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

}
CTool_Astar::~CTool_Astar(void)
{
	Release();
	Safe_Delete(m_terrain_openlist);
	Safe_Delete(m_PathPool);

}
//for_each()
void CTool_Astar::Initialize(void)
{
}
bool CTool_Astar::Shortest_distance_check(const int& idx , PATH_NODE* pnode , const int& g)
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
void CTool_Astar::Make_TerrainNode(const int& idx , PATH_NODE* parent_node ,const int& g)
{
	PATH_NODE*	pnewnode = (PATH_NODE*)m_PathPool->malloc();

	pnewnode->pParent = parent_node;
	pnewnode->index = idx;

	pnewnode->G = parent_node->G + g;
	pnewnode->H = CTool_MyMath::idx_distance(idx , m_goalidx , m_tilecnt);
	pnewnode->iCost = pnewnode->G + pnewnode->H;
	pnewnode->vPos = m_vNode_pos;

	m_terrain_openlist->push_node(pnewnode);
;
	m_terrain_idxopenlist[pnewnode->index] = pnewnode;
}
void CTool_Astar::Init_eightidx(const int& idx)
{
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
void CTool_Astar::InitPos(const float& addx , const float& addy , const D3DXVECTOR2& vpos)
{
	m_vNode_pos = vpos;
	m_vNode_pos.x += addx;
	m_vNode_pos.y += addy;
}
void CTool_Astar::Path_calculation_Update(const D3DXVECTOR2& goalpos)
{
	m_tilecnt = 128;

	PATH_NODE* pnode = NULL;

	m_fAstarTime = 0;

	while(true)
	{
		pnode = m_terrain_openlist->pop_node();

		if(NULL == m_dummynode)
			m_dummynode = pnode;


		if(NULL == pnode)
		{
			pnode = m_dummynode;
			while(NULL != pnode)
			{
				m_terrain_path.push_back(pnode->vPos);
				pnode = pnode->pParent;
			}
			break; //여기 걸리면 그뒤로 A스타 그만해야함..
		}

		if(m_dummynode->H > pnode->H)
			m_dummynode = pnode;



		if(pnode->index == m_goalidx)
		{
			m_brelease_start = true;
			m_bPathUpdatestart = false;

			while(NULL != pnode)
			{
				m_terrain_path.push_back(pnode->vPos);
				pnode = pnode->pParent;
			}
			break;
		}
		m_terrain_idxopenlist[pnode->index] = NULL;
		m_terrain_closelist.insert(make_pair(pnode->index , pnode));

		Init_eightidx(pnode->index);
		//업
		if(Check_TileOption(m_eight_idx[UP]) &&
			Check_TerrainCloseList(m_eight_idx[UP]))
		{
			if(Shortest_distance_check(m_eight_idx[UP] , pnode , m_eight_weight[UP]))
			{
				Make_TerrainNode(m_eight_idx[UP] , pnode , m_eight_weight[UP]/*대각은 45*/);
			}
		}
		//다운
		if(Check_TileOption(m_eight_idx[DOWN]) &&
			Check_TerrainCloseList(m_eight_idx[DOWN]))
		{
			if(Shortest_distance_check(m_eight_idx[DOWN] , pnode , m_eight_weight[DOWN]))
			{
				Make_TerrainNode(m_eight_idx[DOWN] , pnode , m_eight_weight[DOWN]);
			}
		}
		//왼쪽
		if(Check_TileOption(m_eight_idx[LEFT]) &&
			Check_TerrainCloseList(m_eight_idx[LEFT]))
		{
			if(Shortest_distance_check(m_eight_idx[LEFT] , pnode , m_eight_weight[LEFT]))
			{
				Make_TerrainNode(m_eight_idx[LEFT] , pnode , m_eight_weight[LEFT]);
			}
		}
		//오른쪽
		if(Check_TileOption(m_eight_idx[RIGHT]) &&
			Check_TerrainCloseList(m_eight_idx[RIGHT]))
		{
			if(Shortest_distance_check(m_eight_idx[RIGHT] , pnode , m_eight_weight[RIGHT]))
			{
				Make_TerrainNode(m_eight_idx[RIGHT] , pnode , m_eight_weight[RIGHT]);
			}
		}
		//위 왼
		if(Check_TileOption(m_eight_idx[LEFT_UP]) &&
			Check_TerrainCloseList(m_eight_idx[LEFT_UP]))
		{
			if(Shortest_distance_check(m_eight_idx[LEFT_UP] , pnode , m_eight_weight[LEFT_UP]))
			{
				Make_TerrainNode(m_eight_idx[LEFT_UP] , pnode , m_eight_weight[LEFT_UP]);
			}
		}
		//위 오
		if(Check_TileOption(m_eight_idx[RIGHT_UP]) &&
			Check_TerrainCloseList(m_eight_idx[RIGHT_UP]))
		{
			if(Shortest_distance_check(m_eight_idx[RIGHT_UP] , pnode , m_eight_weight[RIGHT_UP]))
			{
				Make_TerrainNode(m_eight_idx[RIGHT_UP] , pnode , m_eight_weight[RIGHT_UP]);
			}
		}
		//아래 왼
		if(Check_TileOption(m_eight_idx[LEFT_DOWN]) &&
			Check_TerrainCloseList(m_eight_idx[LEFT_DOWN]))
		{
			if(Shortest_distance_check(m_eight_idx[LEFT_DOWN] , pnode , m_eight_weight[LEFT_DOWN]))
			{
				Make_TerrainNode(m_eight_idx[LEFT_DOWN] , pnode , m_eight_weight[LEFT_DOWN]);
			}
		}
		//아래 오
		if(Check_TileOption(m_eight_idx[RIGHT_DOWN]) &&
			Check_TerrainCloseList(m_eight_idx[RIGHT_DOWN]))
		{
			if(Shortest_distance_check(m_eight_idx[RIGHT_DOWN] , pnode , m_eight_weight[RIGHT_DOWN]))
			{
				Make_TerrainNode(m_eight_idx[RIGHT_DOWN] , pnode , m_eight_weight[RIGHT_DOWN]);		
			}
		}
	}
}
void CTool_Astar::Path_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos, bool bdummypath)
{
	m_fdummytime = 0.f;
	m_fAstarTime = 0.f;

	m_vStart_pos = startpos;
	m_vGoal_pos = goalpos;
	m_dummynode = NULL;
	m_accumulate = 0;

	m_bdummy = bdummypath;

	Release_TerrainPath();

	m_bPathUpdatestart = true;
	/*지형 경로를 구한다.*/
	m_tilecnt = 128;
	m_startindex = CTool_MyMath::Pos_to_index(startpos.x , startpos.y ,32);
	m_goalidx = CTool_MyMath::Pos_to_index(goalpos.x , goalpos.y ,32);

	PATH_NODE* pnode = (PATH_NODE*)m_PathPool->malloc();
	pnode->G = 0;
	pnode->H = CTool_MyMath::idx_distance(m_startindex , m_goalidx , m_tilecnt);
	pnode->iCost = pnode->H;
	pnode->pParent = NULL;
	pnode->index = m_startindex;
	pnode->vPos = startpos;

	m_terrain_openlist->push_node(pnode);
	m_terrain_idxopenlist[pnode->index] = pnode;
}

bool CTool_Astar::Check_TileOption(const int& idx)
{
	if(idx < 0)
		return false;

	int option = CTileMgr::GetInstance()->GetTileOption(idx);

	if(MOVE_NONE == option)
		return false;
	else if(option < 0)
		return false;
	else
		return true;

}

bool CTool_Astar::Check_TerrainOpenList(const int& idx)
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

bool CTool_Astar::Check_TerrainCloseList(const int& idx)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return false;

	if(!m_terrain_closelist.empty())
	{
		if(m_terrain_closelist.end() != m_terrain_closelist.find(idx))
			return false;
	}
	return true;
}
void CTool_Astar::Release(void)
{
	Release_TerrainPath();
}
void CTool_Astar::Release_TerrainPath(void)
{
	if(!m_terrain_closelist.empty())
	{
		boost::unordered_map<int , PATH_NODE*>::iterator iter = m_terrain_closelist.begin();
		boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_terrain_closelist.end();
		for( ; iter != iter_end; ++iter)
		{
			//Safe_Delete(iter->second);
			m_PathPool->free(iter->second);
			iter->second = NULL;
		}
		m_terrain_closelist.clear();
	}

	m_terrain_openlist->Release(m_terrain_idxopenlist , m_PathPool);

	if(!m_terrain_path.empty())
		m_terrain_path.clear();

}

void CTool_Astar::Path_Render(void)
{
	//const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	//D3DXMATRIX	tempmat;
	//D3DXMatrixIdentity(&tempmat);

	//if(!m_terrain_path.empty())
	//{
	//	for(size_t i = 0; i < m_terrain_path.size(); ++i)
	//	{
	//		tempmat._41 = m_terrain_path[i].x - CScrollMgr::m_fScrollX;
	//		tempmat._42 = m_terrain_path[i].y - CScrollMgr::m_fScrollY;
	//		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	//		if(0 == i)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
	//		else if( i == m_terrain_path.size() - 1)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
	//		else
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	//	}

	//	//tempmat._41 = m_terrain_path[0].x - CScrollMgr::m_fScrollX;
	//	//tempmat._42 = m_terrain_path[0].y - CScrollMgr::m_fScrollY;
	//	//CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);
	//	//CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	//}
	
	
}

void CTool_Astar::Waypoint_calculation(void)
{
	int maxloop = SQ_TILECNTX*SQ_TILECNTY;
}
