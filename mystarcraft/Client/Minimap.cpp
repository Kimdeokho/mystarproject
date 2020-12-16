#include "StdAfx.h"
#include "Minimap.h"

#include "TileManager.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "Session_Mgr.h"
#include "MyMath.h"
#include "Debug_Mgr.h"

#include "Obj.h"
#include "UI_MiniUnitDisplay.h"
CMinimap::CMinimap(void)
{
}

CMinimap::~CMinimap(void)
{
	list<CUI_MiniUnitDisplay*>::iterator iter		= m_miniunit_display.begin();
	list<CUI_MiniUnitDisplay*>::iterator iter_end	= m_miniunit_display.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(*iter);

	m_miniunit_display.clear();
}

void CMinimap::Initialize(void)
{
	m_MinimapTexture = CTileManager::GetInstance()->GetMiniampTexture();
	m_MinifogTexture = CTileManager::GetInstance()->GetMiniFogmapTexture();
	
	m_matworld._41 = m_vpos.x;  
	m_matworld._42 = m_vpos.y; 

	m_rect.left = m_matworld._41;
	m_rect.top = m_matworld._42;
	m_rect.right = m_matworld._41 + 128;
	m_rect.bottom = m_matworld._42 + 128;

	m_fupdatetime = 0.f;
	m_bactive = true;
}

void CMinimap::Update(void)
{
	m_fupdatetime += GETTIME;

	list<CUI_MiniUnitDisplay*>::iterator iter		= m_miniunit_display.begin();
	list<CUI_MiniUnitDisplay*>::iterator iter_end	= m_miniunit_display.end();

	for( ; iter != iter_end; )
	{
		if( true == (*iter)->GetDestroy() )
		{
			Safe_Delete((*iter));
			iter = m_miniunit_display.erase(iter);
		}
		else
			++iter;
	}

	if(m_fupdatetime >= 0.5f)
	{
		iter		= m_miniunit_display.begin();
		iter_end	= m_miniunit_display.end();

		m_fupdatetime = 0.f;

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Update();
		}

		CTileManager::GetInstance()->MinifogUpdate();
	}

}

void CMinimap::Render(void)
{


	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_MinimapTexture , NULL , &D3DXVECTOR3(0,0,0), NULL
		, D3DCOLOR_ARGB(255,255,255,255));

		
	//CTileManager::GetInstance()->MinifogUpdate();

	if(!CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
	{
		m_pSprite->SetTransform(&m_matworld);
		m_pSprite->Draw(m_MinifogTexture , NULL , &D3DXVECTOR3(0,0,0), NULL
			, D3DCOLOR_ARGB(255,255,255,255));
	}

	list<CUI_MiniUnitDisplay*>::iterator iter		= m_miniunit_display.begin();
	list<CUI_MiniUnitDisplay*>::iterator iter_end	= m_miniunit_display.end();

	int fogidx = 0;
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
	D3DXVECTOR2 vpos;
	for( ; iter != iter_end; )
	{
		if( true == (*iter)->GetDestroy() )
		{
			Safe_Delete((*iter));
			iter = m_miniunit_display.erase(iter);
		}
		else
		{
			vpos = (*iter)->GetPos();
			fogidx = CMyMath::Pos_to_index(vpos , 32);

			if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
				(*iter)->Render();
			else
			{
				if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight(fogidx , eteam))
					(*iter)->Render();
				else if(FOG_GREY == CTileManager::GetInstance()->GetFogLight(fogidx , eteam))
				{
					if(CATEGORY_BUILDING == (*iter)->GetCategory())
						(*iter)->Render();
				}

			}

			++iter;
		}
	}

	MYRECT<float> cam_rect;
	cam_rect.left = CScrollMgr::m_fScrollX * 0.03125f + m_matworld._41;
	cam_rect.top = CScrollMgr::m_fScrollY * 0.03125f + m_matworld._42;
	cam_rect.right = (CScrollMgr::m_fScrollX + BACKBUFFER_SIZEX) * 0.03125f + m_matworld._41;
	cam_rect.bottom = (CScrollMgr::m_fScrollY + BACKBUFFER_SIZEY) * 0.03125f + m_matworld._42;
	CLineMgr::GetInstance()->minicambox_render(cam_rect);

}

void CMinimap::Setminiunit(CUI_MiniUnitDisplay* pui)
{
	m_miniunit_display.push_back(pui);
}

void CMinimap::SetMinimapCamPos(const D3DXVECTOR2& vmousepos)
{
	D3DXVECTOR2 vpt = vmousepos;
	if(true == MyPtInrect(vmousepos , &m_rect))
	{
		vpt.x -= m_vpos.x;
		vpt.y -= m_vpos.y;

		vpt.x *= 32;
		vpt.y *= 32;

		vpt.x = vpt.x - BACKBUFFER_SIZEX/2;
		vpt.y = vpt.y - BACKBUFFER_SIZEY/2;

		if(0 > vpt.x)
			vpt.x = 0;
		else if(4096 - BACKBUFFER_SIZEX <= vpt.x)
			vpt.x = 4096 - BACKBUFFER_SIZEX;

		if(0 > vpt.y)
			vpt.y = 0;
		else if(4096 - BACKBUFFER_SIZEY <= vpt.y)
			vpt.y = 4096 - BACKBUFFER_SIZEY;

		CScrollMgr::m_fScrollX = vpt.x;
		CScrollMgr::m_fScrollY = vpt.y;
	}
}
void CMinimap::Minimappos_to_screen(D3DXVECTOR2& vmousept)
{

	//if(true == MyPtInrect(vpt , &m_rect))
	{

		vmousept.x -= m_vpos.x; //vweight ¹Ì´Ï¸Ê À§Ä¡
		vmousept.y -= m_vpos.y;

		vmousept.x *= 32;
		vmousept.y *= 32;
	}
	
}

void CMinimap::Release(void)
{

}
