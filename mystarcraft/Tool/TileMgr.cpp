#include "StdAfx.h"
#include "TileMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"

#include "ToolView.h"
#include "TileDebug.h"
#include "Rewind.h"

IMPLEMENT_SINGLETON(CTileMgr)
CTileMgr::CTileMgr(void)
{
	m_pToolView = NULL;
	m_rbRender = false;
	m_rbidx = -1;
	m_sqidx = -1;
}

CTileMgr::~CTileMgr(void)
{
	Release();
}

void CTileMgr::InitTile(void)
{

	m_pToolView = ((CMainFrame*)AfxGetMainWnd())->m_pToolView;

	m_sqTile.reserve(SQ_TILECNTX*SQ_TILECNTY);
	m_rbTile.reserve(RB_TILECNTX*RB_TILECNTY);

	m_DirtTex.reserve(GROUP_END);
	m_DirtTex.resize(GROUP_END);
	m_HighDirtTex.reserve(GROUP_END);
	m_HighDirtTex.resize(GROUP_END);


	m_DirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetStateTexture(L"Dirt" , L"FLAT");

	m_HighDirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"FLAT");
	m_HighDirtTex[GROUP_L] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"L");
	m_HighDirtTex[GROUP_LU] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"LU");
	m_HighDirtTex[GROUP_RU] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"RU");
	m_HighDirtTex[GROUP_R] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"R");
	m_HighDirtTex[GROUP_RD] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"RD");
	m_HighDirtTex[GROUP_LD] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"LD");


	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			int iindex = i*SQ_TILECNTX + j;

			TILE*	ptile = new TILE();
			ptile->vPos.x = (float)(SQ_TILESIZEX/2 + j*SQ_TILESIZEX);
			ptile->vPos.y = (float)(SQ_TILESIZEY/2 + i*SQ_TILESIZEY);

			TERRAIN_INFO* pterrain_info = new TERRAIN_INFO;
			pterrain_info->byTerrain_ID = TERRAIN_HIGHDIRT;
			int group_id = pterrain_info->byGroup_ID;
			pterrain_info->byGroup_sequence = rand()%m_DirtTex[group_id]->size();

			ptile->terrainList.push_back(pterrain_info);

			m_sqTile.push_back(ptile);
		}
	}
	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			int iindex = i*SQ_TILECNTX + j;
			CRewind::GetInstance()->InitStackTile(m_sqTile[iindex]->terrainList , iindex);
		}
	}


	CTileDebug::GetInstance()->SetTile(&m_sqTile);

	for(int i = 0; i < RB_TILECNTY; ++i)
	{
		for(int j = 0; j < RB_TILECNTX; ++j)
		{
			D3DXVECTOR2	vPos;

			if(i%2 == 0)
				vPos = D3DXVECTOR2( float(j*RB_TILESIZEX) , float(i*RB_TILESIZEY)/2 );
			else
				vPos = D3DXVECTOR2( float(RB_TILESIZEX/2) + float(j*RB_TILESIZEX) , float(i*RB_TILESIZEY)/2 );

			m_rbTile.push_back(vPos);
		}
	}
}
void CTileMgr::MinimapRender(void)
{
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity(&matWorld);
	TEXINFO*	pTexture = NULL;
	int iindex = 0;
	list<TERRAIN_INFO*>::iterator iter;

	for(int i = 0; i < SQ_TILECNTX; ++i)
	{
		for(int j = 0; j < SQ_TILECNTY; ++j)
		{
			iindex = i*SQ_TILECNTX + j;
			matWorld._41 = m_sqTile[iindex]->vPos.x;
			matWorld._42 = m_sqTile[iindex]->vPos.y;

			iter = m_sqTile[iindex]->terrainList.begin();
			BYTE	group_id = 0;
			BYTE	squence_id = 0;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			for( ; iter != m_sqTile[iindex]->terrainList.end(); ++iter)
			{
				group_id = (*iter)->byGroup_ID;
				squence_id = (*iter)->byGroup_sequence;
				if( (*iter)->byGroup_ID == TERRAIN_DIRT )
				{
					pTexture = (*m_DirtTex[group_id])[squence_id];
				}
				CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture
					, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
					, D3DCOLOR_ARGB(255,255,255,255));
			}			
		}
	}
}
void CTileMgr::TileOption_Update(void)
{
	int scrollX = m_pToolView->GetScrollPos(0);
	int scrollY = m_pToolView->GetScrollPos(1);

	int iindex = 0;
	for(int i = 0; i < 30; ++i)
	{
		for(int j = 0; j < 40; ++j)
		{
			int rowidx = i + scrollY/SQ_TILESIZEY;
			int colidx = j + scrollX/SQ_TILESIZEX;
			iindex = rowidx*SQ_TILECNTX + colidx;
			if(iindex < 0 || iindex >= SQ_TILECNTX*SQ_TILECNTY)
				continue;

			TERRAIN_INFO* ptemp = GetTerrain_Info(iindex);

			
			m_sqTile[iindex]->byOption = MOVE_NONE;
			if(ptemp->byGroup_ID == GROUP_L)
			{
				m_sqTile[iindex]->byFloor = 1;
			}
			else if(ptemp->byGroup_ID == GROUP_LU)
			{
				if(iindex != 3)
					m_sqTile[iindex]->byFloor = 1;
				else
					m_sqTile[iindex]->byFloor = 2;
			}
			else if(ptemp->byGroup_ID == GROUP_RU)
			{
				if(iindex != 2)
					m_sqTile[iindex]->byFloor = 1;
				else
					m_sqTile[iindex]->byFloor = 2;
			}
			else if(ptemp->byGroup_ID == GROUP_R)
			{					
				m_sqTile[iindex]->byFloor = 1;
			}
			else if(ptemp->byGroup_ID == GROUP_RD)
			{
				if(iindex != 0)
					m_sqTile[iindex]->byFloor = 1;
				else
					m_sqTile[iindex]->byFloor = 2;
			}
			else if(ptemp->byGroup_ID == GROUP_LD)
			{
				if(iindex != 1)
					m_sqTile[iindex]->byFloor = 1;
				else
					m_sqTile[iindex]->byFloor = 2;
			}
			else if(ptemp->byGroup_ID == GROUP_FLAT)
			{
				m_sqTile[iindex]->byOption = MOVE_OK;
				if(ptemp->byGroup_sequence != 3)
					m_sqTile[iindex]->byFloor = 2;
				else
					m_sqTile[iindex]->byFloor = 1;
			}
		}
	}
}
void CTileMgr::TileRender(void)
{
	int iindex = 0;
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity(&matWorld);
	const TEXINFO*	pTexture = NULL;

	int scrollX = m_pToolView->GetScrollPos(0);
	int scrollY = m_pToolView->GetScrollPos(1);
	for(int i = 0; i < 30; ++i)
	{
		for(int j = 0; j < 40; ++j)
		{
			int rowidx = i + scrollY/SQ_TILESIZEY;
			int colidx = j + scrollX/SQ_TILESIZEX;
			iindex = rowidx*SQ_TILECNTX + colidx;

			if(iindex < 0 || iindex >= SQ_TILECNTX*SQ_TILECNTY)
				continue;

			list<TERRAIN_INFO*>::iterator iter = m_sqTile[iindex]->terrainList.begin();
			list<TERRAIN_INFO*>::iterator iter_end = m_sqTile[iindex]->terrainList.end();
			int group_id = 0;
			int squence = 0;
			const vector<TEXINFO*>* temp;

			matWorld._41 = m_sqTile[iindex]->vPos.x - scrollX;
			matWorld._42 = m_sqTile[iindex]->vPos.y - scrollY;
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			for( ; iter != iter_end; ++iter)
			{
				if( (*iter)->byTerrain_ID == TERRAIN_DIRT )
				{
					group_id = (*iter)->byGroup_ID;
					temp = m_DirtTex[group_id];
					squence = (*iter)->byGroup_sequence;
					pTexture = (*temp)[squence];
				}
				else if( (*iter)->byTerrain_ID == TERRAIN_HIGHDIRT )
				{
					group_id = (*iter)->byGroup_ID;
					temp = m_HighDirtTex[group_id];
					squence = (*iter)->byGroup_sequence;
					pTexture = (*temp)[squence];
				}
				CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture
					, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
					, D3DCOLOR_ARGB(255,255,255,255));
			}
		}
	}

	D3DXMATRIX	matfont;
	D3DXMatrixIdentity(&matfont);

	RECT	rc = {0};
	TCHAR	szTemp[MIN_STR] = L"";
	//wsprintf(szTemp, L"스크롤X %d", m_pToolView->GetScrollPos(0));
	//matfont._41 = 300;
	//matfont._42 = 400;
	//CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	//CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
	//	, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
	//	, D3DCOLOR_ARGB(255,255,255,255));

	//matfont._41 = 300;
	//matfont._42 = 420;
	//wsprintf(szTemp, L"스크롤Y %d", m_pToolView->GetScrollPos(1));
	//CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	//CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
	//	, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
	//	, D3DCOLOR_ARGB(255,255,255,255));

	//matfont._41 = 300;
	//matfont._42 = 440;
	//wsprintf(szTemp, L"사각인덱스 %d", m_sqidx);
	//CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	//CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
	//	, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
	//	, D3DCOLOR_ARGB(255,255,255,255));

}
void CTileMgr::Rohmbus_Render(void)
{
	if(m_rbidx < 0 || m_rbRender == false)
		return;

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	D3DXVECTOR2	vPoint[5];

	float tilesizeX = (float)RB_TILESIZEX/2;
	float tilesizeY = (float)RB_TILESIZEY/2;
	int scollX = m_pToolView->GetScrollPos(0);
	int scollY = m_pToolView->GetScrollPos(1);

	D3DXVECTOR2	vCenter = m_rbTile[m_rbidx];
	vPoint[0].x = vCenter.x - tilesizeX - scollX;
	vPoint[0].y = vCenter.y - scollY;

	vPoint[1].x = vCenter.x - scollX;
	vPoint[1].y = vCenter.y - tilesizeY - scollY;

	vPoint[2].x = vCenter.x + tilesizeX - scollX;
	vPoint[2].y = vCenter.y - scollY;

	vPoint[3].x = vCenter.x - scollX;
	vPoint[3].y = vCenter.y + tilesizeY - scollY;

	vPoint[4] = vPoint[0];

	CDevice::GetInstance()->GetLine()->SetWidth(2.0f);
	CDevice::GetInstance()->GetLine()->Draw(vPoint , 5 , D3DCOLOR_ARGB(255,0,255,0));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CTileMgr::ShowGrid(void)
{
	D3DXVECTOR2	vPoint[2];

	int scrollX = m_pToolView->GetScrollPos(0);
	int scrollY = m_pToolView->GetScrollPos(1);

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	for(int j = 0; j < 40; ++j)
	{
		int index = j + scrollX/SQ_TILESIZEX;
		vPoint[0] = D3DXVECTOR2( float(index*SQ_TILESIZEX) - scrollX,0 - (float)scrollY);
		vPoint[1] = D3DXVECTOR2( float(index*SQ_TILESIZEX) - scrollX, float(SQ_TILECNTY*SQ_TILESIZEY) - scrollY);

		CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
		CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,0,0));
	}
	for(int j = 0; j < 32; ++j)
	{
		int index = j + scrollY/SQ_TILESIZEY;
		vPoint[0] = D3DXVECTOR2(0 - (float)scrollX ,float(index*SQ_TILESIZEX) - scrollY );
		vPoint[1] = D3DXVECTOR2(float (SQ_TILECNTX*SQ_TILESIZEX) - scrollX, float(index*SQ_TILESIZEX) - scrollY);

		CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
		CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,0,0));
	}

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

}
void CTileMgr::Release(void)
{

	for(size_t i = 0; i < m_sqTile.size(); ++i)
	{
		list<TERRAIN_INFO*>::iterator iter = m_sqTile[i]->terrainList.begin();
		list<TERRAIN_INFO*>::iterator iter_end = m_sqTile[i]->terrainList.end();
		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete((*iter));
			//iter = m_sqTile[i]->terrainList.erase(iter);
		}
		m_sqTile[i]->terrainList.clear();
		Safe_Delete(m_sqTile[i]);
	}
	m_sqTile.clear();

	vector<TILE*>().swap(m_sqTile);


	m_rbTile.clear();

	vector<D3DXVECTOR2>().swap(m_rbTile);
}

void CTileMgr::SetRohmbusRender(bool _bRender)
{
	m_rbRender = _bRender;
}
int	 CTileMgr::GetRbIdx(void)
{
	return m_rbidx;
}
void CTileMgr::Rohmbus_Picking(const CPoint&	_pt)
{

	float tilesizeX = (float)RB_TILESIZEX/2;
	float tilesizeY = (float)RB_TILESIZEY/2;

	float a = -(tilesizeY / tilesizeX);

	float b[4] = {0};

	int mouseptX = _pt.x + m_pToolView->GetScrollPos(0);
	int mouseptY = _pt.y + m_pToolView->GetScrollPos(1);

	m_sqidx = mouseptY/SQ_TILESIZEY*SQ_TILECNTX + mouseptX/SQ_TILESIZEX;
	
	for(int i = 0; i < RB_TILECNTY; ++i)
	{
		for(int j = 0; j < RB_TILECNTX; ++j)
		{
			int iindex = i*RB_TILECNTX + j;

			b[0] = m_rbTile[iindex].y - a * (m_rbTile[iindex].x - tilesizeX) ;
			b[1] = m_rbTile[iindex].y - a * (m_rbTile[iindex].x + tilesizeX) ;
			b[2] = m_rbTile[iindex].y + a * (m_rbTile[iindex].x - tilesizeX) ;
			b[3] = m_rbTile[iindex].y + a * (m_rbTile[iindex].x + tilesizeX) ;

			if( a*mouseptX + b[0] < mouseptY &&
				a*mouseptX + b[1] > mouseptY &&
				-a*mouseptX  + b[2] > mouseptY &&
				-a*mouseptX + b[3] < mouseptY )
			{
				m_rbidx = iindex;
				return;
			}
		}
	}
}
void CTileMgr::SetTerrain(const int idx , TERRAIN_INFO& pterrain_info , bool _bdelete/* = true*/)
{
	if(idx < 0 || idx >= SQ_TILECNTX * SQ_TILECNTY)
		return;

	TERRAIN_INFO* ptemp = NULL;


	if(pterrain_info.bysortLV == 1)
	{		
		if(m_sqTile[idx]->terrainList.size() == 2)
		{
			ptemp = m_sqTile[idx]->terrainList.back();
			ptemp->byGroup_ID = pterrain_info.byGroup_ID;
			ptemp->byGroup_sequence = pterrain_info.byGroup_sequence;
			ptemp->bysortLV = pterrain_info.bysortLV;
			ptemp->byTerrain_ID = pterrain_info.byTerrain_ID;
		}
		else
		{
			ptemp = new TERRAIN_INFO;
			ptemp->byGroup_ID = pterrain_info.byGroup_ID;
			ptemp->byGroup_sequence = pterrain_info.byGroup_sequence;
			ptemp->bysortLV = pterrain_info.bysortLV;
			ptemp->byTerrain_ID = pterrain_info.byTerrain_ID;

			m_sqTile[idx]->terrainList.push_back(ptemp);
		}
	}
	else
	{
		ptemp = m_sqTile[idx]->terrainList.front();
		ptemp->byGroup_ID = pterrain_info.byGroup_ID;
		ptemp->byGroup_sequence = pterrain_info.byGroup_sequence;
		ptemp->bysortLV = pterrain_info.bysortLV;
		ptemp->byTerrain_ID = pterrain_info.byTerrain_ID;

		if(m_sqTile[idx]->terrainList.size() == 2 && _bdelete == true)
		{
			ptemp = m_sqTile[idx]->terrainList.back();

			Safe_Delete(ptemp);
			m_sqTile[idx]->terrainList.pop_back();
		}
	}

	if(m_sqTile[idx]->terrainList.size() == 2)
		m_sqTile[idx]->terrainList.sort(rendersort_compare());

	CRewind::GetInstance()->SetTerrainData(m_sqTile[idx]->terrainList , idx);

}
int CTileMgr::FloorCheck(const int _index , const int _terrain_id)
{
	TERRAIN_INFO* ptemp = m_sqTile[_index]->terrainList.back();

	if(TERRAIN_HIGHDIRT == ptemp->byTerrain_ID)
	{
		if(TERRAIN_DIRT == _terrain_id)
		{
			if( (GROUP_LD == ptemp->byGroup_ID ||
				GROUP_RD == ptemp->byGroup_ID ||
				GROUP_L == ptemp->byGroup_ID ||
				GROUP_R == ptemp->byGroup_ID ) &&
				4<= ptemp->byGroup_sequence)
				return 0;

			return -1;
		}
		else if(TERRAIN_HIGHDIRT == _terrain_id)
			return 1;
	}
	else if(TERRAIN_DIRT == ptemp->byTerrain_ID)
	{
		if(TERRAIN_DIRT == _terrain_id)
			return 0;
		else if(TERRAIN_HIGHDIRT == _terrain_id)
			return 1;
	}

	return 0;
}
TERRAIN_INFO* CTileMgr::GetTerrain_Info(const int _index)
{
	if( 0 <= _index && _index < SQ_TILECNTX * SQ_TILECNTY)
	{
		return m_sqTile[_index]->terrainList.back();
	}
	else
		return NULL;
}
void CTileMgr::SetBeforeTile(list<TERRAIN_INFO>&	terrain_list , const int idx)
{
	list<TERRAIN_INFO*> ptemplist = m_sqTile[idx]->terrainList;

	TERRAIN_INFO* ptemp = NULL;
	TERRAIN_INFO terrain_info;

	for(list<TERRAIN_INFO*>::iterator iter = m_sqTile[idx]->terrainList.begin(); iter != m_sqTile[idx]->terrainList.end(); ++iter)
	{
		Safe_Delete((*iter));
	}
	m_sqTile[idx]->terrainList.clear();

	list<TERRAIN_INFO>::iterator iter = terrain_list.begin();
	list<TERRAIN_INFO>::iterator iter_end = terrain_list.end();

	for(; iter != iter_end; ++iter)
	{
		ptemp = new TERRAIN_INFO;
		ptemp->byGroup_ID		= (*iter).byGroup_ID;
		ptemp->byGroup_sequence = (*iter).byGroup_sequence;
		ptemp->bysortLV			= (*iter).bysortLV;
		ptemp->byTerrain_ID		= (*iter).byTerrain_ID;

		m_sqTile[idx]->terrainList.push_back(ptemp);
	}

	m_sqTile[idx]->terrainList.sort(rendersort_compare());
}