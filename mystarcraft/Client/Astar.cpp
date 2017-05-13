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

	m_terrain_openlist = new CMyHeapSort;
	m_openlist = new CMyHeapSort;
	m_PathPool = new boost::pool<>(sizeof(PATH_NODE));

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
}
CAstar::~CAstar(void)
{
	Release();
	Safe_Delete(m_terrain_openlist);
	Safe_Delete(m_openlist);
	Safe_Delete(m_PathPool);

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
	PATH_NODE*	pnewnode = (PATH_NODE*)m_PathPool->malloc();

	pnewnode->pParent = parent_node;
	pnewnode->index = idx;

	pnewnode->G = parent_node->G + g;
	pnewnode->H = CMyMath::idx_distance(idx , m_goalidx , m_tilecnt);
	pnewnode->iCost = pnewnode->G + pnewnode->H;
	pnewnode->vPos = m_vNode_pos;

	m_terrain_openlist->push_node(pnewnode);
;
	m_terrain_idxopenlist[pnewnode->index] = pnewnode;
}
void CAstar::Make_UnitNode(const int& idx , PATH_NODE* parent_node ,const int& g)
{
	//PATH_NODE*	pnewnode = new PATH_NODE;
	PATH_NODE*	pnewnode =(PATH_NODE*)m_PathPool->malloc();

	pnewnode->pParent = parent_node;
	pnewnode->index = idx;
	pnewnode->G = parent_node->G + g;
	pnewnode->H = CMyMath::idx_distance(idx  , m_goalidx , m_tilecnt/*간격도 넣어야할듯*/);	
	pnewnode->vPos = m_vNode_pos;
	pnewnode->iCost = CMyMath::pos_distance(m_vNode_pos , m_vGoal_pos);//pnewnode->G + pnewnode->H;


	m_openlist->push_node(pnewnode);
	m_idxopenlist.insert(pnewnode->index);
}
void CAstar::Init_eightidx(const int& idx)
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
void CAstar::InitPos(const float& addx , const float& addy , const D3DXVECTOR2& vpos)
{
	m_vNode_pos = vpos;
	m_vNode_pos.x += addx;
	m_vNode_pos.y += addy;
}
void CAstar::Path_calculation_Update(const D3DXVECTOR2& goalpos)
{
	Release_TerrainCloseList();

	if(false == m_bPathUpdatestart)
		return;

	m_tilecnt = 128;
	PATH_NODE* pnode = NULL;

	m_fAstarTime = 0;

	float tilesize = 32;

	m_fdummytime += GETTIME;
	while( m_fAstarTime <= 0.02f /*m_curloopcnt <= 10*/)
	{
		m_fAstarTime += GETTIME;
		
		++m_accumulate;


		pnode = m_terrain_openlist->pop_node();

		if(NULL == m_dummynode)
			m_dummynode = pnode;


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
			break; //여기 걸리면 그뒤로 A스타 그만해야함..
		}

		if(m_dummynode->H > pnode->H)
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
void CAstar::Path_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos, bool bdummypath)
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
	m_startindex = CMyMath::Pos_to_index(startpos.x , startpos.y ,32);
	m_goalidx = CMyMath::Pos_to_index(goalpos.x , goalpos.y ,32);

	PATH_NODE* pnode = (PATH_NODE*)m_PathPool->malloc();
	pnode->G = 0;
	pnode->H = CMyMath::idx_distance(m_startindex , m_goalidx , m_tilecnt);
	pnode->iCost = pnode->H;
	pnode->pParent = NULL;
	pnode->index = m_startindex;
	pnode->vPos = startpos;

	m_terrain_openlist->push_node(pnode);
	//m_terrain_openlist.insert(make_pair(pnode->iCost , pnode));
	m_terrain_idxopenlist[pnode->index] = pnode;
}
void CAstar::Path_calculation(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , const MYRECT<float>& rect , const int& objid)
{

	Release_UnitPath();

	m_tilecnt = 512;
	int idx64 = CMyMath::Pos_to_index(startpos.x , startpos.y , 64);
	m_startindex = CMyMath::Pos_to_index(startpos.x , startpos.y , 8);
	m_goalidx = CMyMath::Pos_to_index(goalpos.x , goalpos.y , 8);

	//PATH_NODE* pnode = new PATH_NODE;
	PATH_NODE* pnode = (PATH_NODE*)m_PathPool->malloc();
	pnode->index = m_startindex;
	pnode->vPos = startpos;

	m_openlist->push_node(pnode);

	CArea_Mgr::GetInstance()->SetObjId(objid);

	while(true)
	{
		pnode = m_openlist->pop_node();
		m_closelist.insert(boost::unordered_map<int , PATH_NODE*>::value_type(pnode->index , pnode));

		if(pnode->index == m_goalidx)
			break;


		Init_eightidx(pnode->index);
		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[UP]) &&
			Check_OpenList(m_eight_idx[UP]) &&
			Search_surrounding_units(UP , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[UP] , pnode , 8);	
		}

		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[DOWN]) &&
			Check_OpenList(m_eight_idx[DOWN]) &&
			Search_surrounding_units(DOWN , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[DOWN] , pnode , 8);
		}

		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[LEFT]) &&
			Check_OpenList(m_eight_idx[LEFT]) &&
			Search_surrounding_units(LEFT , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[LEFT] , pnode , 8);
		}

		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[RIGHT]) &&
			Check_OpenList(m_eight_idx[RIGHT]) &&
			Search_surrounding_units(RIGHT , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[RIGHT] , pnode , 8);
		}

		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[RIGHT_DOWN]) &&
			Check_OpenList(m_eight_idx[RIGHT_DOWN]) &&
			Search_surrounding_units(RIGHT_DOWN , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[RIGHT_DOWN] , pnode , 11);
		}

		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[RIGHT_UP]) &&
			Check_OpenList(m_eight_idx[RIGHT_UP]) &&
			Search_surrounding_units(RIGHT_UP , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[RIGHT_UP] , pnode , 11);
		}

		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[LEFT_DOWN]) &&
			Check_OpenList(m_eight_idx[LEFT_DOWN]) &&
			Search_surrounding_units(LEFT_DOWN , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[LEFT_DOWN] , pnode , 11);
		}

		m_vNode_pos = pnode->vPos;
		if( Check_CloseList(m_eight_idx[LEFT_UP]) &&
			Check_OpenList(m_eight_idx[LEFT_UP]) &&
			Search_surrounding_units(LEFT_UP , idx64 , rect , m_vNode_pos) )
		{
			Make_UnitNode(m_eight_idx[LEFT_UP] , pnode , 11);
		}
	}

	while(NULL != pnode)
	{
		m_unit_path.push_back(pnode->vPos);
		pnode = pnode->pParent;
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

bool CAstar::Search_surrounding_units(ASTAR_DIR edir , const int& idx/*인덱스64여야 한다.*/, const MYRECT<float>& rect ,D3DXVECTOR2& vpos)
{
	/*주위 유닛과 충돌이 일어나는지 검사한다*/

	if(CArea_Mgr::GetInstance()->Check_Area(edir , idx , rect , vpos))
		return true;
	else
		return false;

}
bool CAstar::Check_CloseList(const int& idx)
{
	if( m_closelist.end() != m_closelist.find(idx) )
		return false;

	return true;
}
bool CAstar::Check_OpenList(const int& idx)
{
	if(m_idxopenlist.end() != m_idxopenlist.find(idx))
		return false;
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
const vector<D3DXVECTOR2>* CAstar::GetUnit_Path(void)
{
	return &m_unit_path;
}
void CAstar::Release(void)
{
	Release_TerrainPath();
	Release_UnitPath();
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

			//Safe_Delete(iter->second);
			m_PathPool->free(iter->second);
			iter->second = NULL;

			iter = m_terrain_closelist.erase(iter);

			++m_releasecnt;
		}
	}
	else
		m_brelease_start = false;

	m_terrain_openlist->Release(m_terrain_idxopenlist , m_PathPool);

}
void CAstar::Release_TerrainPath(void)
{
	if(!m_terrain_closelist.empty())
	{
		boost::unordered_map<int , PATH_NODE*>::iterator iter = m_terrain_closelist.begin();
		boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_terrain_closelist.end();
		for( ; iter != iter_end; ++iter)
		{
			m_PathPool->free(iter->second);
			iter->second = NULL;
		}
		m_terrain_closelist.clear();
	}

	m_terrain_openlist->Release(m_terrain_idxopenlist , m_PathPool);

	if(!m_terrain_path.empty())
		m_terrain_path.clear();

	if(!m_dummy_path.empty())
		m_dummy_path.clear();
}
void CAstar::Release_UnitPath(void)
{
	if(!m_closelist.empty())
	{
		boost::unordered_map<int , PATH_NODE*>::iterator iter = m_closelist.begin();
		boost::unordered_map<int , PATH_NODE*>::iterator iter_end = m_closelist.end();
		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete(iter->second);
		}
		m_closelist.clear();
	}

	m_idxopenlist.clear();
	m_openlist->Release();

	if(!m_unit_path.empty())
		m_unit_path.clear();
}

void CAstar::Path_Render(void)
{
	const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	D3DXMATRIX	tempmat;
	D3DXMatrixIdentity(&tempmat);

	if(!m_terrain_path.empty())
	{
		for(size_t i = 0; i < m_terrain_path.size(); ++i)
		{
			tempmat._41 = m_terrain_path[i].x - CScrollMgr::m_fScrollX;
			tempmat._42 = m_terrain_path[i].y - CScrollMgr::m_fScrollY;
			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			if(0 == i)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
			else if( i == m_terrain_path.size() - 1)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
			else
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
		}

		//tempmat._41 = m_terrain_path[0].x - CScrollMgr::m_fScrollX;
		//tempmat._42 = m_terrain_path[0].y - CScrollMgr::m_fScrollY;
		//CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);
		//CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	}
	

	if(!m_dummy_path.empty())
	{
		for(size_t i = 0; i < m_dummy_path.size(); ++i)
		{
			tempmat._41 = m_dummy_path[i].x - CScrollMgr::m_fScrollX;
			tempmat._42 = m_dummy_path[i].y - CScrollMgr::m_fScrollY;
			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			if(0 == i)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
			else if( i == m_dummy_path.size() - 1)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
			else
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
		}
	}
}
void CAstar::MoveUpdate(void)
{
	if(m_brealpath)
	{
		if(!m_terrain_path.empty())
		{
			D3DXVECTOR2 curpos;
			D3DXVECTOR2 nextpos;
			D3DXVECTOR2 vnormal;
			if( 0 != m_pathidx)
			{
				curpos = (m_terrain_path)[m_pathidx];
				nextpos = (m_terrain_path)[m_pathidx-1];

				D3DXVec2Normalize(&vnormal , &(nextpos - curpos));

				m_pObj->GetReferencePos() += vnormal*GETTIME*500;

				if(CMyMath::pos_distance(m_pObj->GetPos() , nextpos) < 30*30)
				{
					--m_pathidx;
				}
			}
		}

	}
}
