#include "StdAfx.h"
#include "TileMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"

#include "ToolView.h"

IMPLEMENT_SINGLETON(CTileMgr)
CTileMgr::CTileMgr(void)
{
	m_rbidx = 0;
	m_pToolView = NULL;
	m_rbRender = false;
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


	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			int iindex = i*SQ_TILECNTX + j;

			TILE*	ptile = new TILE();
			ptile->vPos.x = (float)(SQ_TILESIZEX/2 + j*SQ_TILESIZEX);
			ptile->vPos.y = (float)(SQ_TILESIZEY/2 + i*SQ_TILESIZEY);

			int group_id = ptile->byGroup_ID;
			ptile->byGroup_sequence = rand()%m_DirtTex[group_id]->size();
			m_sqTile.push_back(ptile);
		}
	}

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
		for(int j = 0; j < 40; ++j)
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
			else if(m_sqTile[iindex]->byTerrain_ID == TERRAIN_HIGHDIRT)
			{
				int group_id = m_sqTile[iindex]->byGroup_ID;
				const vector<TEXINFO*>* temp = m_HighDirtTex[group_id];
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
	wsprintf(szTemp, L"스크롤X %d", m_pToolView->GetScrollPos(0));
	matfont._41 = 300;
	matfont._42 = 400;
	CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
		, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
		, D3DCOLOR_ARGB(255,255,255,255));

	matfont._41 = 300;
	matfont._42 = 420;
	wsprintf(szTemp, L"스크롤Y %d", m_pToolView->GetScrollPos(1));
	CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
		, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
		, D3DCOLOR_ARGB(255,255,255,255));

	matfont._41 = 300;
	matfont._42 = 440;
	wsprintf(szTemp, L"마름모인덱스 %d", m_rbidx);
	CDevice::GetInstance()->GetSprite()->SetTransform(&matfont);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite()
		, szTemp, lstrlen(szTemp), &rc, DT_NOCLIP
		, D3DCOLOR_ARGB(255,255,255,255));

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

	m_rbidx = -1;

	int mouseptX = _pt.x + m_pToolView->GetScrollPos(0);
	int mouseptY = _pt.y + m_pToolView->GetScrollPos(1);

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
void CTileMgr::SetTerrain(const int idx , const TILE&	temptile)
{
	m_sqTile[idx]->byFloor = temptile.byFloor;
	m_sqTile[idx]->byGroup_ID = temptile.byGroup_ID;
	m_sqTile[idx]->byGroup_sequence = temptile.byGroup_sequence;
	m_sqTile[idx]->byTerrain_ID = temptile.byTerrain_ID;
}
int CTileMgr::TileCheck(const int _index , const int _flr)
{
	return _flr - m_sqTile[_index]->byFloor;
}
