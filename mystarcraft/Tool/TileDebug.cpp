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
		m_DownFloorPos[i] = -1;

	for(int i = 0; i < 6; ++i)
		for(int j = 0; j < 6; ++j)
			m_HillPos[i][j] = -1;

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 4; ++j)
			m_GasPos[i][j] = -1;

	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 4; ++j)
			m_StartbasePos[i][j] = -1;

	D3DXMatrixIdentity(&m_matWorld);

	for(int i = 0; i < DBG_END; ++i)
		m_debug_set[i] = false;

	m_eDir = HILL_L;
}

CTileDebug::~CTileDebug(void)
{
}
void CTileDebug::SetTile(const vector<TILE*>* ptile)
{
	m_psqTile = ptile;
}
bool CTileDebug::Whether_install_Mineral()
{
	InitMineralPos();
	for(int i = 0; i < 1; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(!CTileMgr::GetInstance()->InstallResourceCheck(m_MineralPos[i][j]))
				return false;
		}
	}
	return true;
}
bool CTileDebug::Whether_install_Gas()
{
	InitGasPos();
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(!CTileMgr::GetInstance()->InstallResourceCheck(m_GasPos[i][j]))
				return false;
		}
	}
	return true;
}
bool CTileDebug::Whether_install_StartBase()
{
	InitStartBasePos();
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(!CTileMgr::GetInstance()->InstallResourceCheck(m_StartbasePos[i][j]))
				return false;
		}
	}
	return true;
}
const D3DXVECTOR2& CTileDebug::Get_evenColPos(void)
{
	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	int scrollX = CMyMouse::GetInstance()->GetScrollPt().x;
	int scrollY = CMyMouse::GetInstance()->GetScrollPt().y;

	m_vInstallPos.x = (*m_psqTile)[sqidx]->vPos.x - scrollX - SQ_TILESIZEX/2;
	m_vInstallPos.y = (*m_psqTile)[sqidx]->vPos.y - scrollY - SQ_TILESIZEY/2;

	return m_vInstallPos;
}
const D3DXVECTOR2& CTileDebug::Get_oddColPos(void)
{
	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	int scrollX = CMyMouse::GetInstance()->GetScrollPt().x;
	int scrollY = CMyMouse::GetInstance()->GetScrollPt().y;

	m_vInstallPos.x = (*m_psqTile)[sqidx]->vPos.x - scrollX - SQ_TILESIZEX/2;
	m_vInstallPos.y = (*m_psqTile)[sqidx]->vPos.y - scrollY;

	return m_vInstallPos;
}
void CTileDebug::Set_TileOption(TILE_OPTION eOption)
{
	switch(eOption)
	{
	case RESOURCE_MINERAL:
		{
			CTileMgr::GetInstance()->SetTileOption(m_MineralPos[0][0] , eOption);
			CTileMgr::GetInstance()->SetTileOption(m_MineralPos[0][1] , eOption);
			break;
		}
	case RESOURCE_GAS:
		{
			for(int i = 0; i < 2; ++i)
				for(int j = 0; j < 4; ++j)
					CTileMgr::GetInstance()->SetTileOption(m_GasPos[i][j] , eOption);
			break;
		}
	case OP_STARTBASE:
		{
			for(int i = 0; i < 3; ++i)
				for(int j = 0; j < 4; ++j)
					CTileMgr::GetInstance()->SetTileOption(m_StartbasePos[i][j] , MOVE_NONE);

			break;
		}
	}
}
void CTileDebug::InitMineralPos()
{
	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	if(sqidx < 0 || sqidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	for(int i = 0; i < 1; ++i)
	{
		for(int j = -1; j < 1; ++j)
		{
			m_MineralPos[i][j+1] = sqidx + (i*SQ_TILECNTX+j);
		}
	}
}
void CTileDebug::InitGasPos()
{
	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	if(sqidx < 0 || sqidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	for(int i = -1; i < 1; ++i)
	{
		for(int j = -2; j < 2; ++j)
		{
			m_GasPos[i+1][j+2] = sqidx + (i*SQ_TILECNTX+j);
		}
	}
}
void CTileDebug::InitStartBasePos()
{
	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	if(sqidx < 0 || sqidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	for(int i = -1; i < 2; ++i)
		for(int j = -2; j < 2; ++j)
			m_StartbasePos[i+1][j+2] = sqidx + (i*SQ_TILECNTX+j);
}
void CTileDebug::Preview_StartBase(void)
{
	if(!m_debug_set[DBG_STARTBASE])
		return;

	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	if(sqidx < 0 || sqidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	D3DXMatrixIdentity(&m_matWorld);

	InitStartBasePos();

	int maskidx = 0;

	int scrollX = CMyMouse::GetInstance()->GetScrollPt().x;
	int scrollY = CMyMouse::GetInstance()->GetScrollPt().y;

	const TEXINFO*  pTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"ETC" , L"Startbase");
	const TEXINFO*	whiteTile = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	m_matWorld._41 = (*m_psqTile)[sqidx]->vPos.x - scrollX - SQ_TILESIZEX/2;
	m_matWorld._42 = (*m_psqTile)[sqidx]->vPos.y - scrollY;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture
		, NULL, &D3DXVECTOR3(64, 48, 0.f), NULL
		, D3DCOLOR_ARGB(255, 255 , 255 , 255));

	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 4; ++j)
		{
			if(m_StartbasePos[i][j] < 0 || m_StartbasePos[i][j] > SQ_TILECNTX*SQ_TILECNTY)
				continue;

			m_color.a = 65;
			m_color.b = 0;
			if(Whether_install_StartBase())
			{
				m_color.g = 255;
				m_color.r = 0;
			}
			else
			{
				m_color.r = 255;
				m_color.g = 0;
			}
			maskidx = m_StartbasePos[i][j];
			m_matWorld._41 = (*m_psqTile)[maskidx]->vPos.x - scrollX;
			m_matWorld._42 = (*m_psqTile)[maskidx]->vPos.y - scrollY;
			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(whiteTile->pTexture
				, NULL, &D3DXVECTOR3(16.f, 16.f, 0.f), NULL
				, D3DCOLOR_ARGB(m_color.a, m_color.r , m_color.g , m_color.b));
		}
}
void CTileDebug::Preview_Mineral(void)
{
	if(!m_debug_set[DBG_MINERAL])
		return;

	D3DXMatrixIdentity(&m_matWorld);

	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	if(sqidx < 0 || sqidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	InitMineralPos();

	int maskidx = 0;

	int scrollX = CMyMouse::GetInstance()->GetScrollPt().x;
	int scrollY = CMyMouse::GetInstance()->GetScrollPt().y;

	const vector<TEXINFO*>* vtemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Mineral");
	const TEXINFO*	whiteTile = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");


	m_matWorld._41 = (*m_psqTile)[sqidx]->vPos.x - scrollX - SQ_TILESIZEX/2;
	m_matWorld._42 = (*m_psqTile)[sqidx]->vPos.y - scrollY;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);

	CDevice::GetInstance()->GetSprite()->Draw((*vtemp)[0]->pTexture
		, NULL, &D3DXVECTOR3(32, 48, 0.f), NULL
		, D3DCOLOR_ARGB(255, 255 , 255 , 255));

	for(int i = 0; i < 1; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(m_MineralPos[i][j] < 0 || m_MineralPos[i][j] > SQ_TILECNTX*SQ_TILECNTY)
				continue;

			m_color.a = 65;
			m_color.b = 0;
			if(Whether_install_Mineral())
			{
				m_color.g = 255;
				m_color.r = 0;
			}
			else
			{
				m_color.r = 255;
				m_color.g = 0;
			}
			maskidx = m_MineralPos[i][j];
			m_matWorld._41 = (*m_psqTile)[maskidx]->vPos.x - scrollX;
			m_matWorld._42 = (*m_psqTile)[maskidx]->vPos.y - scrollY;
			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(whiteTile->pTexture
				, NULL, &D3DXVECTOR3(16.f, 16.f, 0.f), NULL
				, D3DCOLOR_ARGB(m_color.a, m_color.r , m_color.g , m_color.b));
		}
	}
}
void CTileDebug::Preview_Gas(void)
{
	if(!m_debug_set[DBG_GAS])
		return;

	D3DXMatrixIdentity(&m_matWorld);

	int sqidx = CTileMgr::GetInstance()->GetsqIdx();
	if(sqidx < 0 || sqidx > SQ_TILECNTX*SQ_TILECNTY)
		return;


	int maskidx = 0;
	for(int i = -1; i < 1; ++i)
	{
		for(int j = -2; j < 2; ++j)
		{
			m_GasPos[i+1][j+2] = sqidx + (i*SQ_TILECNTX+j);
		}
	}


	int scrollX = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(0);
	int scrollY = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(1);

	const vector<TEXINFO*>* vtemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Gas");
	const TEXINFO*	whiteTile = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	POINT pt = CMyMouse::GetInstance()->GetMousePt();

	m_matWorld._41 = (*m_psqTile)[sqidx]->vPos.x - scrollX - SQ_TILESIZEX/2;
	m_matWorld._42 = (*m_psqTile)[sqidx]->vPos.y - scrollY - SQ_TILESIZEY/2;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	
	CDevice::GetInstance()->GetSprite()->Draw((*vtemp)[0]->pTexture
		, NULL, &D3DXVECTOR3(64, 32, 0.f), NULL
		, D3DCOLOR_ARGB(255, 255 , 255 , 255));


	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(m_GasPos[i][j] < 0 || m_GasPos[i][j] > SQ_TILECNTX*SQ_TILECNTY)
				continue;

			m_color.a = 65;
			m_color.b = 0;
			if(true == CTileMgr::GetInstance()->InstallResourceCheck(m_GasPos[i][j]))
			{
				m_color.g = 255;
				m_color.r = 0;
			}
			else
			{
				m_color.r = 255;
				m_color.g = 0;
			}
			maskidx = m_GasPos[i][j];
			m_matWorld._41 = (*m_psqTile)[maskidx]->vPos.x - scrollX;
			m_matWorld._42 = (*m_psqTile)[maskidx]->vPos.y - scrollY;
			CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
				CDevice::GetInstance()->GetSprite()->Draw(whiteTile->pTexture
				, NULL, &D3DXVECTOR3(16.f, 16.f, 0.f), NULL
				, D3DCOLOR_ARGB(m_color.a, m_color.r , m_color.g , m_color.b));
		}
	}

}
void CTileDebug::Preview_Hill(void)
{
	if(!m_debug_set[DBG_HILL])
		return;

	const TEXINFO* ptemp = NULL;
	int idx = 0;

	int x = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(0);
	int y = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->GetScrollPos(1);

	const vector<TEXINFO*>* vectexL = CTextureMgr::GetInstance()->GetStateTexture(L"HillL" , L"L");
	const vector<TEXINFO*>* vectexR = CTextureMgr::GetInstance()->GetStateTexture(L"HillR" , L"R");

	const TEXINFO*	whiteTile = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	for(int i = 0; i < 6; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			if(HILL_L == m_eDir)
				ptemp = (*vectexL)[i*6+j];
			else
				ptemp = (*vectexR)[i*6+j];

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
	if(!m_debug_set[DBG_GROUP])
		return;

	const TEXINFO*	ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

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
	if(m_debug_set[DBG_MOVE] != true)
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
			if(MOVE_NONE == MoveOption || 
				RESOURCE_MINERAL == MoveOption ||
				RESOURCE_GAS == MoveOption)
			{
				const TEXINFO*	ptemp;
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

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
	Preview_Hill();
	Preview_Gas();
	Preview_Mineral();
	Preview_StartBase();
}
void CTileDebug::DebugTile_PosSet(void)
{
	int idx = CTerrainBrushMgr::GetInstance()->getrb_to_sqindex();

	if(m_debug_set[DBG_GROUP] == true)
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
	if(m_debug_set[DBG_HILL] == true)
	{
		idx = CTileMgr::GetInstance()->GetsqIdx();

		for(int i = -3; i < 3; ++i)
		{
			for(int j = -3; j < 3; ++j)
			{				
				m_HillPos[i+3][j+3] = idx + SQ_TILECNTY*i + j;
			}
		}
	}
}
void CTileDebug::SetDebug_Allinit()
{
	for(int i = 0; i < DBG_END; ++i)
		m_debug_set[i] = false;
}
void CTileDebug::SetDebugGroup(DEBUG_OPTION eop , bool _bdebug)
{
	m_debug_set[eop] = _bdebug;
}
void CTileDebug::SwitchDebug(DEBUG_OPTION eop)
{
	if(m_debug_set[eop] == true)
		m_debug_set[eop] = false;
	else
	{
		m_debug_set[eop] = true;
		DebugTile_PosSet();
	}
}
void CTileDebug::SetLR(HILL_DIR _edir)
{
	m_eDir = _edir;
}


