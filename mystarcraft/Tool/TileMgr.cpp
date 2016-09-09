#include "StdAfx.h"
#include "TileMgr.h"
#include "TextureMgr.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CTileMgr)
CTileMgr::CTileMgr(void)
{
}

CTileMgr::~CTileMgr(void)
{
}

void CTileMgr::InitTile(void)
{
	m_sqTile.reserve(SQ_TILECNTX*SQ_TILECNTY);
	m_rbTile.reserve(RB_TILECNTX*RB_TILECNTY);

	m_DirtTex.reserve(GROUP_END);

	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			TILE*	ptile = new TILE();
			ptile->matWorld._41 = (float)(SQ_TILESIZEX/2 + j*SQ_TILESIZEX);
			ptile->matWorld._42 = (float)(SQ_TILESIZEY/2 + i*SQ_TILESIZEY);
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


	m_DirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetStateTexture(L"Dirt" , L"FLAT");
}
void CTileMgr::TileRender(void)
{
	int iindex = 0;
	D3DXMATRIX	matWorld , matTrans;
	const TEXINFO*	pTexture = NULL;
	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			iindex = i*SQ_TILECNTX + j;

			if( m_sqTile[iindex]->byTerrain_ID == TERRAIN_DIRT )
			{
				int group_id = m_sqTile[iindex]->byGroup_ID;
				int squence = m_sqTile[iindex]->byGroup_sequence;

				const vector<TEXINFO*>* temp = m_DirtTex[group_id];
				pTexture = (*temp)[squence];
			}

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(255,255,255,255));

		}
	}
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
