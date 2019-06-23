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
#include "Obj.h"
#include "Area_Mgr.h"
#include "Session_Mgr.h"
#include "Debug_Mgr.h"

IMPLEMENT_SINGLETON(CTileManager)

CTileManager::CTileManager(void)
{
	for(int i = 0; i < MAP_TEXTURECNTY; ++i)
		for(int j = 0; j < MAP_TEXTURECNTX; ++j)
			m_MapTexture[i][j] = NULL;
;
}

CTileManager::~CTileManager(void)
{
	Release();
}
void CTileManager::Initialize(void)
{
	m_minifog_onidx.reserve(SQ_TILECNTX*SQ_TILECNTX);
	m_minifog_offidx.reserve(SQ_TILECNTX*SQ_TILECNTX);

	m_heapsort = new CMyHeapSort<FLOW_NODE*>(128*128);
	m_pSprite = CDevice::GetInstance()->GetSprite();
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_fogmat);

	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	for(int i = 0; i < MAP_TEXTURECNTY; ++i)
	{
		for(int j = 0; j < MAP_TEXTURECNTX; ++j)
		{
			D3DXCreateTexture(pdevice , BACKBUFFER_SIZEX, BACKBUFFER_SIZEY, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET
				,D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_MapTexture[i][j]);
		}
	}


	D3DXCreateTexture(pdevice , 128, 128, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET
		,D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_MinimapTexture);

	m_MinifogTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"minifogmap")->pTexture;


	m_fogtexvec = CTextureMgr::GetInstance()->GetGeneralTexture(L"Fog");

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


	RenderCreep();
	//Flowfield_Render();

	/*타일 옵션 디버그 확인용*/


	if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_TILEPOP])
	{
		const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");
		int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
		int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
		int idx;
		for(int i = 0; i < CULLINGCNTY; ++i)
		{
			for(int j = 0; j < CULLINGCNTX; ++j)
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
					,D3DCOLOR_ARGB(255,255,0,0));
			}
		}
	}




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
	//TEXINFO**	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Fog");
	//const vector<TEXINFO*>*	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Fog");

	if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
		return;

	int ifogsquence = 1;

	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx = 0;
	unsigned short fog_option = 0;

	int fCenterX = 0;
	int fCenterY = 0;
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

	for(int i = 0; i < CULLINGCNTY; ++i)
	{
		for(int j = 0; j < CULLINGCNTX; ++j)
		{
			idx = (istartY+i) * SQ_TILECNTX + (istartX + j);
			if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			m_matWorld._41 = m_sqTile[idx]->vPos.x - CScrollMgr::m_fScrollX;
			m_matWorld._42 = m_sqTile[idx]->vPos.y - CScrollMgr::m_fScrollY;

			//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_fogTile[idx]->bLight[eteam] , m_sqTile[idx]->vPos.x  , m_sqTile[idx]->vPos.y);

			if(FOG_ALPHA == m_fogTile[idx]->eSight[eteam])
			{
				ifogsquence = 0;
			}
			else if(FOG_BLACK == m_fogTile[idx]->eSight[eteam] ||
				FOG_GREY == m_fogTile[idx]->eSight[eteam])
			{
				ifogsquence = 1;
			}
			//ifogsquence = m_fogTile[idx]->fog_sequence[eteam];

			//if(0 == ifogsquence ||
				//2 == ifogsquence)
				//continue;

			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw( (*m_fogtexvec)[ifogsquence]->pTexture , NULL , &D3DXVECTOR3(48, 48, 0), NULL
				,m_fogTile[idx]->fog_color[eteam]);

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
	const vector<TEXINFO*>*	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Creep");
	//TEXINFO**	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Creep");
	int icreep_squence = 0;

	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx = 0;
	unsigned short fog_option = 0;

	int fCenterX = 0;
	int fCenterY = 0;

	ULONG	pCreepbit = 0;

	for(int i = 0; i < CULLINGCNTY; ++i)
	{
		for(int j = 0; j < CULLINGCNTX; ++j)
		{
			idx = (istartY+i) * SQ_TILECNTX + (istartX + j);
			if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			//if(FOG_BLACK == m_fogTile[idx]->eSight) //이게 뜬금없이 왜있지??
			//	continue;

			//if(false == m_creepTile[idx]->bcreep_install)
				//continue;

			m_matWorld._41 = m_sqTile[idx]->vPos.x - CScrollMgr::m_fScrollX;//float((istartX + j)*SQ_TILESIZEX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = m_sqTile[idx]->vPos.y - CScrollMgr::m_fScrollY;//float((istartY + i)*SQ_TILESIZEY) - CScrollMgr::m_fScrollY;


			pCreepbit = m_creepTile[idx]->creep_bit;

			if(0 == pCreepbit)
				continue;

			//if(0xFFFFFFFF == pCreepbit)
			m_creepTile[idx]->creep_sequence = 12;

			if(0x33333333 == pCreepbit /*||
				0x31311313 == pCreepbit ||
				(0x01011010 ^ pCreepbit) == 0x01011010*/)
				m_creepTile[idx]->creep_sequence = 12;

			else if( (0x00011010 & pCreepbit) == 0x00011010)
				m_creepTile[idx]->creep_sequence = 8;

			else if( (0x01001010 & pCreepbit) == 0x01001010)
				m_creepTile[idx]->creep_sequence = 15;

			else if( (0x01010010 & pCreepbit) == 0x01010010)
				m_creepTile[idx]->creep_sequence = 16;

			else if( (0x01011000 & pCreepbit) == 0x01011000)
				m_creepTile[idx]->creep_sequence = 20;

			else if( 0x00000212 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 0;

			else if( 0x00000112 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 5;

			else if( 0x00000211 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 6;

			else if( 0x00201002 == pCreepbit ||
				0x00201001 == pCreepbit ||
				0x00101002 == pCreepbit ||
				 0x00101001 == pCreepbit )
				m_creepTile[idx]->creep_sequence = 13;

			else if( 0x20010200 == pCreepbit ||
				0x20010100 == pCreepbit ||
				0x10010200 == pCreepbit ||
				0x10010100 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 14;

			else if(  0x00000212 == pCreepbit ||
				0x00000112 == pCreepbit ||
				0x00000211 == pCreepbit ||
				0x00000111 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 25;

			else if(  0x21200000 == pCreepbit ||
				0x11200000 == pCreepbit ||
				0x21100000 == pCreepbit ||
				0x11100000 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 1;

			else if( 0x21301002 == pCreepbit ||
				0x21301001 == pCreepbit ||
				0x21201002 == pCreepbit ||
				0x21201001 == pCreepbit ||
				0x11301002 == pCreepbit ||
				0x11201002 == pCreepbit ||
				0x11301001 == pCreepbit ||
				0x11201001 == pCreepbit )
				m_creepTile[idx]->creep_sequence = 22;

				
			else if( 0x31210200 == pCreepbit ||
				0x31210100 == pCreepbit ||
				0x31110200 == pCreepbit ||
				0x31110100 == pCreepbit ||				
				0x21210200 == pCreepbit ||
				0x21210100 == pCreepbit ||
				0x21110200 == pCreepbit ||
				0x21110100 == pCreepbit )
				m_creepTile[idx]->creep_sequence = 23;


			else if( 0x00201213 == pCreepbit ||
				0x00201113 == pCreepbit ||
				0x00201212 == pCreepbit ||
				0x00201112 == pCreepbit ||
				0x00101213 == pCreepbit ||
				0x00101113 == pCreepbit ||
				0x00101212 == pCreepbit ||
				0x00101112 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 10;
		

			else if( 0x20010312 == pCreepbit ||
				0x20010311 == pCreepbit ||
				0x20010211 == pCreepbit ||
				0x20010212 == pCreepbit ||
				0x10010312 == pCreepbit ||
				0x10010311 == pCreepbit ||
				0x10010212 == pCreepbit ||
				0x10010211 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 11;


			else if( 0x10010311 == pCreepbit ||
				0x10010211 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 9;

			else if( 0x00101113 == pCreepbit ||
				0x00101112 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 7;

			else if( 0x21200000 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 24;

			else if( 0x11200000 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 17;

			else if( 0x21100000 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 18;

			else if( 0x31311000 == pCreepbit ||
				0x21311000 == pCreepbit ||
				0x21211000 == pCreepbit ||
				0x31211000 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 20;

			else if( 0x31010311 == pCreepbit ||
				0x31010211 == pCreepbit ||
				0x21010311 == pCreepbit ||
				0x21010211 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 29;

			else if((0x01000010 & pCreepbit) == 0x01000010)
				m_creepTile[idx]->creep_sequence = 27;

			else if(  0x21200212 == pCreepbit ||
				0x11200212 == pCreepbit ||
				0x21100212 == pCreepbit ||
				0x11100212 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 3;

			else if(  0x21200112 == pCreepbit ||
				0x11200112 == pCreepbit ||
				0x21100112 == pCreepbit ||
				0x11100112 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 2;

			else if(  0x21200211 == pCreepbit ||
				0x11200211 == pCreepbit ||
				0x21100211 == pCreepbit ||
				0x11100211 == pCreepbit)
				m_creepTile[idx]->creep_sequence = 4;

			else
			{
				//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_creepTile[idx]->creep_cnt ,
				//	m_sqTile[idx]->vPos.x , m_sqTile[idx]->vPos.y);
				continue;
			}


			icreep_squence = m_creepTile[idx]->creep_sequence;

			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw( (*ptemp)[icreep_squence]->pTexture , NULL , &D3DXVECTOR3(16, 16,0), NULL
				,D3DCOLOR_ARGB(255,255,255,255));

			//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_creepTile[idx]->creep_cnt ,
			//	m_sqTile[idx]->vPos.x , m_sqTile[idx]->vPos.y);
		}
	}

}
void CTileManager::Creep_Autotile(const int& idx)
{
	int UP_idx	 = -1;
	int DOWN_idx = -1;
	int LU_idx	 = -1;
	int RU_idx	 = -1;
	int L_idx	 = -1;
	int R_idx	 = -1;
	int DL_idx	 = -1;
	int DR_idx	 = -1;



	if(0 != idx / SQ_TILECNTX)
		UP_idx	 = idx - SQ_TILECNTX;

	if(SQ_TILECNTX - 1 != idx / SQ_TILECNTX)
		DOWN_idx = idx + SQ_TILECNTX;

	if(0 != idx / SQ_TILECNTX)
		LU_idx	 = idx - SQ_TILECNTX - 1;

	if(0 != idx / SQ_TILECNTX)
		RU_idx	 = idx - SQ_TILECNTX + 1;

	if(0 != idx % SQ_TILECNTX) 
		L_idx	 = idx - 1;

	if(SQ_TILECNTX - 1 != idx % SQ_TILECNTX)
		R_idx	 = idx + 1;

	if(SQ_TILECNTX - 1 != idx / SQ_TILECNTX)
		DL_idx	 = idx + SQ_TILECNTX - 1;

	if(SQ_TILECNTX - 1 != idx / SQ_TILECNTX)
		DR_idx	 = idx + SQ_TILECNTX + 1;

	/*
	0열 1열 
	0행	 0,  0, 
	1행	 0,  0, 


	m_creepTile[idx]->creep_bit = false; 
	false는 그 인덱스에 크립이 없다는 의미

	/*
	8개의 비트중 앞 세자리는 LU , UP , RU
	그다음 두 자리는 L , R
	그 다음 세자리는 LD , DOWN , RD 이다.
	*/

	//m_creepTile[UP_idx]->creep_bit		+= 0x00000111;
	//m_creepTile[DOWN_idx]->creep_bit	+= 0x11100000;
	//m_creepTile[LU_idx]->creep_bit		+= 0x00000001;
	//m_creepTile[RU_idx]->creep_bit		+= 0x00000100;
	//m_creepTile[L_idx]->creep_bit		+= 0x00101001;
	//m_creepTile[R_idx]->creep_bit		+= 0x10010100;
	//m_creepTile[DL_idx]->creep_bit		+= 0x00100000;
	//m_creepTile[DR_idx]->creep_bit		+= 0x10000000;

	
	m_creepTile[idx]->creep_cnt += 1;

	if(m_creepTile[idx]->creep_cnt >= 2)
		return;

	m_creepTile[idx]->creep_bit = 0X33333333;

	if( UP_idx >= 0 && m_creepTile[UP_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[UP_idx]->creep_bit		+= 0x00000111;
	}

	if( DOWN_idx >= 0 && m_creepTile[DOWN_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[DOWN_idx]->creep_bit	+= 0x11100000;
	}

	if( LU_idx >= 0 && m_creepTile[LU_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[LU_idx]->creep_bit		+= 0x00000001;
	}

	if( RU_idx >= 0 && m_creepTile[RU_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[RU_idx]->creep_bit		+= 0x00000100;
	}

	if( L_idx >= 0 && m_creepTile[L_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[L_idx]->creep_bit		+= 0x00101001;
	}

	if( R_idx >= 0 && m_creepTile[R_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[R_idx]->creep_bit		+= 0x10010100;
	}

	if( DL_idx >= 0 && m_creepTile[DL_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[DL_idx]->creep_bit		+= 0x00100000;
	}

	if( DR_idx >= 0 && m_creepTile[DR_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[DR_idx]->creep_bit		+= 0x10000000;
	}
}
void CTileManager::Creep_decrease_autotile(const int& idx)
{	

	//크립 줄어드는건 시체에서 하자

	int UP_idx	 = -1;
	int DOWN_idx = -1;
	int LU_idx	 = -1;
	int RU_idx	 = -1;
	int L_idx	 = -1;
	int R_idx	 = -1;
	int DL_idx	 = -1;
	int DR_idx	 = -1;


	if(0 != idx / SQ_TILECNTX)
		UP_idx	 = idx - SQ_TILECNTX;

	if(SQ_TILECNTX - 1 != idx / SQ_TILECNTX)
		DOWN_idx = idx + SQ_TILECNTX;

	if(0 != idx / SQ_TILECNTX)
		LU_idx	 = idx - SQ_TILECNTX - 1;

	if(0 != idx / SQ_TILECNTX)
		RU_idx	 = idx - SQ_TILECNTX + 1;

	if(0 != idx % SQ_TILECNTX) 
		L_idx	 = idx - 1;

	if(SQ_TILECNTX - 1 != idx % SQ_TILECNTX)
		R_idx	 = idx + 1;

	if(SQ_TILECNTX - 1 != idx / SQ_TILECNTX)
		DL_idx	 = idx + SQ_TILECNTX - 1;

	if(SQ_TILECNTX - 1 != idx / SQ_TILECNTX)
		DR_idx	 = idx + SQ_TILECNTX + 1;

		
	m_creepTile[idx]->creep_cnt -= 1;

	if(m_creepTile[idx]->creep_cnt >= 1)
		return;

	m_creepTile[idx]->creep_bit = 0x00000000;

	///////////
	if( UP_idx >= 0 && m_creepTile[UP_idx]->creep_bit != 0 && m_creepTile[UP_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[UP_idx]->creep_bit		-= 0x00000111;
	}

	if( DOWN_idx >= 0 && m_creepTile[DOWN_idx]->creep_bit != 0 && m_creepTile[DOWN_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[DOWN_idx]->creep_bit	-= 0x11100000;
	}

	if( LU_idx >= 0 && m_creepTile[LU_idx]->creep_bit != 0 && m_creepTile[LU_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[LU_idx]->creep_bit		-= 0x00000001;
	}

	if( RU_idx >= 0 && m_creepTile[RU_idx]->creep_bit != 0 && m_creepTile[RU_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[RU_idx]->creep_bit		-= 0x00000100;
	}

	if( L_idx >= 0 && m_creepTile[L_idx]->creep_bit != 0 && m_creepTile[L_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[L_idx]->creep_bit		-= 0x00101001;
	}

	if( R_idx >= 0 && m_creepTile[R_idx]->creep_bit != 0 && m_creepTile[R_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[R_idx]->creep_bit		-= 0x10010100;
	}

	if( DL_idx >= 0 && m_creepTile[DL_idx]->creep_bit != 0 && m_creepTile[DL_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[DL_idx]->creep_bit		-= 0x00100000;
	}

	if( DR_idx >= 0 && m_creepTile[DR_idx]->creep_bit != 0 && m_creepTile[DR_idx]->creep_bit != 0x33333333)
	{
		m_creepTile[DR_idx]->creep_bit		-= 0x10000000;
	}

	////////////////////////////////////

	if( UP_idx >= 0 && m_creepTile[UP_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit		+= 0x11100000;
	}

	if( DOWN_idx >= 0 && m_creepTile[DOWN_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit	+= 0x00000111;
	}

	if( LU_idx >= 0 && m_creepTile[LU_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit		+= 0x10000000;
	}

	if( RU_idx >= 0 && m_creepTile[RU_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit		+= 0x00100000;
	}

	if( L_idx >= 0 && m_creepTile[L_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit		+= 0x10010100;
	}

	if( R_idx >= 0 && m_creepTile[R_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit		+= 0x00101001;
	}

	if( DL_idx >= 0 && m_creepTile[DL_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit		+= 0x00000100;
	}

	if( DR_idx >= 0 && m_creepTile[DR_idx]->creep_bit == 0x33333333)
	{
		m_creepTile[idx]->creep_bit		+= 0x00000001;
	}
}
void CTileManager::SightOffRender(const int& idx ,const TEAM_NUMBER& eteam)
{
	m_fogTile[idx]->overlap_cnt[eteam] -= 1;

	if(m_fogTile[idx]->overlap_cnt[eteam] <= 0)
	{
		m_fogTile[idx]->overlap_cnt[eteam] = 0;	

		m_fogTile[idx]->fog_color[eteam] = D3DCOLOR_ARGB(50,0,0,0);
		m_fogTile[idx]->eSight[eteam] = FOG_GREY;

		if(CSession_Mgr::GetInstance()->GetTeamNumber() == eteam)
			m_minifog_offidx.push_back(idx);
	}
}
void CTileManager::SightOnRender(const int idx ,const int irange , vector<int>& sightoff_list , bool* fogsearch ,CObj* pobj)
{
	//range는 가급적 홀수

	//int fradius = irange/2*32; irange는 픽셀
	int fradius = (irange/2);

	int half_range = fradius / SQ_TILESIZEX; //halfrange는 타일갯수차이

	int startIdx = idx;
	int destidx = 0;

	TEAM_NUMBER eteam = pobj->GetTeamNumber();
	if(false == fogsearch[idx])
	{
		fogsearch[idx] = true;
		sightoff_list.push_back(startIdx);

		m_fogTile[idx]->overlap_cnt[eteam] += 1;

		m_fogTile[idx]->eSight[eteam] = FOG_ALPHA;
		SetFogLight(idx , eteam);
	}


	int LUvtx_idx;// = idx - SQ_TILECNTX*half_range - half_range;
	int RUvtx_idx;// = idx - SQ_TILECNTX*half_range + half_range;
	int LDvtx_idx;// = idx + SQ_TILECNTX*half_range - half_range;
	int RDvtx_idx;// = idx + SQ_TILECNTX*half_range + half_range;


	int topidx = (idx - half_range*SQ_TILECNTX)/SQ_TILECNTX;
	int bottomidx = (idx + half_range*SQ_TILECNTX)/SQ_TILECNTX;
	int leftidx = (idx - half_range)%SQ_TILECNTX;
	int rightidx = (idx + half_range)%SQ_TILECNTX;

	if( idx / SQ_TILECNTX <= half_range) //인덱스가 위쪽을 초과할때
	{
		topidx = (idx -  (idx / SQ_TILECNTX) * SQ_TILECNTX)/ SQ_TILECNTX; //위쪽끝으로 붙인다
	}
	if( SQ_TILECNTX - idx / SQ_TILECNTX <= half_range ) //아래를 초과할때
	{
		bottomidx = (idx + (SQ_TILECNTX - 1 - (idx / SQ_TILECNTX)) * SQ_TILECNTX) / SQ_TILECNTX;
	}
	if( idx % SQ_TILECNTX <= half_range) //인덱스가 왼쪽을 넘어갈때
	{
		leftidx = (idx - (idx % SQ_TILECNTX)) % SQ_TILECNTX;
	}
	if( SQ_TILECNTX - (idx % SQ_TILECNTX) <= half_range)//인덱스가 오른쪽을 넘어갈때
	{
		rightidx = (idx + ( SQ_TILECNTX - 1 - (idx % SQ_TILECNTX) ) ) % SQ_TILECNTX;
	}

	LUvtx_idx = topidx * SQ_TILECNTX + leftidx;
	LDvtx_idx = bottomidx * SQ_TILECNTX + leftidx;
	RUvtx_idx = topidx * SQ_TILECNTX + rightidx;
	RDvtx_idx = bottomidx * SQ_TILECNTX + rightidx;


	for(int j = LUvtx_idx; j <= RUvtx_idx; ++j) //윗줄 
	{
		destidx = j;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , pobj);
	}

	for(int j = LDvtx_idx; j <= RDvtx_idx; ++j) //아랫줄 
	{
		destidx = j;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , pobj);
	}

	for(int i = LUvtx_idx + SQ_TILECNTX; i < LDvtx_idx; i += SQ_TILECNTX) //왼쪽 열
	{
		destidx = i;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , pobj);
	}

	for(int i = RUvtx_idx + SQ_TILECNTX; i < RDvtx_idx; i += SQ_TILECNTX) //오른쪽 열
	{
		destidx = i;
		Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , sightoff_list , fogsearch , pobj);
	}
}

void CTileManager::MinifogUpdate(void)
{
	//m_fogmat._11 = 1;
	//m_fogmat._22 = 1;
	//m_fogmat._41 = 85;
	//m_fogmat._42 = 470;
	//m_pSprite->SetTransform(&m_fogmat);

	//m_pSprite->Draw(m_MinifogTexture , NULL , &D3DXVECTOR3(0,0,0), NULL
	//	, D3DCOLOR_ARGB(255,255,255,255));


	D3DLOCKED_RECT	lrect;
	LPDIRECT3DSURFACE9	psurface;
	size_t maxloop;


	if(!m_minifog_offidx.empty())
	{
		maxloop = m_minifog_offidx.size();

		m_MinifogTexture->GetSurfaceLevel(0 , &psurface);
		psurface->LockRect(&lrect , NULL , 0);
		DWORD* pcolor = (DWORD*)lrect.pBits;

		for(size_t i = 0; i < maxloop; ++i)
		{
			pcolor[ m_minifog_offidx[i] ] = 0x88000000;
		}
		psurface->UnlockRect();
		psurface->Release();

		m_minifog_offidx.clear();
	}

	if(!m_minifog_onidx.empty())
	{
		maxloop = m_minifog_onidx.size();

		m_MinifogTexture->GetSurfaceLevel(0 , &psurface);
		psurface->LockRect(&lrect , NULL , 0);
		DWORD* pcolor = (DWORD*)lrect.pBits;

		for(size_t i = 0; i < maxloop; ++i)
		{
			pcolor[ m_minifog_onidx[i] ] = 0x00ffffff;
		}
		psurface->UnlockRect();
		psurface->Release();

		m_minifog_onidx.clear();
	}


}
void CTileManager::SetFogSquence(int idx , unsigned short sequence)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return;

	//m_fogTile[idx]->fog_sequence		 = sequence;
}
void CTileManager::SetFogLight(int idx , const TEAM_NUMBER& eteam)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return;

	//if(false == m_fogTile[idx]->bLight[eteam])
	//	m_fogTile[idx]->bLight[eteam] = true;

	if(FOG_BLACK == m_fogTile[idx]->eSight[eteam] ||
		FOG_GREY == m_fogTile[idx]->eSight[eteam])
		m_fogTile[idx]->eSight[eteam] = FOG_ALPHA;

	if( m_fogTile[idx]->fog_color[eteam] <= D3DCOLOR_ARGB(10,0,0,0))
		return;

	m_fogTile[idx]->fog_color[eteam] = D3DCOLOR_ARGB(0 , 0,0,0 );

	if(CSession_Mgr::GetInstance()->GetTeamNumber() == eteam)
		m_minifog_onidx.push_back(idx);

}
void CTileManager::SetCreepInstall(const int& idx , bool binstall)
{
	m_creepTile[idx]->bcreep_install = binstall;
}
void CTileManager::SetFogColor(const int& idx , D3DCOLOR color)
{
	//m_fogTile[idx]->fog_color = color;
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
int CTileManager::GetTileOption(const int& idx)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return -1;

	//enum TILE_OPTION값 참조
	return m_sqTile[idx]->byOption;
}
int CTileManager::GetTileFloor(const int& idx)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return -1;

	//enum TILE_OPTION값 참조
	return m_sqTile[idx]->byFloor;
}
void CTileManager::CopySurface(LPDIRECT3DTEXTURE9 ptexturemap)
{
	LPDIRECT3DSURFACE9 pBackBuffer , pSurface;
	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	pdevice->GetRenderTarget(0 , &pBackBuffer);

	ptexturemap->GetSurfaceLevel(0, &pSurface);

	//pBackBuffer를 pSurface에 담는다
	pdevice->StretchRect(pBackBuffer , NULL, pSurface ,NULL , D3DTEXF_NONE);

	pBackBuffer->Release();
	pSurface->Release();
}

void CTileManager::LoadTileData(HANDLE hFile)
{
	DWORD dwbyte;
	int mapsize = 0;
	int	basecnt = 0;
	int sortsize = -1;

	ReadFile(hFile , &basecnt, sizeof(int), &dwbyte, NULL); //임시,

	ReadFile(hFile , &mapsize, sizeof(int), &dwbyte, NULL);
	m_mapsize = mapsize;

	m_terrainInfo_List.reserve(mapsize);

	TILE* tempTile = NULL;
	TERRAIN_INFO* tempterrain = NULL;


	int idx64 = 0;
	MYRECT<float> temprc;
	MYRECT<float> tempvtx;
	tempvtx.left = 16;
	tempvtx.right = 16;
	tempvtx.top = 16;
	tempvtx.bottom = 16;

	for(int i = 0; i < mapsize; ++i)
	{
		tempTile = new TILE();
		ReadFile(hFile, tempTile, sizeof(TILE), &dwbyte, NULL);

		m_sqTile[i] = tempTile;

		if(MOVE_NONE == m_sqTile[i]->byOption)
		{
			idx64 = CMyMath::Pos_to_index(m_sqTile[i]->vPos , 64);
			temprc.left = m_sqTile[i]->vPos.x - 16;
			temprc.right = m_sqTile[i]->vPos.x + 16;
			temprc.top = m_sqTile[i]->vPos.y - 16;
			temprc.bottom = m_sqTile[i]->vPos.y + 16;

			CObj* tempobj = new CObj;		
			tempobj->SetPos(m_sqTile[i]->vPos);
			tempobj->SetMyrect(temprc);
			tempobj->SetVertex(tempvtx);
			CArea_Mgr::GetInstance()->SetObj_Area64(idx64 , idx64 , tempobj);
			CArea_Mgr::GetInstance()->setobstacle(tempobj);
		}
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
	ReadyMiniMap();

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

	//CDevice::GetInstance()->GetDevice()->Clear(0 , NULL
	//	, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
	//	, D3DCOLOR_XRGB(0,0,255), 1.f , 0);

	for(int Y = 0; Y < MAP_TEXTURECNTY; ++Y)
	{
		for(int X = 0; X < MAP_TEXTURECNTX; ++X)
		{
			istartidx = ((Y*BACKBUFFER_SIZEY)/SQ_TILESIZEY)*SQ_TILECNTX + (X*BACKBUFFER_SIZEX)/SQ_TILESIZEX;

			CDevice::GetInstance()->Render_Begin();

			for(int i = 0; i < CULLINGCNTY; ++i)
			{		
				
				for(int j = 0; j < CULLINGCNTX; ++j)
				{
					//CDevice::GetInstance()->Render_Begin();

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
					//CDevice::GetInstance()->Render_End();
					//CopySurface(m_MapTexture[Y][X]);
				}
			}

			CDevice::GetInstance()->Render_End();
			CopySurface(m_MapTexture[Y][X]);
		}
	}

	//CDevice::GetInstance()->GetDevice()->Present(NULL, NULL , NULL , NULL);
}
void CTileManager::ReadyMiniMap(void)
{	

	list<TERRAIN_INFO*>::iterator iter;
	list<TERRAIN_INFO*>::iterator iter_end;

	LPDIRECT3DTEXTURE9 pTexture;
	BYTE byTerrain_ID = 0;
	BYTE byGroup_ID = 0;
	BYTE byGroup_sequence = 0;

	int idx = 0;

	D3DXMATRIX tempmat;
	D3DXMatrixIdentity(&tempmat);

	//CDevice::GetInstance()->GetDevice()->Clear(0 , NULL
	//	, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
	//	, D3DCOLOR_XRGB(0,0,255), 1.f , 0);

	CDevice::GetInstance()->Render_Begin();

	float ratioX = BACKBUFFER_SIZEX / 4096.f;
	float ratioY = BACKBUFFER_SIZEY / 4096.f;

	for(int i = 0; i < SQ_TILECNTY; ++i)
	{
		for(int j = 0; j < SQ_TILECNTX; ++j)
		{
			//CDevice::GetInstance()->Render_Begin();

			idx = i * SQ_TILECNTX + j;

			//tempmat._11 = 0.2f;
			//tempmat._22 = 0.15f;
			//tempmat._41 = m_sqTile[idx]->vPos.x * 0.2f;
			//tempmat._42 = m_sqTile[idx]->vPos.y * 0.15f;

			tempmat._11 = ratioX;
			tempmat._22 = ratioY;
			tempmat._41 = m_sqTile[idx]->vPos.x * ratioX;
			tempmat._42 = m_sqTile[idx]->vPos.y * ratioY;

			m_pSprite->SetTransform(&tempmat);

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

				m_pSprite->Draw(pTexture , NULL , &D3DXVECTOR3(16, 16 , 0) , NULL ,
					m_TileColor); //백버퍼크기에 그려넣는 작업이다..
				
			}
			//CDevice::GetInstance()->Render_End();

			//CopySurface(m_MinimapTexture);
		}

	}

	CDevice::GetInstance()->Render_End();

	CopySurface(m_MinimapTexture);

	CDevice::GetInstance()->GetDevice()->Clear(0 , NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_XRGB(0,0,0), 1.f , 0);

	//CDevice::GetInstance()->GetDevice()->Present(NULL, NULL , NULL , NULL);

}
FOGSIGHT_OPTION CTileManager::GetFogLight(const int& idx , const TEAM_NUMBER& eteam)
{
	//return m_fogTile[idx]->bLight[eteam];
	return m_fogTile[idx]->eSight[eteam];
}
bool CTileManager::GetCreepInstall(const int& idx)
{
	if(m_creepTile[idx]->creep_cnt == 0)
		return false;
	else
		 return true;
}

TILE** CTileManager::GetSqTile(void)
{
	return m_sqTile;
}

CREEP_INFO** CTileManager::GetCreepTile(void)
{
	return m_creepTile;
}

void CTileManager::SetFogoverlap_cnt(const int& idx , const TEAM_NUMBER& eteam)
{
	//유닛 시야끼리 겹친부분의 타일을 +1한다
	m_fogTile[idx]->overlap_cnt[eteam] += 1;
}
bool CTileManager::Bresenham_Tilecheck(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest )
{
	int iWidth = int(vDest.x - vStart.x);
	int iHeight = int(vDest.y - vStart.y);

	int e = 0;
	int icnt = 0;
	int idx = 0;
	int iAdd = 32;

	int loopcnt = 0;
	int isignX = 1;
	int isignY = 1;

	int opvalue = 0;

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

			idx = CMyMath::Pos_to_index(vStart.x + i * SQ_TILESIZEX * isignX, vStart.y + icnt * isignY);

			opvalue = GetTileOption(idx);
			if( opvalue < 0 || MOVE_NONE == opvalue)
				return true;
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

			//idx = CMyMath::Pos_to_index(vStart.x + i * SQ_TILESIZEX * isignX, vStart.y + icnt * isignY);
			idx = CMyMath::Pos_to_index(vStart.x + icnt * isignX , vStart.y + i*SQ_TILESIZEY*isignY);

			opvalue = GetTileOption(idx);
			if( opvalue < 0 || MOVE_NONE == opvalue)
				return true;
		}
	}

	return false;
}
void CTileManager::Bresenham_fog(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int fRadius ,vector<int>& light_IdxList , bool* fogsearch, CObj* pobj)
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

	MOVE_TYPE etype = pobj->GetUnitinfo().eMoveType;
	TEAM_NUMBER eteam = pobj->GetTeamNumber();

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
						light_IdxList.push_back(idx);
						m_fogTile[idx]->overlap_cnt[eteam] += 1;											
					}

					m_fogTile[idx]->eSight[eteam] = FOG_ALPHA;
					SetFogLight(idx , eteam);
					//}
				}
				else if(MOVE_GROUND == etype)
				{
					if(true == CTileManager::GetInstance()->CheckFogFloor(myidx , idx))//공중일 경우 언덕지형을 신경안쓴다.
					{					
						if(false == fogsearch[idx])
						{						
							fogsearch[idx] = true;
							light_IdxList.push_back(idx);
							m_fogTile[idx]->overlap_cnt[eteam] += 1;											
						}

						m_fogTile[idx]->eSight[eteam] = FOG_ALPHA;
						SetFogLight(idx , eteam);
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
						light_IdxList.push_back(idx);
						m_fogTile[idx]->overlap_cnt[eteam] += 1;								
					}

					m_fogTile[idx]->eSight[eteam] = FOG_ALPHA;
					SetFogLight(idx , eteam);
				}
				else if(MOVE_GROUND == etype)
				{
					if(true == CheckFogFloor(myidx , idx))//공중일 경우 신경안쓴다.
					{					
						if(false == fogsearch[idx])
						{						
							fogsearch[idx] = true;
							light_IdxList.push_back(idx);
							m_fogTile[idx]->overlap_cnt[eteam] += 1;			
						}

						m_fogTile[idx]->eSight[eteam] = FOG_ALPHA;
						SetFogLight(idx , eteam);
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
void CTileManager::Flowfield_Pathfinding(const D3DXVECTOR2& goalpos)
{
	//D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
	int			goalidx = CMyMath::Pos_to_index(goalpos , 32);


	for(int i = 0; i < 16384; ++i)
	{
		m_flownode[i]->bcheck = false;
	}
	m_flowdestidx[goalidx] = -1;

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

			//가로 세로 10 , 대각 14
			if( LEFT == i || RIGHT == i || UP == i || DOWN == i)
				pushnode->iCost = curcost + 10;
			else
				pushnode->iCost = curcost + 14;

			if(false == pushnode->bmove)
				pushnode->iCost += 10000;

			m_heapsort->push_node(pushnode);
		}
	}

	//CUnitMgr::GetInstance()->path_relay(m_flowfield_goalidx);
}
void CTileManager::Render_Flowfield(void)
{
	if(!CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FLOWPATH])
		return;

	const TEXINFO*	ptemp = NULL;
	int ifogsquence = 1;

	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx = 0;

	for(int i = 0; i < CULLINGCNTY; ++i)
	{
		for(int j = 0; j < CULLINGCNTX; ++j)
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

			//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_flownode[idx]->bmove , m_matWorld._41 , m_matWorld._42);
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


	Safe_Delete(m_heapsort);

	for(int i = 0; i < MAP_TEXTURECNTY; ++i)
	{
		for(int j = 0; j < MAP_TEXTURECNTX; ++j)
		{
			m_MapTexture[i][j]->Release();
		}
	}
	m_MinimapTexture->Release();

	vector<int> temp1 , temp2;
	temp1.swap(m_minifog_offidx);
	temp2.swap(m_minifog_onidx);
}

LPDIRECT3DTEXTURE9 CTileManager::GetMiniampTexture(void)
{
	return m_MinimapTexture;
}

LPDIRECT3DTEXTURE9 CTileManager::GetMiniFogmapTexture(void)
{
	return m_MinifogTexture;
}
