#include "StdAfx.h"
#include "TileManager.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "MouseMgr.h"
#include "MyMath.h"
#include "FontMgr.h"

IMPLEMENT_SINGLETON(CTileManager)

CTileManager::CTileManager(void)
{
	for(int i = 0; i < 7; ++i)
		for(int j = 0; j < 6; ++j)
			m_MapTexture[i][j] = NULL;

	m_fogTexture = NULL;
	m_fogMaskTexture = NULL;
}

CTileManager::~CTileManager(void)
{
	Release();
}
void CTileManager::Initialize(void)
{
	m_pSprite = CDevice::GetInstance()->GetSprite();
	D3DXMatrixIdentity(&m_matWorld);

	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	for(int i = 0; i < 7; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			D3DXCreateTexture(pdevice , 800, 600, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET
				,D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_MapTexture[i][j]);
		}
	}

	//m_fogTile.reserve(SQ_TILECNTY*SQ_TILECNTX);
	FOG_INFO*	pfoginfo_temp = NULL;
	CREEP_INFO*	pcreep_temp = NULL;
	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
		pfoginfo_temp = new FOG_INFO();
		m_fogTile[i] = pfoginfo_temp;
		//m_fogTile.push_back(pfoginfo_temp);

		pcreep_temp = new CREEP_INFO();
		m_creepTile[i] = pcreep_temp;
	}

	m_fogTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Fog" , L"blackfog4096")->pTexture;
	m_fogMaskTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Fog" , L"fogmask640")->pTexture;

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
	float fspeed = 1500.f;
	if(GetAsyncKeyState(VK_LEFT))
	{
		CScrollMgr::m_fScrollX -= GETTIME*fspeed;
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		CScrollMgr::m_fScrollX += GETTIME*fspeed;
	}
	if(GetAsyncKeyState(VK_UP))
	{
		CScrollMgr::m_fScrollY -= GETTIME*fspeed;
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		CScrollMgr::m_fScrollY += GETTIME*fspeed;
	}

	if(0 > CScrollMgr::m_fScrollX)
		CScrollMgr::m_fScrollX = 0;
	else if(3296 <= CScrollMgr::m_fScrollX)
		CScrollMgr::m_fScrollX = 3296;

	if(0 > CScrollMgr::m_fScrollY)
		CScrollMgr::m_fScrollY = 0;
	else if(3496 <= CScrollMgr::m_fScrollY)
		CScrollMgr::m_fScrollY = 3496;

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






	CreepAlgorithm();
	FogAlgorithm();

	RenderCreep();
	RenderFog();
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


	//D3DXVECTOR2	vPoint[2];

	//int scrollX = (int)CScrollMgr::m_fScrollX;
	//int scrollY = (int)CScrollMgr::m_fScrollY;

	//CDevice::GetInstance()->Render_End();
	//CDevice::GetInstance()->Render_Begin();

	//for(int j = 0; j < 40; ++j)
	//{
	//	int index = j + scrollX/SQ_TILESIZEX;
	//	vPoint[0] = D3DXVECTOR2( float(index*SQ_TILESIZEX) - scrollX,0 - (float)scrollY);
	//	vPoint[1] = D3DXVECTOR2( float(index*SQ_TILESIZEX) - scrollX, float(SQ_TILECNTY*SQ_TILESIZEY) - scrollY);

	//	CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
	//	CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,255,0));
	//}
	//for(int j = 0; j < 32; ++j)
	//{
	//	int index = j + scrollY/SQ_TILESIZEY;
	//	vPoint[0] = D3DXVECTOR2(0 - (float)scrollX ,float(index*SQ_TILESIZEX) - scrollY );
	//	vPoint[1] = D3DXVECTOR2(float (SQ_TILECNTX*SQ_TILESIZEX) - scrollX, float(index*SQ_TILESIZEX) - scrollY);

	//	CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
	//	CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,255,0));
	//}

	//CDevice::GetInstance()->Render_End();
	//CDevice::GetInstance()->Render_Begin();
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

			ifogsquence = m_fogTile[idx]->fog_sequence;

			if(0 == ifogsquence)
				continue;

			fCenterX = (ptemp[ifogsquence]->ImgInfo.Width >> 1);
			fCenterY = (ptemp[ifogsquence]->ImgInfo.Height >> 1);

			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(ptemp[ifogsquence]->pTexture , NULL , &D3DXVECTOR3((float)fCenterX, (float)fCenterY,0), NULL
				,m_fogTile[idx]->fog_color);
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


			icreep_squence = m_creepTile[idx]->creep_sequence;

			if(icreep_squence < 0)
				continue;

			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(ptemp[icreep_squence]->pTexture , NULL , &D3DXVECTOR3(16, 16,0), NULL
				,D3DCOLOR_ARGB(255,255,255,255));
		}
	}

}

void CTileManager::FogAlgorithm(void/*여기에도 매개변수가 필요*/)
{
	static int oldIdx = -1;

	//이게 한 유닛의 시야알고리즘

	m_vcurPos.x = CMouseMgr::GetMousePt().x /*유닛의좌표*/ + CScrollMgr::m_fScrollX;
	m_vcurPos.y = CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY;



	m_icuridx = CMyMath::Pos_to_index(m_vcurPos.x , m_vcurPos.y); 

	static bool fog_offswitch = false;

	if(true == fog_offswitch)
		m_fTimeTest += GETTIME;


	if(m_fTimeTest > 3.0f)
	{
		if(!m_LightOff_List.empty())
		{
			list<int>::iterator iter = m_LightOff_List.begin();
			list<int>::iterator iter_end = m_LightOff_List.end();

			for( ; iter != iter_end; ++iter)
				SightOffRender(*iter);

			m_LightOff_List.clear();
		}
		m_fTimeTest = 0.f;
		fog_offswitch = false;

		SightOnRender(m_vcurPos , 19);
	}


	if(m_icuridx != oldIdx)
	{
		oldIdx = m_icuridx;
		fog_offswitch = true;
	}
	else
		return;



	SightOnRender(m_vcurPos , 19);

}
void CTileManager::CreepAlgorithm(void)
{
	m_vcurPos.x = CMouseMgr::GetMousePt().x /*유닛의좌표*/ + CScrollMgr::m_fScrollX;
	m_vcurPos.y = CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY;


	Creep_increase(m_vcurPos , 13);
}
void CTileManager::Creep_increase(const D3DXVECTOR2& vPos/*유닛의 위치*/ , int irange)
{
	int fradius = irange/2*32;
	int istartX = int( vPos.x - fradius )/SQ_TILESIZEX;
	int istartY = int( vPos.y - fradius )/SQ_TILESIZEY;

	int startIdx = CMyMath::Pos_to_index(vPos.x , vPos.y);	
	int destidx = 0;

	//if(false == GetFogLight(startIdx)) 크립으로 대신한다
	//m_LightOff_List.push_back(startIdx);

	CTileManager::GetInstance()->SetCreepInstall(startIdx , true);


	if(istartY < 0)
		istartY = 0;
	if(istartY + irange - 1 >= SQ_TILECNTY)
		istartY = SQ_TILECNTY - irange;

	if(istartX < 0)
		istartX = 0;
	if(istartX + irange - 1 >= SQ_TILECNTX)
		istartX = SQ_TILECNTX - irange;


	static int offset = irange >> 1;

	enum LINE_DIR{LINE_LDIR , LINE_RDIR , LINE_END};
	enum UPDOWN_DIR{LINE_UP , LINE_DOWN , UPDOWN_END};
	static int top_line[LINE_END] = {offset, offset};
	static int bot_line[LINE_END] = {offset, offset};
	static int left_line[UPDOWN_END] = {offset, offset};
	static int right_line[UPDOWN_END] = {offset, offset};

	static float timer = 0.f;
	static int tickcnt = 0;

	static int loopcnt1 = 1;
	static int loopcnt2 = 1;
	static bool bswitch = true;

	timer += GETTIME;

	while(bswitch)//초기화 단계에서 한번만 실행 
	{
		--top_line[LINE_LDIR];
		++top_line[LINE_RDIR];

		--bot_line[LINE_LDIR];
		++bot_line[LINE_RDIR];

		--left_line[LINE_UP];
		++left_line[LINE_DOWN];

		--right_line[LINE_UP];
		++right_line[LINE_DOWN];

		++tickcnt;

		if(tickcnt > offset)
		{
			//++loopcnt1;
			tickcnt = 0;			
		}


		if( irange - 1 <= left_line[LINE_DOWN] )
		{
			left_line[LINE_DOWN] = offset;
			right_line[LINE_DOWN] = offset;
			++loopcnt2;
		}
		if( left_line[LINE_UP] < 1 )
		{
			left_line[LINE_UP] = offset;
			right_line[LINE_UP] = offset;
		}


		if(irange  <= top_line[LINE_RDIR])
		{
			top_line[LINE_RDIR] = offset;
			bot_line[LINE_RDIR] = offset;
			++loopcnt1;
		}
		if(top_line[LINE_LDIR] < 0)
		{
			top_line[LINE_LDIR] = offset;
			bot_line[LINE_LDIR] = offset;
		}

		if(loopcnt1 >= (irange >> 1) + 1)
		{
			bswitch = false;
			break;
		}
		

		for(int k = 0; k < LINE_END; ++k)
		{
			destidx = istartY*SQ_TILECNTX + (istartX + top_line[k]);

			if(istartX + top_line[k] >= SQ_TILECNTX || istartX + top_line[k] < 0)
				continue;
			if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,loopcnt1 , m_CreepOff_List);
		}


		for(int k = 0; k < LINE_END; ++k)
		{
			destidx = (istartY + irange - 1)*SQ_TILECNTX + (istartX + bot_line[k]);

			if(istartX + bot_line[k] >= SQ_TILECNTX || istartX + bot_line[k] < 0)
				continue;
			if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,loopcnt1, m_CreepOff_List);
		}


		//============================================================================================================================================

		for(int k = 0; k < UPDOWN_END; ++k)
		{
			destidx = (istartY + left_line[k])*SQ_TILECNTX + istartX;

			if(istartX < 0 || istartX >= SQ_TILECNTX)
				continue;
			if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,loopcnt2, m_CreepOff_List);
		}


		for(int k = 0; k < UPDOWN_END; ++k)
		{
			destidx = (istartY + right_line[k])*SQ_TILECNTX + (istartX + irange - 1);

			if(istartX + irange - 1 < 0 || istartX + irange - 1 >= SQ_TILECNTX)
				continue;
			if(destidx < 0 || destidx >= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,loopcnt2, m_CreepOff_List);
		}
	}

	static bool once = true;
	if(once)
	{
		for(int i = 0; i < 81; ++i) //초기화 단계에서 한번만 실행 
		{
			int idx = m_CreepOff_List.front();
			m_CreepOff_List.pop_front();

			if(false == GetCreepInstall(idx))
			{
				SetCreepInstall(idx , true);
				Creep_Autotile(idx);
			}
		}
		once = false;
	}

	if(timer >= 0.5f) //업데이트단계
	{
		timer = 0.f;
		if(!m_CreepOff_List.empty())
		{
			while(true)
			{
				int idx = m_CreepOff_List.front();

				m_CreepOff_List.pop_front();

				if(false == GetCreepInstall(idx))
				{
					SetCreepInstall(idx , true);
					Creep_Autotile(idx);
					break;
				}
			}

			int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
			int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
			int curidx = 0;
			CREEP_INFO*	pCreep_temp = NULL;

			for(int i = 0; i < irange + 2; ++i)
			{
				for(int j = 0; j < irange + 3; ++j)
				{
					curidx = (istartY+i) * SQ_TILECNTX + (istartX + j);

					if(curidx < 0 || curidx>= SQ_TILECNTY*SQ_TILECNTX)
						continue;


					pCreep_temp = m_creepTile[curidx];

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
				}
			}
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
void CTileManager::Creep_decrease(void)
{
}
void CTileManager::SightOffRender(const int& idx)
{
	m_fogTile[idx]->bLight = false;
	m_fogTile[idx]->eSight = FOG_GREY;


	m_fogTile[idx]->fog_color = D3DCOLOR_ARGB(100,0,0,0);
	m_fogTile[idx]->fog_sequence = 1;
}
void CTileManager::SightOnRender(const D3DXVECTOR2& vPos/*유닛의 위치*/ , int irange)
{
	//range는 가급적 홀수

	int fradius = irange/2*32;
	int istartX = int( vPos.x - fradius )/SQ_TILESIZEX;
	int istartY = int( vPos.y - fradius )/SQ_TILESIZEY;

	int startIdx = CMyMath::Pos_to_index(vPos.x , vPos.y);	
	int destidx = 0;

	if(false == GetFogLight(startIdx))
		m_LightOff_List.push_back(startIdx);

	CTileManager::GetInstance()->SetFogSquence(startIdx , 0);
	CTileManager::GetInstance()->SetFogLight(startIdx , 0 , 1);


	if(istartY < 0)
		istartY = 0;
	if(istartY + irange - 1 >= SQ_TILECNTY)
		istartY = SQ_TILECNTY - irange;

	if(istartX < 0)
		istartX = 0;
	if(istartX + irange - 1 >= SQ_TILECNTX)
		istartX = SQ_TILECNTX - irange;


	for(int j = 0; j < irange; ++j) //윗줄 
	{
		destidx = istartY*SQ_TILECNTX + (istartX + j);

		if(istartX + j >= SQ_TILECNTX || istartX + j < 0)
			continue;
		if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , m_LightOff_List);
	}

	for(int j = 0; j < irange; ++j) //아랫줄 
	{
		destidx = (istartY + irange - 1)*SQ_TILECNTX + (istartX + j);

		if(istartX + j >= SQ_TILECNTX || istartX + j< 0)
			continue;
		if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , m_LightOff_List);
	}

	for(int i = 1; i < irange - 1; ++i) //왼쪽 열
	{
		destidx = (istartY + i)*SQ_TILECNTX + istartX;

		if(istartX < 0 || istartX >= SQ_TILECNTX)
			continue;
		if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , m_LightOff_List);
	}

	for(int i = 1; i < irange - 1; ++i) //오른쪽 열
	{
		destidx = (istartY + i)*SQ_TILECNTX + (istartX + irange - 1);

		if(istartX + irange - 1 < 0 || istartX + irange - 1 >= SQ_TILECNTX)
			continue;
		if(destidx < 0 || destidx >= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham_fog(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius , m_LightOff_List);
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
	m_creepTile[idx]->bcreep_install = true;
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
	//m_sqTile.clear();
	m_terrainInfo_List.clear();

	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
		Safe_Delete(m_fogTile[i]);
		Safe_Delete(m_creepTile[i]);
	}
	//m_fogTile.clear();

	//vector<TILE*>().swap(m_sqTile);
	vector<list<TERRAIN_INFO*>>().swap(m_terrainInfo_List);

	m_LightOff_List.clear();
}

bool CTileManager::GetFogLight(const int& idx)
{
	return m_fogTile[idx]->bLight;
}

bool CTileManager::GetCreepInstall(const int& idx)
{
	return m_creepTile[idx]->bcreep_install;
}
