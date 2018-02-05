#include "StdAfx.h"
#include "Building_Preview.h"

#include "TextureMgr.h"
#include "MouseMgr.h"

#include "MyMath.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "ScrollMgr.h"
#include "FontMgr.h"
CBuilding_Preview::CBuilding_Preview(void)
{
	m_preview_info.objname = L"";
	m_pobj = NULL;
	m_active = false;
}

CBuilding_Preview::~CBuilding_Preview(void)
{
}

void CBuilding_Preview::SetPreviewInfo(const TCHAR* objkey , TERRAN_BUILD_TECH ebuild , const int& icol , const int& irow , CObj* pobj)
{
	D3DXMatrixIdentity(&m_tempmat);
	m_preview_info.objname = objkey;
	m_preview_info.ebuild = ebuild;
	m_preview_info.icol = icol;
	m_preview_info.irow = irow;
	m_pobj = pobj;

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
	D3DXVECTOR2 vtemp = m_preview_info.vpos;//m_vpos는 애초에 화면좌표
	MYRECT<float>	temprc;
	D3DCOLOR	tile_color;

	for(int i = 0; i < m_preview_info.icol; ++i)
	{
		vtemp.y = m_preview_info.vpos.y + i*32;
		for(int j = 0; j < m_preview_info.irow; ++j)
		{
			vtemp.x = m_preview_info.vpos.x + j*32;
			idx32 = CMyMath::Pos_to_index(vtemp  , 32);
			idx64 = CMyMath::Pos_to_index(vtemp , 64);

			temprc.left = vtemp.x - 16;
			temprc.right = vtemp.x + 16;
			temprc.top = vtemp.y - 16;
			temprc.bottom = vtemp.y + 16;

			m_tempmat._41 = vtemp.x - CScrollMgr::m_fScrollX;
			m_tempmat._42 = vtemp.y - CScrollMgr::m_fScrollY;
			m_pSprite->SetTransform(&m_tempmat);
			BYTE op = CTileManager::GetInstance()->GetTileOption(idx32);
			FOGSIGHT_OPTION fog_sight = CTileManager::GetInstance()->GetFogSightOp(idx32);

			if(T_GAS == m_preview_info.ebuild )
			{
				if(op == RESOURCE_GAS && FOG_BLACK != fog_sight)
				{
					tile_color = D3DCOLOR_ARGB(80, 0 , 255, 0 );
				}
				else
				{
					tile_color = D3DCOLOR_ARGB(80, 255 , 0, 0 );
				}
			}
			else
			{
				if(true == CArea_Mgr::GetInstance()->Building_Collocate_check( m_pobj  , idx64 , temprc) &&
					op == MOVE_OK &&
					FOG_BLACK != fog_sight)
				{
					tile_color = D3DCOLOR_ARGB(80, 0 , 255, 0 );
				}
				else
				{
					tile_color = D3DCOLOR_ARGB(80, 255 , 0, 0 );
				}
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
	D3DXVECTOR2 vtemp = m_preview_info.vpos;
	MYRECT<float>	temprc;
	for(int i = 0; i < m_preview_info.icol; ++i)
	{
		vtemp.y = m_preview_info.vpos.y + i*32;
		for(int j = 0; j < m_preview_info.irow; ++j)
		{
			vtemp.x = m_preview_info.vpos.x + j*32;
			idx32 = CMyMath::Pos_to_index(vtemp  , 32);
			idx64 = CMyMath::Pos_to_index(vtemp , 64);

			temprc.left = vtemp.x - 16;
			temprc.right = vtemp.x + 16;
			temprc.top = vtemp.y - 16;
			temprc.bottom = vtemp.y + 16;

			BYTE op = CTileManager::GetInstance()->GetTileOption(idx32);
			FOGSIGHT_OPTION fog_sight = CTileManager::GetInstance()->GetFogSightOp(idx32);

			if(T_GAS == m_preview_info.ebuild )
			{
				if(RESOURCE_GAS != op || FOG_BLACK == fog_sight)
				{
					CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
						, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f);
					return false;
				}
			}
			else
			{
				if(true == CArea_Mgr::GetInstance()->Building_Collocate_check( m_pobj  , idx64 , temprc) &&
					MOVE_OK == op &&
					FOG_BLACK != fog_sight)
				{
				}
				else
				{
					CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
						, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f);
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
	for(int i = 0; i < cur_info.icol; ++i)
	{
		vtemp.y = cur_info.vpos.y + i*32;
		for(int j = 0; j < cur_info.irow; ++j)
		{
			vtemp.x = cur_info.vpos.x + j*32;
			idx32 = CMyMath::Pos_to_index(vtemp  , 32);
			idx64 = CMyMath::Pos_to_index(vtemp , 64);

			temprc.left = vtemp.x - 16;
			temprc.right = vtemp.x + 16;
			temprc.top = vtemp.y - 16;
			temprc.bottom = vtemp.y + 16;

			BYTE op = CTileManager::GetInstance()->GetTileOption(idx32);
			FOGSIGHT_OPTION fog_sight = CTileManager::GetInstance()->GetFogSightOp(idx32);

			if(T_GAS == cur_info.ebuild )
			{
				if(RESOURCE_GAS != op || FOG_BLACK == fog_sight)
				{
					CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
						, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f);
					return false;
				}
			}
			else
			{
				if(true == CArea_Mgr::GetInstance()->Building_Collocate_check( m_pobj  , idx64 , temprc) &&
					MOVE_OK == op &&
					FOG_BLACK != fog_sight)
				{
				}
				else
				{
					CFontMgr::GetInstance()->SetNoticeFont(L"그곳엔 설치 할 수 없습니다."
						, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY/1.5f);
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
