#include "StdAfx.h"
#include "TileManager.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "MouseMgr.h"
#include "MyMath.h"
#include "FontMgr.h"
#include "MyHeapSort.h"
#include "UnitMgr.h"

IMPLEMENT_SINGLETON(CTileManager)

CTileManager::CTileManager(void)
{
	for(int i = 0; i < 7; ++i)
		for(int j = 0; j < 6; ++j)
			m_MapTexture[i][j] = NULL;
;
}

CTileManager::~CTileManager(void)
{
	Release();
}
void CTileManager::Initialize(void)
{
	m_heapsort = new CMyHeapSort<FLOW_NODE*>();
	m_pSprite = CDevice::GetInstance()->GetSprite();
	D3DXMatrixIdentity(&m_matWorld);

	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	for(int i = 0; i < 7; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			D3DXCreateTexture(pdevice , BACKBUFFER_SIZEX, BACKBUFFER_SIZEY, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET
				,D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_MapTexture[i][j]);
		}
	}


	//m_fogTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Fog" , L"blackfog4096")->pTexture;
	//m_fogMaskTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Fog" , L"fogmask640")->pTexture;

	m_vTileCenter = D3DXVECTOR3(16.f,16.f,0.f);
	m_TileColor = D3DCOLOR_ARGB(255,255,255,255);	

	ReadyTileTexture();

	m_fTimeTest = 0.f;
}

void CTileManager::ReadyTileTexture(void)
{
	m_DirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Dirt" , L"FLAT");


	m_HighDirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"FLAT");
	m_HighDirtTex[GROUP_L] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"L");
	m_HighDirtTex[GROUP_LU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"LU");
	m_HighDirtTex[GROUP_RU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"RU");
	m_HighDirtTex[GROUP_R] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"R");
	m_HighDirtTex[GROUP_RD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"RD");
	m_HighDirtTex[GROUP_LD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"LD");

	m_WaterTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"FLAT");
	m_WaterTex[GROUP_L] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"L");
	m_WaterTex[GROUP_LU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"LU");
	m_WaterTex[GROUP_RU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"RU");
	m_WaterTex[GROUP_R] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"R");
	m_WaterTex[GROUP_RD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"RD");
	m_WaterTex[GROUP_LD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"LD");

	m_HillTex[GROUP_L] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"HillL" , L"L");
	m_HillTex[GROUP_R] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"HillR" , L"R");
}

void CTileManager::RenderTile(void)
{


	m_matWorld._41 = 0;
	m_matWorld._42 = 0;
	m_matWorld._11 = 1;
	m_matWorld._22 = 1;


	int iX = (int)CScrollMgr::m_fScrollX / BACKBUFFER_SIZEX;
	int iY = (int)CScrollMgr::m_fScrollY / BACKBUFFER_SIZEY;
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			m_matWorld._41 = BACKBUFFER_SIZEX*(j+iX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = BACKBUFFER_SIZEY*(i+iY) - CScrollMgr::m_fScrollY;
			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(m_MapTexture[i+iY][j+iX] , NULL , &D3DXVECTOR3(0,0,0), NULL
				, D3DCOLOR_ARGB(255,255,255,255));
		}
	}



	/*타일 옵션 디버그 확인용*/
	const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");
	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx;
	for(int i = 0; i < 20; ++i)
	{
		for(int j = 0; j < 26; ++j)
		{
			idx = (istartY+i) * SQ_TILECNTX + (istartX + j);
			if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			m_matWorld._41 = m_sqTile[idx]->vPos.x - CScrollMgr::m_fScrollX;//float((istartX + j)*SQ_TILESIZEX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = m_sqTile[idx]->vPos.y - CScrollMgr::m_fScrollY;//float((istartY + i)*SQ_TILESIZEY) - CScrollMgr::m_fScrollY;

			if(MOVE_OK == m_sqTile[idx]->byOption)
				continue;


			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(ptex->pTexture , NULL , &D3DXVECTOR3(16, 16,0), NULL
				,D3DCOLOR_ARGB(255,0,0,0));
		}
	}


	//CreepAlgorithm();
	//FogAlgorithm();

	RenderCreep();
	//Flowfield_Render();


	//TCHAR sz[32] = L"";
	//for(int i = 0; i < 20; ++i)
	//{
	//	for(int j = 0; j < 26; ++j)
	//	{
	//		idx = (istartY+i) * SQ_TILECNTX + (istartX + j);
	//		if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
	//			continue;

	//		m_matWorld._41 = float((istartX + j)*SQ_TILESIZEX) - CScrollMgr::m_fScrollX;
	//		m_matWorld._42 = float((istartY + i)*SQ_TILESIZEY) - CScrollMgr::m_fScrollY;

	//		fog_option = m_fogTile[idx]->fog_sequence;

	//		wsprintf(sz , L"%d" , m_sqTile[idx]->byFloor);
	//		CFontMgr::GetInstance()->FontRender(sz , m_matWorld._41+16 , m_matWorld._42+8 , D3DCOLOR_ARGB(255,0,255,0));
	//	}
	//}
}
void CTileManager::RenderFog(void)
{
	//여기가 안개 렌더
	TEXINFO**	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Fog");
	int ifogsquence = 1;

	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx = 0;
	unsigned short fog_option = 0;

	int fCenterX = 0;
	int fCenterY = 0;
	for(int i = 0; i < 20; ++i)
	{
		for(int j = 0; j < 26; ++j)
		{
			idx = (istartY+i) * SQ_TILECNTX + (istartX + j);
			if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			m_matWorld._41 = m_sqTile[idx]->vPos.x - CScrollMgr::m_fScrollX;//float((istartX + j)*SQ_TILESIZEX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = m_sqTile[idx]->vPos.y - CScrollMgr::m_fScrollY;//float((istartY + i)*SQ_TILESIZEY) - CScrollMgr::m_fScrollY;


			//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , m_fogTile[idx]->overlap_cnt , m_matWorld._41 , m_matWorld._42);
			ifogsquence = m_fogTile[idx]->fog_sequence;

			//if(0 == ifogsquence ||
				//2 == ifogsquence)
				//continue;

			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(ptemp[ifogsquence]->pTexture , NULL , &D3DXVECTOR3(48, 48, 0), NULL
				,m_fogTile[idx]->fog_color);

			//TCHAR temp[32];
			//RECT rc = {0};
			//wsprintf(temp , L"%d" , m_fogTile[idx]->overlap_cnt);
			//CDevice::GetInstance()->GetFont()->DrawTextW(m_pSprite , temp , lstrlen(temp) , &rc , DT_NOCLIP
			//	,D3DCOLOR_ARGB(255,0,255,0));
		}
	}


}

void CTileManager::RenderCreep(void)
{
	TEXINFO**	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Creep");
	int icreep_squence = 0;

	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx = 0;
	unsigned short fog_option = 0;

	int fCenterX = 0;
	int fCenterY = 0;

	CREEP_INFO*	pCreep_temp = NULL;

	for(int i = 0; i < 20; ++i)
	{
		for(int j = 0; j < 26; ++j)
		{
			idx = (istartY+i) * SQ_TILECNTX + (istartX + j);
			if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			if(FOG_BLACK == m_fogTile[idx]->eSight)
				continue;

			if(false == m_creepTile[idx]->bcreep_install)
				continue;

			m_matWorld._41 = m_sqTile[idx]->vPos.x - CScrollMgr::m_fScrollX;//float((istartX + j)*SQ_TILESIZEX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = m_sqTile[idx]->vPos.y - CScrollMgr::m_fScrollY;//float((istartY + i)*SQ_TILESIZEY) - CScrollMgr::m_fScrollY;




			pCreep_temp = m_creepTile[idx];

			if( 0x3332 == pCreep_temp->creep_bit ||
				0x3323 == pCreep_temp->creep_bit ||
				0x3233 == pCreep_temp->creep_bit ||
				0x2333 == pCreep_temp->creep_bit ||
				0x3220 == pCreep_temp->creep_bit ||
				0x2302 == pCreep_temp->creep_bit ||
				0x2032 == pCreep_temp->creep_bit ||
				0x0223 == pCreep_temp->creep_bit ||
				0x3311 == pCreep_temp->creep_bit ||
				0x1313 == pCreep_temp->creep_bit ||
				0x1133 == pCreep_temp->creep_bit ||
				0x3131 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = -1;
			}
			else if( 0x0000 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 0;
			}
			else if( 0x2200 ==  pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 1;
			}
			else if( 0x3210 ==  pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 2;
			}
			else if( 0x2301 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 3;
			}
			else if( 0x2100 == pCreep_temp->creep_bit )
			{
				pCreep_temp->creep_sequence = 4;
			}
			else if( 0x1200 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 6;
			}
			else if( 0x3110 == pCreep_temp->creep_bit ||
				0x2010 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 7;
			}
			else if( 0x1301 == pCreep_temp->creep_bit ||
				0x0201 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 8;
			}
			else if( 0x3120 == pCreep_temp->creep_bit || 
				0x2020 == pCreep_temp->creep_bit ||
				0x2031 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 9;
			}
			else if( 0x1302 == pCreep_temp->creep_bit || 
				0x0202 == pCreep_temp->creep_bit ||
				0x0213 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 10;
			}
			else if( 0x1032 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 11;
			}
			else if( 0x0123 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 12;
			}
			else if( 0x0021 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 13;
			}
			else if( 0x0012 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 15;
			}
			else if( 0x1020 == pCreep_temp->creep_bit ||
				0x1031 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 16;
			}
			else if( 0x0102 == pCreep_temp->creep_bit ||
				0x0113 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 17;
			}
			else if( 0x0022 == pCreep_temp->creep_bit)
			{
				pCreep_temp->creep_sequence = 18;
			}
			else
				pCreep_temp->creep_sequence = 0;

			icreep_squence = m_creepTile[idx]->creep_sequence;

			if(icreep_squence < 0)
				continue;

			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(ptemp[icreep_squence]->pTexture , NULL , &D3DXVECTOR3(16, 16,0), NULL
				,D3DCOLOR_ARGB(255,255,255,255));
		}
	}

}
void CTileManager::Creep_Autotile(const int& idx)
{
	int UP_idx = 0;
	int LU_idx = 0;
	int RU_idx = 0;
	int R_idx = 0;
	int L_idx = 0;
	int DL_idx = 0;
	int DR_idx = 0;
	int DOWN_idx = 0;


	UP_idx	 = idx - SQ_TILECNTX;
	DOWN_idx = idx + SQ_TILECNTX;
	LU_idx	 = idx - SQ_TILECNTX - 1;
	RU_idx	 = idx - SQ_TILECNTX + 1;
	L_idx	 = idx - 1;
	R_idx	 = idx + 1;
	DL_idx	 = idx + SQ_TILECNTX - 1;
	DR_idx	 = idx + SQ_TILECNTX + 1;

	if(false == m_creepTile[UP_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x1100;
	}
	if(false == m_creepTile[RU_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x0100;
	}
	if(false == m_creepTile[R_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x0101;
	}
	if(false == m_creepTile[DR_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x0001;
	}
	if(false == m_creepTile[DOWN_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x0011;
	}
	if(false == m_creepTile[DL_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x0010;
	}
	if(false == m_creepTile[L_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x1010;
	}
	if(false == m_creepTile[LU_idx]->bcreep_install)
	{
		m_creepTile[idx]->creep_bit += 0x1000;
	}


	if(true == m_creepTile[UP_idx]->bcreep_install)
	{
		m_creepTile[UP_idx]->creep_bit -= 0x0011;
	}
	if(true == m_creepTile[RU_idx]->bcreep_install)
	{
		m_creepTile[RU_idx]->creep_bit -= 0x0010;
	}
	if(true == m_creepTile[R_idx]->bcreep_install)
	{
		m_creepTile[R_idx]->creep_bit -= 0x1010;
	}
	if(true == m_creepTile[DR_idx]->bcreep_install)
	{
		m_creepTile[DR_idx]->creep_bit -= 0x1000;
	}
	if(true == m_creepTile[DOWN_idx]->bcreep_install)
	{
		m_creepTile[DOWN_idx]->creep_bit -= 0x1100;
	}
	if(true == m_creepTile[DL_idx]->bcreep_install)
	{
		m_creepTile[DL_idx]->creep_bit -= 0x0100;
	}
	if(true == m_creepTile[L_idx]->bcreep_install)
	{
		m_creepTile[L_idx]->creep_bit -= 0x0101;
	}
	if(true == m_creepTile[LU_idx]->bcreep_install)
	{
		m_creepTile[LU_idx]->creep_bit -= 0x0001;
	}
}
void CTileManager::Creep_decrease_autotile(const int& idx)
{
	int UP_idx = 0;
	int LU_idx = 0;
	int RU_idx = 0;
	int R_idx = 0;
	int L_idx = 0;
	int DL_idx = 0;
	int DR_idx = 0;
	int DOWN_idx = 0;


	UP_idx	 = idx - SQ_TILECNTX;
	DOWN_idx = idx + SQ_TILECNTX;
	LU_idx	 = idx - SQ_TILECNTX - 1;
	RU_idx	 = idx - SQ_TILECNTX + 1;
	L_idx	 = idx - 1;
	R_idx	 = idx + 1;
	DL_idx	 = idx + SQ_TILECNTX - 1;
	DR_idx	 = idx + SQ_TILECNTX + 1;

	m_creepTile[idx]->creep_bit = 0x0000;

	if(true == m_creepTile[UP_idx]->bcreep_install)
	{
		m_creepTile[UP_idx]->creep_bit += 0x0011;
	}
	if(true == m_creepTile[RU_idx]->bcreep_install)
	{
		m_creepTile[RU_idx]->creep_bit += 0x0010;
	}
	if(true == m_creepTile[R_idx]->bcreep_install)
	{
		m_creepTile[R_idx]->creep_bit += 0x1010;
	}
	if(true == m_creepTile[DR_idx]->bcreep_install)
	{
		m_creepTile[DR_idx]->creep_bit += 0x1000;
	}
	if(true == m_creepTile[DOWN_idx]->bcreep_install)
	{
		m_creepTile[DOWN_idx]->creep_bit += 0x1100;
	}
	if(true == m_creepTile[DL_idx]->bcreep_install)
	{
		m_creepTile[DL_idx]->creep_bit += 0x0100;
	}
	if(true == m_creepTile[L_idx]->bcreep_install)
	{
		m_creepTile[L_idx]->creep_bit += 0x0101;
	}
	if(true == m_creepTile[LU_idx]->bcreep_install)
	{
		m_creepTile[LU_idx]->creep_bit += 0x0001;
	}
}
void CTileManager::SightOffRender(const int& idx)
{
	m_fogTile[idx]->overlap_cnt -= 1;

	if(m_fogTile[idx]->overlap_cnt <= 0)
	{
		m_fogTile[idx]->overlap_cnt = 0;
		m_fogTile[idx]->bLight = false;
		m_fogTile[idx]->eSight = FOG_GREY;

		m_fogTile[idx]->fog_color = D3DCOLOR_ARGB(100,0,0,0);
		m_fogTile[idx]->fog_sequence = 1;
	}
}
void CTileManager::SightOnRender(const int& idx ,const int& irange , list<int>& sightoff_list , bool* fogsearch , MOVE_TYPE etype)
{
	//range는 가급적 홀수

	//int fradius = irange/2*32;
	int fradius = (irange/2);

	int half_range = fradius / SQ_TILESIZEX; //irange/2;

	int LUvtx_idx = idx - SQ_TILECNTX*half_range - half_range;
	int RUvtx_idx = idx - SQ_TILECNTX*half_range + half_range;
	int LDvtx_idx = idx + SQ_TILECNTX*half_range - half_range;
	int RDvtx_idx = idx + SQ_TILECNTX*half_range + half_range;


	int startIdx = idx;
	int destidx = 0;

	if(false == fogsearch[idx])
	{
		fogsearch[idx] = true;
		sightoff_list.push_back(startIdx);		
		CTileManager::GetInstance()->SetFogoverlap_cnt(idx);
		CTileManager::GetInstance()->SetFogSquence(startIdx , 0);
		CTileManager::GetInstance()->SetFogLight(startIdx , 0 , 1);
	}


	if( idx % SQ_TILECNTX - half_range < 0)
	{
		LUvtx_idx = LUvtx_idx - (idx % SQ_TILECNTX - half_range);
		LDvtx_idx = LDvtx_idx - (idx % SQ_TILECNTX - half_range);
	}

	if( idx % SQ_TILECNTX + half_range >= SQ_TILECNTX)
	{
		RUvtx_idx -= RUvtx_idx % SQ_TILECNTX + 1;
		RDvtx_idx -= RDvtx_idx % SQ_TILECNTX + 1;
	}

	if( LUvtx_idx < 0)
		LUvtx_idx += abs(LUvtx_idx - SQ_TILECNTX)/SQ_TILECNTX * SQ_TILECNTX;

	if( RUvtx_idx < 0 )
		RUvtx_idx += abs(RUvtx_idx - SQ_TILECNTX)/SQ_TILECNTX * SQ_TILECNTX;

	if( idx + half_range*SQ_TILECNTX >= SQ_TILECNTX*SQ_TILECNTY)
	{
		LDvtx_idx -= (LDvtx_idx/SQ_TILECNTX - SQ_TILECNTX + 1)*SQ_TILECNTX;
		RDvtx_idx -= (RDvtx_idx/SQ_TILECNTX - SQ_TILECNTX + 1)*SQ_TILECNTX;
	}

	for(int j = LUvtx_idx; j <= RUvtx_idx; ++j) //윗줄 
	{
		destidx = j;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , etype);
	}

	for(int j = LDvtx_idx; j <= RDvtx_idx; ++j) //아랫줄 
	{
		destidx = j;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , etype);
	}

	for(int i = LUvtx_idx + SQ_TILECNTX; i < LDvtx_idx; i += SQ_TILECNTX) //왼쪽 열
	{
		destidx = i;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , etype);
	}

	for(int i = RUvtx_idx + SQ_TILECNTX; i < RDvtx_idx; i += SQ_TILECNTX) //오른쪽 열
	{
		destidx = i;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , etype);
	}
}


void CTileManager::SetFogSquence(int idx , unsigned short sequence)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return;

	m_fogTile[idx]->fog_sequence		 = sequence;
}
void CTileManager::SetFogLight(int idx , float fdistance , float fradius)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return;

	if(false == m_fogTile[idx]->bLight)
		m_fogTile[idx]->bLight = true;

	m_fogTile[idx]->eSight = FOG_ALPHA;

	int ivalue = int(fdistance/fradius*100);

	if( m_fogTile[idx]->fog_color < D3DCOLOR_ARGB(ivalue, 0, 0 , 0) ) //기존에 있던 불빛이 더 밝다면
		return;

	m_fogTile[idx]->fog_color = D3DCOLOR_ARGB(ivalue , 0,0,0 );
}
void CTileManager::SetCreepInstall(const int& idx , bool binstall)
{
	m_creepTile[idx]->bcreep_install = binstall;
}
void CTileManager::SetFogColor(const int& idx , D3DCOLOR color)
{
	m_fogTile[idx]->fog_color = color;
}
bool CTileManager::CheckFogFloor(int myidx , int destidx)
{
	if(destidx < 0 || destidx >= SQ_TILECNTX*SQ_TILECNTY)
		return false;
	if(myidx < 0 || myidx >= SQ_TILECNTX*SQ_TILECNTY)
		return false;


	if( 1 == m_sqTile[myidx]->byFloor)
	{
		if(2 == m_sqTile[destidx]->byFloor)
			return false;
	}
	return true;
}
bool CTileManager::CheckCreepFloor(int myidx , int destidx)
{
	if(destidx < 0 || destidx >= SQ_TILECNTX*SQ_TILECNTY)
		return false;
	if(myidx < 0 || myidx >= SQ_TILECNTX*SQ_TILECNTY)
		return false;


	if( 1 == m_sqTile[myidx]->byFloor)
	{
		if(2 == m_sqTile[destidx]->byFloor)
			return false;
	}
	else if( 2 == m_sqTile[myidx]->byFloor )
	{
		if(1 == m_sqTile[destidx]->byFloor)
			return false;
	}
	return true;
}
BYTE CTileManager::GetTileOption(const int& idx)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return -1;

	return m_sqTile[idx]->byOption;
}
void CTileManager::CopySurface(LPDIRECT3DTEXTURE9 ptexturemap)
{
	LPDIRECT3DSURFACE9 pBackBuffer , pSurface;
	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	pdevice->GetRenderTarget(0 , &pBackBuffer);

	ptexturemap->GetSurfaceLevel(0, &pSurface);

	pdevice->StretchRect(pBackBuffer , NULL, pSurface ,NULL , D3DTEXF_NONE);

	pBackBuffer->Release();
}

void CTileManager::LoadTileData(HANDLE hFile)
{
	DWORD dwbyte;
	int mapsize = 0;
	int sortsize = -1;

	ReadFile(hFile , &mapsize, sizeof(int), &dwbyte, NULL);
	m_mapsize = mapsize;

	m_terrainInfo_List.reserve(mapsize);

	TILE* tempTile = NULL;
	TERRAIN_INFO* tempterrain = NULL;

	for(int i = 0; i < mapsize; ++i)
	{
		tempTile = new TILE();
		ReadFile(hFile, tempTile, sizeof(TILE), &dwbyte, NULL);

		m_sqTile[i] = tempTile;
	}

	for(int i = 0; i < mapsize; ++i)
	{
		ReadFile(hFile, &sortsize , sizeof(int), &dwbyte, NULL);

		m_terrainInfo_List.push_back(list<TERRAIN_INFO*>());
		for(int j = 0; j < sortsize; ++j)
		{
			tempterrain = new TERRAIN_INFO;
			ReadFile(hFile, tempterrain, sizeof(TERRAIN_INFO), &dwbyte, NULL);
			m_terrainInfo_List[i].push_back(tempterrain);
		}
	}

	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
		m_fogTile[i] = new FOG_INFO();

		m_creepTile[i] = new CREEP_INFO();

		m_flownode[i] = new FLOW_NODE();

		m_flownode[i]->index = i;
		if(MOVE_NONE == m_sqTile[i]->byOption)
			m_flownode[i]->bmove = false;
	}


	ReadyMainMap();
}
void CTileManager::ReadyMainMap(void)
{

	list<TERRAIN_INFO*>::iterator iter;
	list<TERRAIN_INFO*>::iterator iter_end;

	LPDIRECT3DTEXTURE9 pTexture;
	BYTE byTerrain_ID = 0;
	BYTE byGroup_ID = 0;
	BYTE byGroup_sequence = 0;

	int idx = 0;
	int istartidx = 0;
	int rowidx = 0;
	int colidx = 0;



	for(int Y = 0; Y < 7; ++Y)
	{
		for(int X = 0; X < 6; ++X)
		{
			istartidx = ((Y*BACKBUFFER_SIZEY)/SQ_TILESIZEY)*SQ_TILECNTX + (X*BACKBUFFER_SIZEX)/SQ_TILESIZEX;

			for(int i = 0; i < 20; ++i)
			{		

				for(int j = 0; j < 26; ++j)
				{
					CDevice::GetInstance()->Render_Begin();
					idx = istartidx + i*SQ_TILECNTX + j;

					if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
						continue;

					m_matWorld._41 = m_sqTile[idx]->vPos.x - BACKBUFFER_SIZEX*X;
					m_matWorld._42 = m_sqTile[idx]->vPos.y - BACKBUFFER_SIZEY*Y;


					m_pSprite->SetTransform(&m_matWorld);

					iter = m_terrainInfo_List[idx].begin();
					iter_end = m_terrainInfo_List[idx].end();
					for( ; iter != iter_end; ++iter)
					{
						byTerrain_ID = (*iter)->byTerrain_ID;
						byGroup_ID = (*iter)->byGroup_ID;
						byGroup_sequence = (*iter)->byGroup_sequence;

						if(TERRAIN_DIRT == byTerrain_ID)
							pTexture = (*m_DirtTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_HIGHDIRT == byTerrain_ID)
							pTexture = (*m_HighDirtTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_WATER == byTerrain_ID)
							pTexture = (*m_WaterTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_HILL_R == byTerrain_ID)
							pTexture = (*m_HillTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_HILL_L == byTerrain_ID)
							pTexture = (*m_HillTex[byGroup_ID])[byGroup_sequence]->pTexture;

						m_pSprite->Draw(pTexture , NULL , &m_vTileCenter , NULL ,
							m_TileColor);
					}
					CDevice::GetInstance()->Render_End();
					CopySurface(m_MapTexture[Y][X]);

				}
			}
		}
	}
}
void CTileManager::Release(void)
{
	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
		list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[i].begin();
		list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[i].end();
		for( ; iter != iter_end; ++iter)
			Safe_Delete((*iter));

		m_terrainInfo_List[i].clear();
		Safe_Delete(m_sqTile[i]);
	}
	m_terrainInfo_List.clear();

	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
		Safe_Delete(m_fogTile[i]);
		Safe_Delete(m_creepTile[i]);
		Safe_Delete(m_flownode[i]);
	}

	vector<list<TERRAIN_INFO*>>().swap(m_terrainInfo_List);

	//m_LightOff_List.clear();

	Safe_Delete(m_heapsort);
}

bool CTileManager::GetFogLight(const int& idx)
{
	return m_fogTile[idx]->bLight;
}

bool CTileManager::GetCreepInstall(const int& idx)
{
	return m_creepTile[idx]->bcreep_install;
}

TILE** CTileManager::GetSqTile(void)
{
	return m_sqTile;
}

CREEP_INFO** CTileManager::GetCreepTile(void)
{
	return m_creepTile;
}

void CTileManager::SetFogoverlap_cnt(const int& idx)
{
	//유닛 시야끼리 겹친부분의 타일을 +1한다
	m_fogTile[idx]->overlap_cnt += 1;

}

void CTileManager::SetFogSearch(const int& idx, bool bsearch)
{
	//지워도 될듯
	m_fogTile[idx]->bsearch = bsearch;
}
bool CTileManager::GetFogSearch(const int& idx)
{
	//지워도 될듯
	return m_fogTile[idx]->bsearch;
}
void CTileManager::Bresenham_fog(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int fRadius ,list<int>& light_IdxList , bool* fogsearch , MOVE_TYPE etype)
{
	int iWidth = int(vDest.x - vStart.x);
	int iHeight = int(vDest.y - vStart.y);
	float ishade_range = 0; //ishade_range 범위 0 ~ 1, 1에 가까울수록 범위에 다다른것임

	int e = 0;
	int icnt = 0;
	int idx = 0;
	int iAdd = 32;

	int loopcnt = 0;
	int isignX = 1;
	int isignY = 1;

	int myidx = CMyMath::Pos_to_index(vStart.x , vStart.y);

	if(iHeight < 0)
	{
		iHeight = -iHeight;
		isignY *= -1;
	}

	if(iWidth < 0)
	{
		iWidth = -iWidth;
		isignX *= -1;
	}

	float X = 0;
	float Y = 0;
	//int   iendidx = 0;

	if(iWidth >= iHeight)
	{
		loopcnt = iWidth/SQ_TILESIZEX;

		e = iWidth;
		for(int i = 1; i < loopcnt + 1; ++i)
		{
			e += iHeight*2;

			if(e >= iWidth*2)
			{
				icnt += iAdd;
				e -= iWidth*2;
			}


			//시작지점에서 i지점까지 퍼진 X거리이다.
			X = (vStart.x + i * SQ_TILESIZEX * isignX - vStart.x)*(vStart.x + i * SQ_TILESIZEX * isignX - vStart.x);
			Y = (vStart.y + icnt * isignY - vStart.y)*(vStart.y + icnt * isignY - vStart.y);




			if(X + Y <= fRadius*fRadius ) //범위안에 있으면
			{
				idx = CMyMath::Pos_to_index(vStart.x + i * SQ_TILESIZEX * isignX, vStart.y + icnt * isignY);
				ishade_range = (X + Y) / (fRadius*fRadius);

				if(MOVE_AIR == etype)
				{
					if(false == fogsearch[idx])
					{						
						fogsearch[idx] = true;
						SetFogoverlap_cnt(idx);
						light_IdxList.push_back(idx);						
					}
					if(ishade_range >= 0.8f)
					{
						SetFogSquence(idx , 1);
						SetFogLight(idx , X + Y , float(fRadius*fRadius));
					}
					else
					{
						SetFogSquence(idx , 0);
						SetFogLight(idx , 0 , 1);
					}
				}
				else if(MOVE_GROUND == etype)
				{
					if(true == CTileManager::GetInstance()->CheckFogFloor(myidx , idx))//공중일 경우 언덕지형을 신경안쓴다.
					{					
						if(false == fogsearch[idx])
						{						
							fogsearch[idx] = true;
							SetFogoverlap_cnt(idx);
							light_IdxList.push_back(idx);						
						}
						if(ishade_range >= 0.8f)
						{
							SetFogSquence(idx , 1);
							SetFogLight(idx , X + Y , float(fRadius*fRadius));
						}
						else
						{
							SetFogSquence(idx , 0);
							SetFogLight(idx , 0 , 1);
						}
					}
					else
						break;
				}
			}
			else
				break;
		}
	}
	else
	{
		loopcnt = iHeight/SQ_TILESIZEY;

		e = iHeight;
		for(int i = 1; i < loopcnt + 1; ++i)
		{
			e += iWidth*2;

			if( e >= iHeight*2)
			{
				icnt += iAdd;
				e -= iHeight*2;
			}

			X = (vStart.x + icnt * isignX - vStart.x)*(vStart.x + icnt * isignX - vStart.x);
			Y = (vStart.y + i*SQ_TILESIZEY*isignY - vStart.y)*(vStart.y + i*SQ_TILESIZEY*isignY - vStart.y);


			if(X + Y <= fRadius*fRadius )
			{
				idx = CMyMath::Pos_to_index(vStart.x + icnt * isignX , vStart.y + i*SQ_TILESIZEY*isignY);
				ishade_range = (X + Y) / (fRadius*fRadius);

				if(MOVE_AIR == etype)
				{
					if(false == fogsearch[idx])
					{						
						fogsearch[idx] = true;
						SetFogoverlap_cnt(idx);
						light_IdxList.push_back(idx);						
					}
					if(ishade_range >= 0.8f)
					{
						SetFogSquence(idx , 0);
						SetFogLight(idx , X + Y , float(fRadius*fRadius));
					}
					else
					{
						SetFogSquence(idx , 0);
						SetFogLight(idx , 0 , 1);
					}
				}
				else if(MOVE_GROUND == etype)
				{
					if(true == CheckFogFloor(myidx , idx))//공중일 경우 신경안쓴다.
					{					
						if(false == fogsearch[idx])
						{						
							fogsearch[idx] = true;
							SetFogoverlap_cnt(idx);
							light_IdxList.push_back(idx);						
						}
						//if(fRadius*fRadius - ishade_range <= X + Y)
						if(ishade_range >= 0.8f)
						{
							//ishade_range 범위 0 ~ 1

							//끝에 어두운 효과를 주는 곳
							SetFogSquence(idx , 1);
							SetFogLight(idx , X + Y , float(fRadius*fRadius));
						}
						else
						{
							/*일반적으로 밝히는곳*/
							SetFogSquence(idx , 0);
							SetFogLight(idx , 0 , 1);
						}
					}
					else
						break;
				}
			}
			else
				break;
		}
	}
}
void CTileManager::Bresenham_Creep(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int& fRadius ,const int& loopcnt,list<int>& creep_IdxList)
{
	int iWidth = int(vDest.x - vStart.x);
	int iHeight = int(vDest.y - vStart.y);

	int e = 0;
	int icnt = 0;
	int idx = 0;
	int iAdd = 32;

	int isignX = 1;
	int isignY = 1;

	int myidx = CMyMath::Pos_to_index(vStart.x , vStart.y);

	if(iHeight < 0)
	{
		iHeight = -iHeight;
		isignY = -1;
	}

	if(iWidth < 0)
	{
		iWidth = -iWidth;
		isignX = -1;
	}

	float X = 0;
	float Y = 0;
	int   iendidx = 0;

	if(iWidth >= iHeight)
	{
		e = iWidth;
		for(int i = 0; i < loopcnt; ++i)
		{
			e += iHeight*2;

			if(e >= iWidth*2)
			{
				icnt += iAdd;
				e -= iWidth*2;
			}
		}


		X = (vStart.x + loopcnt * SQ_TILESIZEX * isignX - vStart.x)*(vStart.x + loopcnt * SQ_TILESIZEX * isignX - vStart.x);
		Y = (vStart.y + icnt * isignY - vStart.y)*(vStart.y + icnt * isignY - vStart.y);




		if(X + Y <= fRadius*fRadius + 32*32)
		{
			idx = CMyMath::Pos_to_index(vStart.x + loopcnt * SQ_TILESIZEX * isignX, vStart.y + icnt * isignY);

			if(true == CTileManager::GetInstance()->CheckCreepFloor(myidx , idx) &&
				MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx) &&
				false == CTileManager::GetInstance()->GetCreepInstall(idx))
			{
				creep_IdxList.push_back(idx);
			}
		}		
	}
	else
	{
		e = iHeight;
		for(int i = 0; i < loopcnt; ++i)
		{
			e += iWidth*2;

			if( e >= iHeight*2)
			{
				icnt += iAdd;
				e -= iHeight*2;
			}
		}
		X = (vStart.x + icnt * isignX - vStart.x)*(vStart.x + icnt * isignX - vStart.x);
		Y = (vStart.y + loopcnt*SQ_TILESIZEY*isignY - vStart.y)*(vStart.y + loopcnt*SQ_TILESIZEY*isignY - vStart.y);


		if(X + Y <= fRadius*fRadius + 32*32)
		{
			idx = CMyMath::Pos_to_index(vStart.x + icnt * isignX , vStart.y + loopcnt*SQ_TILESIZEY*isignY);

			if(true == CTileManager::GetInstance()->CheckCreepFloor(myidx , idx) &&
				MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx) &&
				false == CTileManager::GetInstance()->GetCreepInstall(idx))
			{
				creep_IdxList.push_back(idx);
			}
		};		
	}
}
void CTileManager::GetFlowfield_Path(const int& idx , vector<int>& path)
{
	//임시방편으로 만든건데 지워야할듯
	int curidx = idx;
	while(true)
	{
		if(curidx == m_flownode[curidx]->idestidx ||
			MOVE_NONE == m_flownode[curidx]->idestidx)
			break;

		path.push_back(m_flownode[curidx]->index);
		curidx = m_flownode[curidx]->idestidx;

	}	
}
void CTileManager::Flowfield_Pathfinding(void)
{
	D3DXVECTOR2 temp = CMouseMgr::GetvMousePt();
	int goalidx = CMyMath::Pos_to_index(temp.x , temp.y);

	m_flownode[goalidx]->idestidx = goalidx;

	for(int i = 0; i < 16384; ++i)
	{
		m_flownode[i]->bcheck = false;
		//m_flowdestidx[i] = -1;
	}

	FLOW_NODE* pnode = m_flownode[goalidx];
	FLOW_NODE* pushnode = NULL;

	m_heapsort->push_node(pnode);
	pnode->bcheck = true;

	if(10000 != pnode->iCost)
		pnode->iCost = 0;

	int curcost = 0;
	while(true)
	{
		pnode = m_heapsort->pop_node();


		if(NULL == pnode)
			break;

		Init_eightidx(pnode->index);

		if(false == pnode->bmove)
			curcost = pnode->iCost % 10000;
		else
			curcost = pnode->iCost;

		for(int i = 0; i < ASTAR_DIR_END; ++i)
		{
			if( i == CENTER)
				continue;

			if(m_eight_idx[i] < 0)
				continue;

			pushnode = m_flownode[ m_eight_idx[i] ];


			if(false == pushnode->bcheck)
				pushnode->bcheck = true;
			else
				continue;

	
			pushnode->idestidx = pnode->index;
			m_flowdestidx[ m_eight_idx[i] ] = pnode->index;

			if( i <= 4)
				pushnode->iCost = curcost + 1;
			else
				pushnode->iCost = curcost + 2;

			if(false == pushnode->bmove)
				pushnode->iCost += 10000;

			m_heapsort->push_node(pushnode);			
		}
	}

	CUnitMgr::GetInstance()->path_relay(goalidx);
}
void CTileManager::Flowfield_Render(void)
{

	const TEXINFO*	ptemp = NULL;
	int ifogsquence = 1;

	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx = 0;

	for(int i = 0; i < 20; ++i)
	{
		for(int j = 0; j < 26; ++j)
		{
			idx = (istartY+i) * SQ_TILECNTX + (istartX + j);

			if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			m_matWorld._41 = m_sqTile[idx]->vPos.x - CScrollMgr::m_fScrollX;//float((istartX + j)*SQ_TILESIZEX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = m_sqTile[idx]->vPos.y - CScrollMgr::m_fScrollY;//float((istartY + i)*SQ_TILESIZEY) - CScrollMgr::m_fScrollY;


			//int dirsequence = m_flownode[idx]->idestidx - idx;
			int dirsequence = m_flowdestidx[idx] - idx;

			if( -SQ_TILECNTX == dirsequence)
			{
				/*업*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"UP");
			}
			else if( -(SQ_TILECNTX - 1) == dirsequence)
			{
				/*라이트 업*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"RIGHT_UP");
			}
			else if(1 == dirsequence)
			{
				/*라이트*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"RIGHT");
			}
			else if(SQ_TILECNTX + 1 == dirsequence)
			{
				/*라이트 다운*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"RIGHT_DOWN");
			}
			else if(SQ_TILECNTX == dirsequence)
			{
				/*다운*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"DOWN");
			}
			else if(SQ_TILECNTX - 1 == dirsequence)
			{
				/*레프트 다운*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"LEFT_DOWN");
			}
			else if( -1 == dirsequence)
			{
				/*레프트*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"LEFT");
			}
			else if( -(SQ_TILECNTX + 1) == dirsequence)
			{
				/*레프트 업*/
				ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"Direction" , L"LEFT_UP");
			}
			else
				continue;


			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(ptemp->pTexture , NULL , &D3DXVECTOR3(16, 16,0), NULL
				,D3DCOLOR_ARGB(255,255,255,255));
		}
	}
}
void CTileManager::Init_eightidx(const int& idx)
{
	m_eight_idx[CENTER] = idx;

	if(idx - SQ_TILECNTX < 0)
		m_eight_idx[UP] = -1;
	else
		m_eight_idx[UP] = idx - SQ_TILECNTX;

	if(idx % SQ_TILECNTX <= 0 ||
		idx - SQ_TILECNTX < 0)
		m_eight_idx[LEFT_UP] = -1;
	else
		m_eight_idx[LEFT_UP] = idx - SQ_TILECNTX - 1;

	if(idx % SQ_TILECNTX >= SQ_TILECNTX - 1 ||
		idx - SQ_TILECNTX < 0)
		m_eight_idx[RIGHT_UP] = -1;
	else
		m_eight_idx[RIGHT_UP] = idx - SQ_TILECNTX + 1;

	if(idx % SQ_TILECNTX >= SQ_TILECNTX - 1)
		m_eight_idx[RIGHT] = -1;
	else
		m_eight_idx[RIGHT] = idx  + 1;

	if(idx % SQ_TILECNTX <= 0)
		m_eight_idx[LEFT] = -1;
	else
		m_eight_idx[LEFT] = idx  - 1;

	if(idx + SQ_TILECNTX >= SQ_TILECNTX*SQ_TILECNTX)
		m_eight_idx[DOWN] = -1;
	else
		m_eight_idx[DOWN] = idx + SQ_TILECNTX;

	if(idx + SQ_TILECNTX >= SQ_TILECNTX*SQ_TILECNTX ||
		idx % SQ_TILECNTX >= SQ_TILECNTX - 1)
		m_eight_idx[RIGHT_DOWN] = -1;
	else
		m_eight_idx[RIGHT_DOWN] = idx + SQ_TILECNTX + 1;

	if(idx + SQ_TILECNTX >= SQ_TILECNTX*SQ_TILECNTX ||
		idx % SQ_TILECNTX <= 0)
		m_eight_idx[LEFT_DOWN] = -1;
	else
		m_eight_idx[LEFT_DOWN] = idx + SQ_TILECNTX - 1;
}

short* CTileManager::Get_flowfield_node(void)
{
	//return m_flownode;
	return m_flowdestidx;
}
