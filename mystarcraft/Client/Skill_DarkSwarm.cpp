#include "StdAfx.h"
#include "Skill_DarkSwarm.h"

#include "Device.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "FontMgr.h"

CSkill_DarkSwarm::CSkill_DarkSwarm(const D3DXVECTOR2 vpos)
{
	m_vPos = vpos;
}

CSkill_DarkSwarm::~CSkill_DarkSwarm(void)
{
	Release();
}

void CSkill_DarkSwarm::Initialize(void)
{
	m_sortID = SORT_GROUND_EFF;
	int irange = 32*5 / 2;
	
	D3DXVECTOR2 vstartpos = D3DXVECTOR2(m_vPos.x - irange , m_vPos.y - irange);
	D3DXVECTOR2 vendpos = D3DXVECTOR2(m_vPos.x + irange , m_vPos.y + irange);

	if(vstartpos.x < 0)
		vstartpos.x = 0;
	if(vstartpos.y < 0)
		vstartpos.y = 0;

	if(vendpos.x > SQ_TILECNTX*SQ_TILESIZEX)
		vendpos.x = SQ_TILECNTX*SQ_TILESIZEX - 1;
	if(vendpos.y > SQ_TILECNTY*SQ_TILESIZEY)
		vendpos.y = SQ_TILECNTY*SQ_TILESIZEY - 1;

	int row = 5;
	int col = 5;

	int startidx = CMyMath::Pos_to_index(vstartpos , 32);
	int idx = 0;

	for(int i = 0; i < col; ++i)
	{
		for(int j = 0; j < row; ++j)
		{
			idx = startidx + (i*SQ_TILECNTX) + j;
			m_darkswarm_cnt[idx] += 1;

			//m_idxfont.push_back(idx);
		}
	}

	m_dwtime = 0.f;


	m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"DARK_SWARM");

	m_frame.fcurframe = 0;
	m_frame.umax = m_generatex->size() - 1;
	m_frame.fframespeed = float(m_frame.umax)*2.5f;

	m_curtex = ((*m_generatex)[0]);

	m_curidx32 = CMyMath::Pos_to_index(m_vPos , 32);
}

void CSkill_DarkSwarm::Update(void)
{
	m_dwtime += GETTIME;

	if(m_dwtime > 30.f)
	{
		m_dwtime = 0.f;
		m_bdestroy = true;
	}

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe > m_frame.umax )
	{
		m_frame.fcurframe = 5.f;
	}

	if(m_frame.fcurframe > 4)
		m_frame.fframespeed = float(m_frame.umax);


	m_curtex = ((*m_generatex)[ (int)m_frame.fcurframe ]);
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	//for(size_t i = 0; i < m_idxfont.size(); ++i)
	//{
	//	D3DXVECTOR2 vpos = CMyMath::index_to_Pos(m_idxfont[i] , 128 ,32);

	//	CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_darkswarm_cnt[m_idxfont[i]] , vpos.x , vpos.y);
	//}
}

void CSkill_DarkSwarm::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , 
		NULL , D3DCOLOR_ARGB(140,255,255,255));
}

void CSkill_DarkSwarm::Release(void)
{
	int irange = 32*5 / 2;

	D3DXVECTOR2 vstartpos = D3DXVECTOR2(m_vPos.x - irange , m_vPos.y - irange);
	D3DXVECTOR2 vendpos = D3DXVECTOR2(m_vPos.x + irange , m_vPos.y + irange);

	if(vstartpos.x < 0)
		vstartpos.x = 0;
	if(vstartpos.y < 0)
		vstartpos.y = 0;

	if(vendpos.x > SQ_TILECNTX*SQ_TILESIZEX)
		vendpos.x = SQ_TILECNTX*SQ_TILESIZEX - 1;
	if(vendpos.y > SQ_TILECNTY*SQ_TILESIZEY)
		vendpos.y = SQ_TILECNTY*SQ_TILESIZEY - 1;

	int row = 5;
	int col = 5;

	int startidx = CMyMath::Pos_to_index(vstartpos , 32);
	int idx = 0;

	for(int i = 0; i < col; ++i)
	{
		for(int j = 0; j < row; ++j)
		{
			idx = startidx + (i*SQ_TILECNTX) + j;
			m_darkswarm_cnt[idx] -= 1;
		}
	}
}

int CSkill_DarkSwarm::m_darkswarm_cnt[] = {};