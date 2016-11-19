#include "StdAfx.h"
#include "TileDebug.h"
#include "TerrainBrushMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "TileMgr.h"
#include "MyMouse.h"

IMPLEMENT_SINGLETON(CTileDebug)
CTileDebug::CTileDebug(void)
{
	m_psqTile = NULL;

	for(int i = 0; i < GROUP_END; ++i)
	{
		m_DownFloorPos[i] = -1;
	}

	for(int i = 0; i < 6; ++i)
	{
		for(int j = 0; j < 6; ++j)
			m_HillPos[i][j] = -1;
	}

	D3DXMatrixIdentity(&m_matWorld);

	m_bDebugGroup = false;
	m_bMoveOp = false;
	m_hillDebug = false;
	m_eDir = HILL_L;
}

CTileDebug::~CTileDebug(void)
{
}
void CTileDebug::SetTile(const vector<TILE*>* ptile)
{
	m_psqTile = ptile;
}
void CTileDebug::DebugHillRender(void)
{
	if(m_hillDebug != true)
		return;

	const TEXINFO* ptemp = NULL;
	int idx = 0;

	int x = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(0);
	int y = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(1);

	const TEXINFO*	whiteTile = CTextureMgr::GetInstance()->GetTexture(L"DebugTile" , L"White");

	for(int i = 0; i < 6; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			if(HILL_L == m_eDir)
				ptemp = CTextureMgr::GetInstance()->GetTexture(L"HillL" , L"L", i*6+j);
			else
				ptemp = CTextureMgr::GetInstance()->GetTexture(L"HillR" , L"R", i*6+j);

			idx = m_HillPos[i][j];

			if(idx < 0 || SQ_TILECNTX*SQ_TILECNTY <= idx)
				continue;

			m_matWorld._41 = (*m_psqTile)[idx]->vPos.x - x;
			m_matWorld._42 = (*m_psqTile)[idx]->vPos.y - y;
			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(ptemp->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(255, 255 , 255 , 255));

			if(CTileMgr::GetInstance()->InstallHillCheck(idx , i*6+j , m_eDir))
			{
				m_color.a = 65;
				m_color.r = 0;
				m_color.g = 255;
				m_color.b = 0;
			}
			else
			{
				m_color.a = 65;
				m_color.r = 255;
				m_color.g = 0;
				m_color.b = 0;
			}
			CDevice::GetInstance()->GetSprite()->Draw(whiteTile->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(m_color.a, m_color.r , m_color.g , m_color.b));
		}
	}
}
void CTileDebug::DebugGroup(void)
{
	if(m_bDebugGroup != true)
		return;

	const TEXINFO*	ptemp = CTextureMgr::GetInstance()->GetTexture(L"DebugTile" , L"White");

	int icase = CTerrainBrushMgr::GetInstance()->FloorCheck();

	int x = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(0);
	int y = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(1);

	int idx = 0;
	if( -2 == icase)
	{

	}
	else if( -1 == icase)
	{
		for(int i = 0; i < 14; ++i)
		{
			idx = m_DownFloorPos[i];

			if(idx < 0 || SQ_TILECNTX*SQ_TILECNTY <= idx)
				continue;

			m_matWorld._41 = (*m_psqTile)[idx]->vPos.x - x;
			m_matWorld._42 = (*m_psqTile)[idx]->vPos.y - y;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

			if( i < 10)
			{
				m_color.a = 60;
				m_color.r = 255;
				m_color.g = 255;
				m_color.b = 255;
			}
			else
			{
				m_color.a = 60;
				m_color.r = 0;
				m_color.g = 255;
				m_color.b = 0;
			}
			CDevice::GetInstance()->GetSprite()->Draw(ptemp->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(m_color.a,m_color.r,m_color.g,m_color.b));
		}
	}
	else if( 1 == icase)
	{
		for(int i = 0; i < 6; ++i)
		{
			idx = m_UpFloorPos[i];

			if(idx < 0 || SQ_TILECNTX*SQ_TILECNTY <= idx)
				continue;

			m_matWorld._41 = (*m_psqTile)[idx]->vPos.x - x;
			m_matWorld._42 = (*m_psqTile)[idx]->vPos.y - y;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

			m_color.a = 60;
			m_color.r = 255;
			m_color.g = 255;
			m_color.b = 255;

			CDevice::GetInstance()->GetSprite()->Draw(ptemp->pTexture
				, NULL, &D3DXVECTOR3(16, 16, 0.f), NULL
				, D3DCOLOR_ARGB(m_color.a,m_color.r,m_color.g,m_color.b));
		}
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
	DebugHillRender();
}
void CTileDebug::DebugTile_PosSet(void)
{
	int idx = CTerrainBrushMgr::GetInstance()->get_sqindex();

	if(m_bDebugGroup == true)
	{
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

		m_DownFloorPos[10] = idx - 2;
		m_DownFloorPos[11] = idx;
		m_DownFloorPos[12] = idx + SQ_TILECNTX - 3;
		m_DownFloorPos[13] = idx + SQ_TILECNTX*2 -1;

		m_UpFloorPos[0] = idx - 132;
		m_UpFloorPos[1] = idx - 258;
		m_UpFloorPos[2] = idx - 256;
		m_UpFloorPos[3] = idx - 126;
		m_UpFloorPos[4] = idx;
		m_UpFloorPos[5] = idx - 2;
	}
	if(m_hillDebug == true)
	{
		idx = CTileMgr::GetInstance()->GetIdx();

		for(int i = -3; i < 3; ++i)
		{
			for(int j = -3; j < 3; ++j)
			{				
				m_HillPos[i+3][j+3] = idx + SQ_TILECNTY*i + j;
			}
		}
	}
}
void CTileDebug::SetDebugGroup(bool _bdebug)
{
	m_bDebugGroup = _bdebug;
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
void CTileDebug::DebugSwitch(bool bdebug)
{
	m_hillDebug = bdebug;
}
void CTileDebug::SetLR(HILL_DIR _edir)
{
	m_eDir = _edir;
}
void CTileDebug::SetMoveOption()
{
	if(m_bMoveOp == true)
		m_bMoveOp = false;
	else
		m_bMoveOp = true;
}