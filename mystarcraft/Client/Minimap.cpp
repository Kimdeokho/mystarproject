#include "StdAfx.h"
#include "Minimap.h"

#include "TileManager.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
CMinimap::CMinimap(void)
{
}

CMinimap::~CMinimap(void)
{
	list<CUI*>::iterator iter		= m_miniunit_display.begin();
	list<CUI*>::iterator iter_end	= m_miniunit_display.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(*iter);

	m_miniunit_display.clear();
}

void CMinimap::Initialize(void)
{
	m_MinimapTexture = CTileManager::GetInstance()->GetMiniampTexture();
	m_MinifogTexture = CTileManager::GetInstance()->GetMiniFogmapTexture();
	
	m_matworld._41 = 85;
	m_matworld._42 = 470;

	m_rect.left = m_matworld._41;
	m_rect.top = m_matworld._42;
	m_rect.right = m_matworld._41 + 128;
	m_rect.bottom = m_matworld._42 + 128;
}

void CMinimap::Update(void)
{
	list<CUI*>::iterator iter		= m_miniunit_display.begin();
	list<CUI*>::iterator iter_end	= m_miniunit_display.end();

	for( ; iter != iter_end; )
	{
		if( true == (*iter)->GetDestroy() )
		{
			Safe_Delete((*iter));
			iter = m_miniunit_display.erase(iter);
		}
		else
		{
			(*iter)->Update();
			++iter;
		}
	}
}

void CMinimap::Render(void)
{

	m_matworld._11 = 1.f;
	m_matworld._22 = 1.f;

	m_matworld._41 = 85;//85 , 5;
	m_matworld._42 = 470;//470 , 350;

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_MinimapTexture , NULL , &D3DXVECTOR3(0,0,0), NULL
		, D3DCOLOR_ARGB(255,255,255,255));

		

	//D3DXMatrixIdentity(&m_matworld);
	//m_pSprite->SetTransform(&m_matworld);

	//m_pSprite->Draw(m_MinifogTexture , NULL , &D3DXVECTOR3(0,0,0), NULL
	//	, D3DCOLOR_ARGB(255,255,255,255));


	CTileManager::GetInstance()->MinifogUpdate();

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_MinifogTexture , NULL , &D3DXVECTOR3(0,0,0), NULL
		, D3DCOLOR_ARGB(255,255,255,255));





	list<CUI*>::iterator iter		= m_miniunit_display.begin();
	list<CUI*>::iterator iter_end	= m_miniunit_display.end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	MYRECT<float> cam_rect;
	cam_rect.left = CScrollMgr::m_fScrollX * 0.03125f + m_matworld._41;
	cam_rect.top = CScrollMgr::m_fScrollY * 0.03125f + m_matworld._42;
	cam_rect.right = (CScrollMgr::m_fScrollX + BACKBUFFER_SIZEX) * 0.03125f + m_matworld._41;
	cam_rect.bottom = (CScrollMgr::m_fScrollY + BACKBUFFER_SIZEY) * 0.03125f + m_matworld._42;
	CLineMgr::GetInstance()->minicambox_render(cam_rect);

	CLineMgr::GetInstance()->minicambox_render(m_rect);
}

void CMinimap::Release(void)
{

}

void CMinimap::Setminiunit(CUI* pui)
{
	m_miniunit_display.push_back(pui);
}
