#include "StdAfx.h"
#include "TileMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"

#include "ToolView.h"

IMPLEMENT_SINGLETON(CTileMgr)
CTileMgr::CTileMgr(void)
{
	m_pToolView = NULL;
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


	m_DirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetStateTexture(L"Dirt" , L"FLAT");
	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			int iindex = i*SQ_TILECNTY + j;

			TILE*	ptile = new TILE();
			ptile->vPos.x = (float)(SQ_TILESIZEX/2 + j*SQ_TILESIZEX);
			ptile->vPos.y = (float)(SQ_TILESIZEY/2 + i*SQ_TILESIZEY);

			int group_id = ptile->byGroup_ID;
			ptile->byGroup_sequence = rand()%m_DirtTex[group_id]->size();
			m_sqTile.push_back(ptile);
		}
	}

	for(int i = 0; i < RB_TILECNTX; ++i)
	{
		for(int j = 0; j < RB_TILECNTY; ++j)
		{
			D3DXVECTOR2	vPos;

			if(i%2 == 0)
				vPos = D3DXVECTOR2( float(j*RB_TILESIZEX) , float(i*RB_TILESIZEY) );
			else
				vPos = D3DXVECTOR2( float(RB_TILESIZEX/2) + float(j*RB_TILESIZEX) , float(i*RB_TILESIZEY) );
		}
	}
}
void CTileMgr::MinimapRender(void)
{
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity(&matWorld);
	TEXINFO*	pTexture = NULL;
	int iindex = 0;
	for(int i = 0; i < SQ_TILECNTX; ++i)
	{
		for(int j = 0; j < SQ_TILECNTY; ++j)
		{
			iindex = i*SQ_TILECNTX + j;
			matWorld._41 = m_sqTile[iindex]->vPos.x;
			matWorld._42 = m_sqTile[iindex]->vPos.y;

			if(m_sqTile[iindex]->byTerrain_ID == TERRAIN_DIRT)
			{
				pTexture = (*m_DirtTex[m_sqTile[iindex]->byGroup_ID])[m_sqTile[iindex]->byGroup_ID];
			}

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(255,255,255,255));
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
		for(int j = 0; j < 30; ++j)
		{
			int rowidx = i + scrollY/SQ_TILESIZEY;
			int colidx = j + scrollX/SQ_TILESIZEX;
			iindex = rowidx*SQ_TILECNTX + colidx;

			if(iindex < 0 || iindex >= SQ_TILECNTX*SQ_TILECNTY)
				continue;

			if( m_sqTile[iindex]->byTerrain_ID == TERRAIN_DIRT )
			{
				int group_id = m_sqTile[iindex]->byGroup_ID;				

				const vector<TEXINFO*>* temp = m_DirtTex[group_id];
				int squence = m_sqTile[iindex]->byGroup_sequence;

				pTexture = (*temp)[squence];
			}


			matWorld._41 = m_sqTile[iindex]->vPos.x - scrollX;
			matWorld._42 = m_sqTile[iindex]->vPos.y - scrollY;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(255,255,255,255));
		}
	}

	D3DXMATRIX	matfont;
	D3DXMatrixIdentity(&matfont);

	RECT	rc = {0};
	TCHAR	szTemp[MIN_STR] = L"";
	wsprintf(szTemp, L"胶农费X %d", m_pToolView->GetScrollPos(0));
	matfont._41 = 300;
	matfont._42 = 400;
	CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
		, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
		, D3DCOLOR_ARGB(255,255,255,255));

	matfont._41 = 300;
	matfont._42 = 420;
	wsprintf(szTemp, L"胶农费Y %d", m_pToolView->GetScrollPos(1));
	CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
		, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
		, D3DCOLOR_ARGB(255,255,255,255));
}
void CTileMgr::ShowGrid(void)
{
	D3DXVECTOR2	vPoint[2];

	
	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	for(int j = 0; j < 30; ++j)
	{
		vPoint[0] = D3DXVECTOR2( float(j*SQ_TILESIZEX),0);
		vPoint[1] = D3DXVECTOR2( float(j*SQ_TILESIZEX) , float(SQ_TILECNTY*SQ_TILESIZEY));

		CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
		CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,0,0));
	}
	for(int j = 0; j < 30; ++j)
	{
		vPoint[0] = D3DXVECTOR2(0,float(j*SQ_TILESIZEX) );
		vPoint[1] = D3DXVECTOR2(float (SQ_TILECNTX*SQ_TILESIZEX) , float(j*SQ_TILESIZEX) );

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
		Safe_Delete(m_sqTile[i]);
	}
	m_sqTile.clear();

	vector<TILE*>().swap(m_sqTile);


	m_rbTile.clear();

	vector<D3DXVECTOR2>().swap(m_rbTile);
}
