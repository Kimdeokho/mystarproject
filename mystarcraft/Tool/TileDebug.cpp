#include "StdAfx.h"
#include "TileDebug.h"
#include "TerrainBrushMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"

IMPLEMENT_SINGLETON(CTileDebug)
CTileDebug::CTileDebug(void)
{
	m_psqTile = NULL;

	for(int i = 0; i < GROUP_END; ++i)
	{
		m_DownFloorPos[i] = -1;
	}

	D3DXMatrixIdentity(&m_matWorld);

	m_bDebugGroup = false;
	m_bMoveOp = false;
}

CTileDebug::~CTileDebug(void)
{
}
void CTileDebug::SetTile(const vector<TILE*>* ptile)
{
	m_psqTile = ptile;
}
void CTileDebug::DebugGroup(void)
{
	if(m_bDebugGroup != true)
		return;

	const TEXINFO*	ptemp = CTextureMgr::GetInstance()->GetTexture(L"DebugTile" , L"White");

	int icase = CTerrainBrushMgr::GetInstance()->FloorCheck();

	int idx = 0;
	if( -2 == icase)
	{

	}
	else if( -1 == icase)
	{
		for(int i = 0; i < 10; ++i)
		{
			idx = m_DownFloorPos[i];

			if(idx < 0 || SQ_TILECNTX*SQ_TILECNTY <= idx)
				continue;

			int x = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(0);
			int y = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(1);

			m_matWorld._41 = (*m_psqTile)[idx]->vPos.x - x;
			m_matWorld._42 = (*m_psqTile)[idx]->vPos.y - y;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(ptemp->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(125,255,255,255));
		}
	}
	else if( 1 == icase)
	{

	}
	else if( 2 == icase)
	{

	}
}
void CTileDebug::MoveOption_Render(void)
{
	if(m_bMoveOp != true)
		return;

	int iindex = 0;

	for(int i = 0; i < 30; ++i)
	{
		for(int j = 0; j < 40; ++j)
		{
			int x = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(0);
			int y = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(1);

			int rowidx = i + y/SQ_TILESIZEY;
			int colidx = j + x/SQ_TILESIZEX;
			iindex = rowidx*SQ_TILECNTX + colidx;

			if(iindex < 0 || iindex >= SQ_TILECNTX*SQ_TILECNTY)
				continue;

			m_matWorld._41 = (*m_psqTile)[iindex]->vPos.x - x;
			m_matWorld._42 = (*m_psqTile)[iindex]->vPos.y - y;
			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

			int MoveOption = (*m_psqTile)[iindex]->byOption;
			if(MOVE_NONE == MoveOption)
			{
				const TEXINFO*	ptemp;
				ptemp = CTextureMgr::GetInstance()->GetTexture(L"DebugTile" , L"White");

				CDevice::GetInstance()->GetSprite()->Draw(ptemp->pTexture
					, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
					, D3DCOLOR_ARGB(60,255,0,0));
			}
		}
	}
}
void CTileDebug::DebugRender(void)
{
	DebugGroup();
	MoveOption_Render();
}
void CTileDebug::DebugTile_PosSet(void)
{
	if(m_bDebugGroup != true)
		return;

	int idx = CTerrainBrushMgr::GetInstance()->get_sqindex();

		//1단계 내리기
		m_DownFloorPos[0] = idx - SQ_TILECNTX*3 - 2;
		m_DownFloorPos[1] = idx - SQ_TILECNTX*2 - 4;

		m_DownFloorPos[2] = idx - SQ_TILECNTX*3;
		m_DownFloorPos[3] = idx - SQ_TILECNTX*2 + 2;

		m_DownFloorPos[4] = idx + SQ_TILECNTX - 2;
		m_DownFloorPos[5] = idx - 4;

		m_DownFloorPos[6] = idx + SQ_TILECNTX;
		m_DownFloorPos[7] = idx + 2;

		m_DownFloorPos[8] = idx - SQ_TILECNTX - 6;
		m_DownFloorPos[9] = idx - SQ_TILECNTX + 4;
}

void CTileDebug::SetDebugGroup()
{
	if(m_bDebugGroup == true)
		m_bDebugGroup = false;
	else
	{
		m_bDebugGroup = true;
		DebugTile_PosSet();
	}
}

void CTileDebug::SetMoveOption()
{
	if(m_bMoveOp == true)
		m_bMoveOp = false;
	else
		m_bMoveOp = true;
}
