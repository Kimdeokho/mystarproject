#include "StdAfx.h"
#include "TileMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"

#include "ToolView.h"
#include "TileDebug.h"
#include "Rewind.h"
#include "MyMouse.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CTileMgr)
CTileMgr::CTileMgr(void)
{
	m_pToolView = NULL;
	m_textureMap = NULL;
	m_rbRender = false;
	m_rbidx = -1;
	m_sqidx = -1;
	m_oldidx = -1;
	m_minimappt.x = 0;
	m_minimappt.y = 0;
}

CTileMgr::~CTileMgr(void)
{
	Release();
}

void CTileMgr::InitTile(void)
{
	m_minimaRatio.x = 0.3125f;
	m_minimaRatio.y = 0.1757f;

	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	D3DXCreateTexture(pdevice , 1280,720 , D3DX_DEFAULT , D3DUSAGE_RENDERTARGET
		, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_textureMap);

	D3DXCreateTexture(pdevice , 1280,720 , D3DX_DEFAULT , D3DUSAGE_RENDERTARGET
		, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_newtextureMap);

	m_pToolView = ((CMainFrame*)AfxGetMainWnd())->m_pToolView;

	m_sqTile.reserve(SQ_TILECNTX*SQ_TILECNTY);
	m_rbTile.reserve(RB_TILECNTX*RB_TILECNTY);
	m_terrainInfo_List.reserve(SQ_TILECNTX*SQ_TILECNTY);

	m_DirtTex.reserve(GROUP_END);
	m_DirtTex.resize(GROUP_END);
	m_HighDirtTex.reserve(GROUP_END);
	m_HighDirtTex.resize(GROUP_END);
	m_WaterTex.reserve(GROUP_END);
	m_WaterTex.resize(GROUP_END);
	m_HillTex.reserve(GROUP_END);
	m_HillTex.resize(GROUP_END);

	m_DirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetStateTexture(L"Dirt" , L"FLAT");

	m_HighDirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"FLAT");
	m_HighDirtTex[GROUP_L] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"L");
	m_HighDirtTex[GROUP_LU] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"LU");
	m_HighDirtTex[GROUP_RU] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"RU");
	m_HighDirtTex[GROUP_R] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"R");
	m_HighDirtTex[GROUP_RD] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"RD");
	m_HighDirtTex[GROUP_LD] = CTextureMgr::GetInstance()->GetStateTexture(L"High_dirt" , L"LD");

	m_WaterTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetStateTexture(L"Water" , L"FLAT");
	m_WaterTex[GROUP_L] = CTextureMgr::GetInstance()->GetStateTexture(L"Water" , L"L");
	m_WaterTex[GROUP_LU] = CTextureMgr::GetInstance()->GetStateTexture(L"Water" , L"LU");
	m_WaterTex[GROUP_RU] = CTextureMgr::GetInstance()->GetStateTexture(L"Water" , L"RU");
	m_WaterTex[GROUP_R] = CTextureMgr::GetInstance()->GetStateTexture(L"Water" , L"R");
	m_WaterTex[GROUP_RD] = CTextureMgr::GetInstance()->GetStateTexture(L"Water" , L"RD");
	m_WaterTex[GROUP_LD] = CTextureMgr::GetInstance()->GetStateTexture(L"Water" , L"LD");

	m_HillTex[GROUP_L] = CTextureMgr::GetInstance()->GetStateTexture(L"HillL" , L"L");
	m_HillTex[GROUP_R] = CTextureMgr::GetInstance()->GetStateTexture(L"HillR" , L"R");

	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			int iindex = i*SQ_TILECNTX + j;

			TILE*	ptile = new TILE();
			ptile->vPos.x = (float)(SQ_TILESIZEX/2 + j*SQ_TILESIZEX);
			ptile->vPos.y = (float)(SQ_TILESIZEY/2 + i*SQ_TILESIZEY);

			TERRAIN_INFO* pterrain_info = new TERRAIN_INFO;
			pterrain_info->byTerrain_ID = TERRAIN_DIRT;
			int group_id = pterrain_info->byGroup_ID;
			pterrain_info->byGroup_sequence = rand()%m_DirtTex[group_id]->size();

			m_terrainInfo_List.push_back(list<TERRAIN_INFO*>());
			m_terrainInfo_List[iindex].push_back(pterrain_info);


			m_sqTile.push_back(ptile);
		}
	}
	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			int iindex = i*SQ_TILECNTX + j;
			CRewind::GetInstance()->InitStackTile(m_terrainInfo_List[iindex] , iindex);
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
void CTileMgr::Initminimap(void)
{
	int iindex = 0;

	CDevice::GetInstance()->Render_Begin();


	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			iindex = i*SQ_TILECNTX + j;
			MinimapDraw(iindex);
		}
	}
	CObjMgr::GetInstance()->MinimapRender(m_minimaRatio.x , m_minimaRatio.y);

	CDevice::GetInstance()->Render_End();
	CopySurface(m_newtextureMap);
}
void CTileMgr::SetTileOption(const int idx , TILE_OPTION eoption)
{
	m_sqTile[idx]->byOption = eoption;
}
void CTileMgr::SetMinimapupdate()
{
	int iindex = 0;

	list<int>* templist = CRewind::GetInstance()->Getidxlist();


	CDevice::GetInstance()->Render_Begin();

	if(NULL != templist)
	{
		list<int>::iterator iter = templist->begin();
		list<int>::iterator iter_end = templist->end();

		for(; iter != iter_end; ++iter)
		{
			iindex = *iter;
			if(m_oldidx == iindex)			
				continue;
			else
			{
				m_oldidx = iindex;
				if(iindex < 0 || iindex >= SQ_TILECNTX*SQ_TILECNTY)
					continue;

				MinimapDraw(iindex);
			}
		}	
		templist->clear();
	}
	CObjMgr::GetInstance()->MinimapRender(m_minimaRatio.x , m_minimaRatio.y);

	CDevice::GetInstance()->Render_End();

	CopySurface(m_newtextureMap);

	MinimapSquare();
}
void CTileMgr::MinimapRender(void)
{
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity(&matWorld);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	//CDevice::GetInstance()->GetSprite()->Draw(m_textureMap
	//	, NULL, &D3DXVECTOR3(0, 0, 0.f), NULL
	//	, D3DCOLOR_ARGB(255,255,255,255));

	CDevice::GetInstance()->GetSprite()->Draw(m_newtextureMap
		, NULL, &D3DXVECTOR3(0, 0, 0.f), NULL
		, D3DCOLOR_ARGB(255,255,255,255));

}
void CTileMgr::TileOption_Update(void)
{
	int scrollX = m_pToolView->GetScrollPos(0);
	int scrollY = m_pToolView->GetScrollPos(1);

	int iindex = 0;
	for(int i = 0; i < 30; ++i)
	{
		for(int j = 0; j < 45; ++j)
		{
			int rowidx = i + scrollY/SQ_TILESIZEY;
			int colidx = j + scrollX/SQ_TILESIZEX;
			iindex = rowidx*SQ_TILECNTX + colidx;
			if(iindex < 0 || iindex >= SQ_TILECNTX*SQ_TILECNTY)
				continue;

			TERRAIN_INFO* ptemp = GetTerrain_Info(iindex);

			if(RESOURCE_MINERAL == m_sqTile[iindex]->byOption)
				continue;

			m_sqTile[iindex]->byOption = MOVE_NONE;

			if(TERRAIN_HILL_R == ptemp->byTerrain_ID)
			{
				m_sqTile[iindex]->byOption = MOVE_OK;
				if(4 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 5)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(10 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 13)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(17 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 19)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(24 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 27)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(30 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 33)
					m_sqTile[iindex]->byOption = MOVE_NONE;
			}
			else if(TERRAIN_HILL_L == ptemp->byTerrain_ID)
			{
				m_sqTile[iindex]->byOption = MOVE_OK;
				if(0 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 1)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(6 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 7)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(12 == ptemp->byGroup_sequence)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(16 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 17)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(22 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 23)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(26 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 29)
					m_sqTile[iindex]->byOption = MOVE_NONE;
				else if(32 <= ptemp->byGroup_sequence &&
					ptemp->byGroup_sequence <= 35)
					m_sqTile[iindex]->byOption = MOVE_NONE;
			}

			if(ptemp->byGroup_ID == GROUP_L)
			{
				m_sqTile[iindex]->byFloor = 1;
			}
			else if(ptemp->byGroup_ID == GROUP_LU)
			{
				if(ptemp->byGroup_sequence != 3)
					m_sqTile[iindex]->byFloor = 1;
				else
					m_sqTile[iindex]->byFloor = 2;
			}
			else if(ptemp->byGroup_ID == GROUP_RU)
			{
				if(ptemp->byGroup_sequence != 2)
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
				if(ptemp->byGroup_sequence != 0)
					m_sqTile[iindex]->byFloor = 1;
				else
					m_sqTile[iindex]->byFloor = 2;
			}
			else if(ptemp->byGroup_ID == GROUP_LD)
			{
				if(ptemp->byGroup_sequence != 1)
					m_sqTile[iindex]->byFloor = 1;
				else
					m_sqTile[iindex]->byFloor = 2;
			}
			else if(ptemp->byGroup_ID == GROUP_FLAT && ptemp->byTerrain_ID != TERRAIN_WATER)
			{
				m_sqTile[iindex]->byOption = MOVE_OK;
				if(ptemp->byTerrain_ID != TERRAIN_DIRT)
					m_sqTile[iindex]->byFloor = 2;
				else
					m_sqTile[iindex]->byFloor = 1;
			}
		}
	}
}
void CTileMgr::CopySurface(LPDIRECT3DTEXTURE9 ptexturemap)
{
	LPDIRECT3DSURFACE9 pBackBuffer , pSurface;
	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();


	//pdevice->GetBackBuffer(0,0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pdevice->GetRenderTarget(0 , &pBackBuffer);

	ptexturemap->GetSurfaceLevel(0, &pSurface);

	RECT rc;
	rc.left = 0;
	rc.right= 1280;
	rc.top = 0;
	rc.bottom = 720;

	pdevice->StretchRect(pBackBuffer , NULL, pSurface ,NULL , D3DTEXF_NONE);

	pBackBuffer->Release();
}
void CTileMgr::TileRender(void)
{
	int iindex = 0;
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity(&matWorld);
	const TEXINFO*	pTexture = NULL;

	int scrollX = m_pToolView->GetScrollPos(0);
	int scrollY = m_pToolView->GetScrollPos(1);
	for(int i = 0; i < 24; ++i)
	{
		for(int j = 0; j < 41; ++j)
		{
			int rowidx = i + scrollY/SQ_TILESIZEY;
			int colidx = j + scrollX/SQ_TILESIZEX;
			iindex = rowidx*SQ_TILECNTX + colidx;

			if(iindex < 0 || iindex >= SQ_TILECNTX*SQ_TILECNTY)
				continue;

			list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[iindex].begin();
			list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[iindex].end();
			int group_id = 0;
			int squence = 0;
			const vector<TEXINFO*>* temp;

			matWorld._41 = m_sqTile[iindex]->vPos.x - scrollX;
			matWorld._42 = m_sqTile[iindex]->vPos.y - scrollY;
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			for( ; iter != iter_end; ++iter)
			{
				group_id = (*iter)->byGroup_ID;
				squence = (*iter)->byGroup_sequence;
				if( (*iter)->byTerrain_ID == TERRAIN_DIRT )
				{
					temp = m_DirtTex[group_id];
					pTexture = (*temp)[squence];
				}
				else if( (*iter)->byTerrain_ID == TERRAIN_HIGHDIRT )
				{
					temp = m_HighDirtTex[group_id];
					pTexture = (*temp)[squence];
				}
				else if( (*iter)->byTerrain_ID == TERRAIN_WATER )
				{
					temp = m_WaterTex[group_id];
					pTexture = (*temp)[squence];
				}
				else if( (*iter)->byTerrain_ID == TERRAIN_HILL_R )
				{
					temp = m_HillTex[group_id];
					pTexture = (*temp)[squence];
				}
				else if( (*iter)->byTerrain_ID == TERRAIN_HILL_L )
				{
					temp = m_HillTex[group_id];
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
		list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[i].begin();
		list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[i].end();
		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete((*iter));
			//iter = m_sqTile[i]->terrainList.erase(iter);
		}
		m_terrainInfo_List[i].clear();
		Safe_Delete(m_sqTile[i]);
	}
	m_sqTile.clear();
	m_terrainInfo_List.clear();

	vector<TILE*>().swap(m_sqTile);
	vector<list<TERRAIN_INFO*>>().swap(m_terrainInfo_List);



	//m_rbTile.clear();
	//vector<D3DXVECTOR2>().swap(m_rbTile);
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
		if(m_terrainInfo_List[idx].size() == 2)
		{
			ptemp = m_terrainInfo_List[idx].back();
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

			m_terrainInfo_List[idx].push_back(ptemp);
		}
	}
	else
	{
		ptemp = m_terrainInfo_List[idx].front();
		ptemp->byGroup_ID = pterrain_info.byGroup_ID;
		ptemp->byGroup_sequence = pterrain_info.byGroup_sequence;
		ptemp->bysortLV = pterrain_info.bysortLV;
		ptemp->byTerrain_ID = pterrain_info.byTerrain_ID;

		if(m_terrainInfo_List[idx].size() == 2 && _bdelete == true)
		{
			ptemp = m_terrainInfo_List[idx].back();

			Safe_Delete(ptemp);
			m_terrainInfo_List[idx].pop_back();
		}
	}

	if(m_terrainInfo_List[idx].size() == 2)
		m_terrainInfo_List[idx].sort(rendersort_compare());

	CRewind::GetInstance()->SetTerrainData(m_terrainInfo_List[idx] , idx);

}
int CTileMgr::FloorCheck(const int _index , const int _terrain_id)
{
	TERRAIN_INFO* pori_terrain = m_terrainInfo_List[_index].back();

	if(TERRAIN_HIGHDIRT == pori_terrain->byTerrain_ID)
	{
		if(TERRAIN_DIRT == _terrain_id)
		{
			if( (GROUP_LD == pori_terrain->byGroup_ID ||
				GROUP_RD == pori_terrain->byGroup_ID ||
				GROUP_L == pori_terrain->byGroup_ID ||
				GROUP_R == pori_terrain->byGroup_ID ) &&
				4<= pori_terrain->byGroup_sequence)
				return 0;

			return -1;
		}
		else if(TERRAIN_HIGHDIRT == _terrain_id)
			return 1;
	}
	else if(TERRAIN_DIRT == pori_terrain->byTerrain_ID)
	{
		if(TERRAIN_WATER == _terrain_id)
		{
			if( (GROUP_LD == pori_terrain->byGroup_ID ||
				GROUP_RD == pori_terrain->byGroup_ID ||
				GROUP_L == pori_terrain->byGroup_ID ||
				GROUP_R == pori_terrain->byGroup_ID ) &&
				4<= pori_terrain->byGroup_sequence)
				return 0;

			return -1;
		}

		else if(TERRAIN_DIRT == _terrain_id)
			return 0;
		else if(TERRAIN_HIGHDIRT == _terrain_id)
			return 1;
	}
	else if(TERRAIN_WATER == pori_terrain->byTerrain_ID)
	{
		if(TERRAIN_WATER == _terrain_id)
			return -1;
		else if(TERRAIN_DIRT == _terrain_id)
			return 1;
	}
	else if(TERRAIN_HILL_L == pori_terrain->byTerrain_ID ||
		TERRAIN_HILL_R == pori_terrain->byTerrain_ID)
	{
		if(TERRAIN_HIGHDIRT == _terrain_id)
			return 1;
		else if(TERRAIN_DIRT == _terrain_id)
			return -1;
	}

	return 0;
}
TERRAIN_INFO* CTileMgr::GetTerrain_Info(const int _index)
{
	if( 0 <= _index && _index < SQ_TILECNTX * SQ_TILECNTY)
	{
		return m_terrainInfo_List[_index].back();
	}
	else
		return NULL;
}
int	CTileMgr::GetTileOption(const int _index)
{
	if( 0 <= _index && _index < SQ_TILECNTX * SQ_TILECNTY)
	{
		return m_sqTile[_index]->byOption;
	}
	else
		return -1;
}
void CTileMgr::SetBeforeTile(list<TERRAIN_INFO>&	terrain_list , const int idx)
{
	list<TERRAIN_INFO*> ptemplist = m_terrainInfo_List[idx];

	TERRAIN_INFO* ptemp = NULL;
	TERRAIN_INFO terrain_info;

	for(list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[idx].begin(); iter != m_terrainInfo_List[idx].end(); ++iter)
	{
		Safe_Delete((*iter));
	}
	m_terrainInfo_List[idx].clear();

	list<TERRAIN_INFO>::iterator iter = terrain_list.begin();
	list<TERRAIN_INFO>::iterator iter_end = terrain_list.end();

	for(; iter != iter_end; ++iter)
	{
		ptemp = new TERRAIN_INFO;
		ptemp->byGroup_ID		= (*iter).byGroup_ID;
		ptemp->byGroup_sequence = (*iter).byGroup_sequence;
		ptemp->bysortLV			= (*iter).bysortLV;
		ptemp->byTerrain_ID		= (*iter).byTerrain_ID;

		m_terrainInfo_List[idx].push_back(ptemp);
	}

	m_terrainInfo_List[idx].sort(rendersort_compare());
}
int CTileMgr::GetsqIdx(void)
{
	int x = CMyMouse::GetInstance()->GetAddScroll().x;
	int y = CMyMouse::GetInstance()->GetAddScroll().y;

	return (y/SQ_TILESIZEY)*SQ_TILECNTX + x/SQ_TILESIZEX;
}
bool CTileMgr::InstallResourceCheck(const int idx)
{
	TILE_OPTION eoption = (TILE_OPTION)GetTileOption(idx);

	if(MOVE_OK == eoption)
		return true;
	else
		return false;
}
bool CTileMgr::InstallHillCheck(const int idx, const int isequence , const int idir)
{
	const TERRAIN_INFO* oriterrain = GetTerrain_Info(idx);

	if(0 == idir)
	{
		if(TERRAIN_HIGHDIRT == oriterrain->byTerrain_ID)
		{
			if(GROUP_FLAT == oriterrain->byGroup_ID)
			{
				if( 2 <= isequence && isequence <= 5 ||
					10 <= isequence && isequence <= 11)
					return true;
				else
					return false;
			}
			else if(GROUP_LD == oriterrain->byGroup_ID)
			{
				if( 0 <= isequence && isequence <=1 ||
					6 <= isequence && isequence <= 9 ||
					12 <= isequence && isequence <= 17 ||
					20 <= isequence && isequence <= 23 ||
					28 <= isequence && isequence <= 29)
					return true;
				else
					return false;
			}
		}
		else if(TERRAIN_DIRT == oriterrain->byTerrain_ID)
		{
			if(GROUP_FLAT == oriterrain->byGroup_ID)
			{
				if( 18 <= isequence && isequence <= 19 ||
					24 <= isequence && isequence <= 27 ||
					30 <= isequence && isequence <= 35)
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
	{
		if(TERRAIN_HIGHDIRT == oriterrain->byTerrain_ID)
		{
			if(GROUP_FLAT == oriterrain->byGroup_ID)
			{
				if( isequence <= 3 ||
					6 == isequence ||
					7 == isequence )
					return true;
				else
					return false;
			}
			else if(GROUP_RD == oriterrain->byGroup_ID)
			{
				if( 8 <= isequence && isequence <= 21 ||
					4 <= isequence && isequence <= 5 ||
					24 <= isequence && isequence <= 25)
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else if(TERRAIN_DIRT == oriterrain->byTerrain_ID)
		{
			if(GROUP_FLAT == oriterrain->byGroup_ID)
			{
				if(	26 <= isequence && isequence <= 35 ||
					22 <= isequence && isequence <= 23)
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}

	return false;
}

void CTileMgr::MinimapDraw(const int iindex)
{
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity(&matWorld);
	TEXINFO*	pTexture = NULL;


	list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[iindex].begin();
	list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[iindex].end();
	int group_id = 0;
	int squence = 0;
	const vector<TEXINFO*>* temp;

	matWorld._41 = m_sqTile[iindex]->vPos.x*m_minimaRatio.x;
	matWorld._42 = m_sqTile[iindex]->vPos.y*m_minimaRatio.y;
	matWorld._11 = m_minimaRatio.x;
	matWorld._22 = m_minimaRatio.y;
	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	for( ; iter != iter_end; ++iter)
	{
		group_id = (*iter)->byGroup_ID;
		squence = (*iter)->byGroup_sequence;
		if( (*iter)->byTerrain_ID == TERRAIN_DIRT )
		{
			temp = m_DirtTex[group_id];
			pTexture = (*temp)[squence];
		}
		else if( (*iter)->byTerrain_ID == TERRAIN_HIGHDIRT )
		{
			temp = m_HighDirtTex[group_id];
			pTexture = (*temp)[squence];
		}
		else if( (*iter)->byTerrain_ID == TERRAIN_WATER )
		{
			temp = m_WaterTex[group_id];
			pTexture = (*temp)[squence];
		}
		else if( (*iter)->byTerrain_ID == TERRAIN_HILL_R )
		{
			temp = m_HillTex[group_id];
			pTexture = (*temp)[squence];
		}
		else if( (*iter)->byTerrain_ID == TERRAIN_HILL_L )
		{
			temp = m_HillTex[group_id];
			pTexture = (*temp)[squence];
		}
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture
			, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
			, D3DCOLOR_ARGB(255,255,255,255));
	}
}
void CTileMgr::SaveTile(HANDLE h)
{
	//WriteFile(hFile , wr , sizeof(WCHAR)*10 , &dwbyte , NULL);

	DWORD dwbyte;

	int	mapsize = 0;
	int sortsize = 0;

	mapsize = SQ_TILECNTX*SQ_TILECNTY;
	WriteFile(h , &mapsize , sizeof(int) , &dwbyte , NULL);

	for(int i = 0; i < SQ_TILECNTX*SQ_TILECNTY; ++i)
		WriteFile(h,  m_sqTile[i], sizeof(TILE) , &dwbyte , NULL);

	for(int i = 0; i < SQ_TILECNTX*SQ_TILECNTY; ++i)
	{
		list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[i].begin();
		list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[i].end();

		sortsize = (int)(m_terrainInfo_List[i].size());
		WriteFile(h,  &sortsize, sizeof(int) , &dwbyte , NULL);

		for( ; iter != iter_end; ++iter)
			WriteFile(h, *iter , sizeof(TERRAIN_INFO) , &dwbyte , NULL);
	}
}
void CTileMgr::LoadTile(HANDLE h)
{
	for(size_t i = 0; i < m_sqTile.size(); ++i)
	{
		list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[i].begin();
		list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[i].end();
		for( ; iter != iter_end; ++iter)
			Safe_Delete((*iter));

		m_terrainInfo_List[i].clear();
	}
	//m_terrainInfo_List.clear();
	//vector<list<TERRAIN_INFO*>>().swap(m_terrainInfo_List);

	DWORD dwbyte;
	int	mapsize = 0;
	int sortsize = -1;

	//맵사이즈 -> 타일옵션 -> 터레인정보 순으로 로드한다.

	ReadFile(h , &mapsize, sizeof(int) , &dwbyte , NULL);
	//m_sqTile.reserve(mapsize);
	m_terrainInfo_List.reserve(mapsize);

	TILE tempTile;
	TERRAIN_INFO* tempterrain = NULL;

	for(int i = 0; i < mapsize; ++i)
	{
		//tempTile = new TILE;
		ReadFile(h , &tempTile, sizeof(TILE) , &dwbyte , NULL);
		//m_sqTile.push_back(tempTile);
		m_sqTile[i]->byFloor = tempTile.byFloor;
		m_sqTile[i]->byOption = tempTile.byOption;
	}

	for(int i = 0; i < mapsize; ++i)
	{		
		ReadFile(h , &sortsize , sizeof(int) , &dwbyte , NULL);

		//list<TERRAIN_INFO*> templist;
		for(int j = 0; j < sortsize; ++j)
		{
			tempterrain = new TERRAIN_INFO;
			ReadFile(h , tempterrain , sizeof(TERRAIN_INFO) , &dwbyte , NULL);
			m_terrainInfo_List[i].push_back(tempterrain);
			//templist.push_back(tempterrain);
		}
		//m_terrainInfo_List.push_back(templist);

		CRewind::GetInstance()->InitStackTile(m_terrainInfo_List[i] , i);
	}
	Initminimap();
}

void CTileMgr::SetMinimapPoint(const CPoint& pt)
{
	m_minimappt.x = pt.x*16;
	m_minimappt.y = pt.y*16;
}

void CTileMgr::MinimapSquare()
{
	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	float ScrollX = (float)(m_pToolView->GetScrollPos(0));
	float ScrollY = (float)(m_pToolView->GetScrollPos(1));

	D3DXVECTOR2	vPoint[5];
	vPoint[0].x = ScrollX;
	vPoint[1].x = ScrollX + TOOLSIZE_X;
	vPoint[2].x = ScrollX + TOOLSIZE_X;
	vPoint[3].x = ScrollX;

	vPoint[0].y = ScrollY;
	vPoint[1].y = ScrollY;
	vPoint[2].y = ScrollY + TOOLSIZE_Y;
	vPoint[3].y = ScrollY + TOOLSIZE_Y;

	for(int i = 0; i < 4; ++i)
	{
		vPoint[i].x *= m_minimaRatio.x;
		vPoint[i].y *= m_minimaRatio.y;
	}
	vPoint[4] = vPoint[0];

	CDevice::GetInstance()->GetLine()->SetWidth(5.0f);
	CDevice::GetInstance()->GetLine()->Draw(vPoint , 5 , D3DCOLOR_ARGB(255,255,255,255));

	CDevice::GetInstance()->Render_Begin();
	CDevice::GetInstance()->Render_End();
}
