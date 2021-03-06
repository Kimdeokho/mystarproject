#include "StdAfx.h"
#include "Building_Preview.h"

#include "TextureMgr.h"
#include "MouseMgr.h"

#include "MyMath.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "ScrollMgr.h"
#include "FontMgr.h"
#include "Session_Mgr.h"
#include "RoomSession_Mgr.h"
#include "Obj.h"
CBuilding_Preview::CBuilding_Preview(CObj* pobj)
{
	m_preview_info.objname = L"";
	m_pobj = pobj;
	m_active = false;
}

CBuilding_Preview::~CBuilding_Preview(void)
{
}
void CBuilding_Preview::SetPreviewInfo(const PREVIEW_INFO& _info)
{
	m_preview_info = _info;
}
void CBuilding_Preview::SetPreviewInfo(const TCHAR* objkey , TERRAN_BUILD_TECH ebuild , const int& icol , const int& irow , MYRECT<float> vtx)
{
	D3DXMatrixIdentity(&m_tempmat);
	m_preview_info.objname = objkey;
	m_preview_info.ebuild = ebuild;
	m_preview_info.icol = icol;
	m_preview_info.irow = irow;
	m_preview_info.vtx = vtx;


	m_Debugtexinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");
	m_preview_tex = CTextureMgr::GetInstance()->GetStateTexture_vecset(m_preview_info.objname , L"IDLE");

	m_vcenter = D3DXVECTOR3(float(((*m_preview_tex)[0])->ImgInfo.Width/2) , float(((*m_preview_tex)[0])->ImgInfo.Height/2 ) , 0);


	if(irow % 2 == 0)
		m_weight.x = float((irow/2) * 32 - 16);
	else
		m_weight.x = float((irow/2) * 32);


	if(icol % 2 == 0)
		m_weight.y = float((icol/2) * 32 - 16);
	else 
		m_weight.y = float((icol/2) * 32);

	m_preview_info.vcenter_pos = m_preview_info.vpos + m_weight;
	m_active = true;
}

void CBuilding_Preview::Initialize(void)
{
	m_Debugtexinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");
}

void CBuilding_Preview::Update(void)
{
	if(!m_active)
		return;

	//m_preview_info.vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
	//m_preview_info.vpos = CMyMath::index_to_Pos( CMyMath::Pos_to_index(m_preview_info.vpos , 32)  , 128 , 32);

	//m_preview_info.vcenter_pos = m_preview_info.vpos + m_weight;

}
void CBuilding_Preview::SetPos(const D3DXVECTOR2& vpos)
{
	m_preview_info.vpos = CMyMath::index_to_Pos( CMyMath::Pos_to_index(vpos , 32)  , 128 , 32);
	m_preview_info.vcenter_pos = m_preview_info.vpos + m_weight;
}
void CBuilding_Preview::Render(void)
{
	if(!m_active)
		return;

	m_matworld._41 = m_preview_info.vpos.x + m_weight.x - CScrollMgr::m_fScrollX;
	m_matworld._42 = m_preview_info.vpos.y + m_weight.y - CScrollMgr::m_fScrollY;

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw( ((*m_preview_tex)[0])->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(255,255,255,255));

	int idx32 = 0;
	int idx64 = 0;
	D3DXVECTOR2 vtemp = m_preview_info.vpos;// 0행0열셀이 가리키는 좌표이다
	MYRECT<float>	temprc;
	D3DCOLOR	tile_color;
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
	TRIBE	etribe = m_pobj->GetUnitinfo().etribe;
	

	for(int i = 0; i < m_preview_info.icol; ++i)
	{
		vtemp.y = m_preview_info.vpos.y + i*32;
		for(int j = 0; j < m_preview_info.irow; ++j)
		{
			vtemp.x = m_preview_info.vpos.x + j*32;
			idx32 = CMyMath::Pos_to_index(vtemp  , 32);
			//idx64 = CMyMath::Pos_to_index(vtemp , 64);

			if( 0 == i)
			{
				//제일 위쪽
				temprc.top = m_preview_info.vcenter_pos.y - m_preview_info.vtx.top;
				temprc.bottom = vtemp.y + 16;
			}
			else if( m_preview_info.icol - 1 == i)
			{
				//제일 아래쪽
				temprc.bottom = m_preview_info.vcenter_pos.y + m_preview_info.vtx.bottom;
				temprc.top = vtemp.y - 16;
			}
			else
			{
				temprc.top = vtemp.y - 16;
				temprc.bottom = vtemp.y + 16;
			}

			if( 0 == j)
			{
				//제일 왼쪽
				temprc.left = m_preview_info.vcenter_pos.x - m_preview_info.vtx.left;
				temprc.right = vtemp.x + 16;
			}
			else if( m_preview_info.irow - 1 == j)
			{
				//제일 오른쪽
				temprc.right = m_preview_info.vcenter_pos.x + m_preview_info.vtx.right;
				temprc.left = vtemp.x - 16;
			}
			else
			{
				temprc.left = vtemp.x - 16;
				temprc.right = vtemp.x + 16;
			}


			m_tempmat._41 = vtemp.x - CScrollMgr::m_fScrollX;
			m_tempmat._42 = vtemp.y - CScrollMgr::m_fScrollY;
			m_pSprite->SetTransform(&m_tempmat);
			BYTE op = CTileManager::GetInstance()->GetTileOption(idx32);
			
			FOGSIGHT_OPTION fog_sight = CTileManager::GetInstance()->GetFogLight(idx32 , eteam);

			if(T_GAS == m_preview_info.ebuild ||
				Z_GAS == m_preview_info.ebuild)
			{
				if(op == RESOURCE_GAS && FOG_BLACK != fog_sight)
					tile_color = D3DCOLOR_ARGB(80, 0 , 255, 0 );
				else
					tile_color = D3DCOLOR_ARGB(80, 255 , 0, 0 );
			}
			else
			{
				if(true == CArea_Mgr::GetInstance()->Building_Collocate_check( m_pobj  , vtemp , temprc) &&
					op == MOVE_OK &&
					FOG_BLACK != fog_sight)
				{
					//tile_color = D3DCOLOR_ARGB(80, 0 , 255, 0 );

					if( TRIBE_TERRAN == etribe )
					{
						if(CTileManager::GetInstance()->GetCreepInstall(idx32))
							tile_color = D3DCOLOR_ARGB(80, 255 , 0, 0 );
						else
							tile_color = D3DCOLOR_ARGB(80, 0 , 255, 0 );
					}
					else if(TRIBE_ZERG == etribe)
					{
						if(Z_HATCHERY == m_preview_info.ebuild)
							tile_color = D3DCOLOR_ARGB(80, 0 , 255, 0 );
						else
						{
							if(CTileManager::GetInstance()->GetCreepInstall(idx32))
								tile_color = D3DCOLOR_ARGB(80, 0 , 255, 0 );
							else
								tile_color = D3DCOLOR_ARGB(80, 255 , 0, 0 );
						}
					}
				}
				else
					tile_color = D3DCOLOR_ARGB(80, 255 , 0, 0 );

				//if(FOG_BLACK == fog_sight)
				//	tile_color = D3DCOLOR_ARGB(80, 255 , 0, 0 );
			}

			m_pSprite->Draw( m_Debugtexinfo->pTexture , NULL , &D3DXVECTOR3(16,16,0)
				, NULL , tile_color);
		}
	}
}

void CBuilding_Preview::Release(void)
{

}

void CBuilding_Preview::SetActive(bool bactive)
{
	m_active = bactive;
}
bool CBuilding_Preview::GetActive(void)
{
	return m_active;
}
bool CBuilding_Preview::Install_check(void)
{
	int idx32 = 0;
	int idx64 = 0;
	D3DXVECTOR2 vtemp = m_preview_info.vpos; // 0행0열셀이 가리키는 좌표이다
	MYRECT<float>	temprc;
	TEAM_NUMBER Player_team = CSession_Mgr::GetInstance()->GetTeamNumber();
	TEAM_NUMBER eteam = m_pobj->GetTeamNumber();
	TRIBE	etribe = m_pobj->GetUnitinfo().etribe;

	for(int i = 0; i < m_preview_info.icol; ++i)
	{
		vtemp.y = m_preview_info.vpos.y + i*32;
		for(int j = 0; j < m_preview_info.irow; ++j)
		{
			vtemp.x = m_preview_info.vpos.x + j*32;
			idx32 = CMyMath::Pos_to_index(vtemp  , 32);
			idx64 = CMyMath::Pos_to_index(vtemp , 64);


			if( 0 == i)
			{
				//제일 위쪽
				temprc.top = m_preview_info.vcenter_pos.y - m_preview_info.vtx.top;
				temprc.bottom = vtemp.y + 16;
			}
			else if( m_preview_info.icol - 1 == i)
			{
				//제일 아래쪽
				temprc.bottom = m_preview_info.vcenter_pos.y + m_preview_info.vtx.bottom;
				temprc.top = vtemp.y - 16;
			}
			else
			{
				temprc.top = vtemp.y - 16;
				temprc.bottom = vtemp.y + 16;
			}

			if( 0 == j)
			{
				//제일 왼쪽
				temprc.left = m_preview_info.vcenter_pos.x - m_preview_info.vtx.left;
				temprc.right = vtemp.x + 16;
			}
			else if( m_preview_info.irow - 1 == j)
			{
				//제일 오른쪽
				temprc.right = m_preview_info.vcenter_pos.x + m_preview_info.vtx.right;
				temprc.left = vtemp.x - 16;
			}
			else
			{
				temprc.left = vtemp.x - 16;
				temprc.right = vtemp.x + 16;
			}

			//TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
			
			BYTE			op = CTileManager::GetInstance()->GetTileOption(idx32);
			FOGSIGHT_OPTION fog_sight = CTileManager::GetInstance()->GetFogLight(idx32 , eteam);

			if(T_GAS == m_preview_info.ebuild ||
				Z_GAS == m_preview_info.ebuild)
			{
				if(RESOURCE_GAS != op || FOG_BLACK == fog_sight)
				{
					if(Player_team == eteam)
					{
						CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
							, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
					}
					return false;
				}
			}
			else
			{
				if(true == CArea_Mgr::GetInstance()->Building_Collocate_check( m_pobj  , vtemp , temprc) &&
					MOVE_OK == op &&
					FOG_BLACK != fog_sight)
				{
					if(TRIBE_TERRAN == etribe)
					{
						if(CTileManager::GetInstance()->GetCreepInstall(idx32) &&
							Player_team == eteam)
						{
							CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
							, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
							return false;
						}
					}
					else if(TRIBE_ZERG == etribe)
					{
						if(!CTileManager::GetInstance()->GetCreepInstall(idx32) &&
							Player_team == eteam)
						{
							if(Z_HATCHERY != m_preview_info.ebuild)
							{
								CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
									, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
								return false;
							}
						}
					}
				}
				else
				{
					if(Player_team == eteam)
					{
						CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
							, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
					}
					return false;
				}
			}
		}
	}

	return true;
}
bool CBuilding_Preview::Install_check(const PREVIEW_INFO& cur_info)
{
	int idx32 = 0;
	int idx64 = 0;
	D3DXVECTOR2 vtemp = cur_info.vpos;
	MYRECT<float>	temprc;
	TEAM_NUMBER eteam = m_pobj->GetTeamNumber();
	TEAM_NUMBER Player_team = CSession_Mgr::GetInstance()->GetTeamNumber();
	TRIBE	etribe = m_pobj->GetUnitinfo().etribe;

	temprc.left		= cur_info.vcenter_pos.x - cur_info.vtx.left;
	temprc.right	= cur_info.vcenter_pos.x + cur_info.vtx.right;
	temprc.top		= cur_info.vcenter_pos.y - cur_info.vtx.top;
	temprc.bottom	= cur_info.vcenter_pos.y + cur_info.vtx.bottom;

	for(int i = 0; i < cur_info.icol; ++i)
	{
		vtemp.y = cur_info.vpos.y + i*32;
		for(int j = 0; j < cur_info.irow; ++j)
		{
			vtemp.x = cur_info.vpos.x + j*32;
			idx32 = CMyMath::Pos_to_index(vtemp  , 32);


			BYTE			op = CTileManager::GetInstance()->GetTileOption(idx32);
			FOGSIGHT_OPTION fog_sight = CTileManager::GetInstance()->GetFogLight(idx32 , eteam);

			if(T_GAS == cur_info.ebuild ||
				Z_GAS == cur_info.ebuild)
			{
				if(RESOURCE_GAS != op || FOG_BLACK == fog_sight)
				{
					if(Player_team == eteam)
					{
						CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
							, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
					}
					return false;
				}
			}
			else
			{
				if(true == CArea_Mgr::GetInstance()->Building_Collocate_check( m_pobj  , vtemp , temprc) &&
					MOVE_OK == op /*&&
					FOG_BLACK != fog_sight*/)
				{
					if(TRIBE_TERRAN == etribe)
					{
						if(CTileManager::GetInstance()->GetCreepInstall(idx32) &&
							Player_team == eteam)
						{
							CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
								, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
							return false;
						}
					}
					else if(TRIBE_ZERG == etribe)
					{
						if(!CTileManager::GetInstance()->GetCreepInstall(idx32) &&
							Player_team == eteam)
						{
							if(Z_HATCHERY != m_preview_info.ebuild)
							{
								CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
									, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
								return false;
							}
						}
					}
				}
				else
				{
					if(Player_team == eteam)
					{
						CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
							, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f , 3.f);
					}
					return false;
				}
			}
		}
	}

	return true;
}
const PREVIEW_INFO CBuilding_Preview::GetPreviewInfo(void)
{
	return m_preview_info;
}
