#include "StdAfx.h"
#include "UIRoot.h"

#include "TimeMgr.h"
#include "MyMath.h"
#include "TextureMgr.h"
CUIRoot::CUIRoot(void)
{
	m_vstart = D3DXVECTOR2(0,0);
	m_vend = D3DXVECTOR2(0,0);
}

CUIRoot::~CUIRoot(void)
{
	Release();
}

void CUIRoot::Initialize(void)
{
	D3DXMatrixIdentity(&m_matworld);

	m_vpos = m_vstart;
	m_fspeed = 2500.0f;
	m_init_dt = sqrtf(CMyMath::pos_distance(m_vstart , m_vend));
	

	m_entry_complete = false;
	m_exit_complete = false;

	m_is_entryanim = true;
	m_is_exitanim = false;
}

void CUIRoot::Update(void)
{
	m_matworld._11 = 1;
	m_matworld._22 = 1;
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	//UI_ITER iter = m_uilist.begin();
	//UI_ITER iter_end = m_uilist.end();
	//for( ; iter != iter_end; ++iter)
	//	(*iter)->Update();

	Animation();
}

void CUIRoot::Render(void)
{

	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	if(NULL != m_texinfo)
	{
		m_pSprite->SetTransform(&m_matworld);
		m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , 
			m_color);
	}

	//UI_ITER iter = m_uilist.begin();
	//UI_ITER iter_end = m_uilist.end();
	//for( ; iter != iter_end; ++iter)
	//	(*iter)->Render();


}
void CUIRoot::Set_texturekey(const TCHAR* texkey)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , texkey);
	m_vcenter.x = 0.f;//float(m_texinfo->ImgInfo.Width / 2);
	m_vcenter.y = 0.f;//float(m_texinfo->ImgInfo.Height / 2);
	m_vcenter.z = 0.f;
}
void CUIRoot::Release(void)
{
	UI_ITER iter = m_uilist.begin();
	UI_ITER iter_end = m_uilist.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete((*iter));

	m_uilist.clear();
}

void CUIRoot::SetStartEndPos(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend)
{
	m_vstart = vstart;
	m_vend = vend;
}
void CUIRoot::Animation()
{
	Entry_Animation();
	Exit_Animation();
}

void CUIRoot::Entry_Animation(void)
{
	if(!m_is_entryanim)
		return;

	float onestep = 0;
	float frate =  1;

	D3DXVECTOR2 vdir = m_vend - m_vstart;
	D3DXVec2Normalize(&vdir, &vdir);

	float fdt2 = sqrtf(CMyMath::pos_distance(m_vpos , m_vend));

	frate = fdt2 / m_init_dt;
	if( frate <= 0.01f)
		frate =  0.01f;

	onestep = GETTIME * m_fspeed * frate;
	m_vpos += vdir * onestep;

	if(CMyMath::pos_distance(m_vpos , m_vend) <= onestep*onestep)
	{
		m_vpos = m_vend;
		m_is_entryanim = false;
		m_entry_complete = true;
		//UI들 기능 활성화
	}
	else
	{
		m_entry_complete = false;
		//UI들 이미지는 보여지지만 기능은 비활성화		
	}
}
void CUIRoot::Exit_Animation(void)
{
	if(!m_is_exitanim)
		return;

	float onestep = 0;
	float frate =  1;

	D3DXVECTOR2 vdir = m_vstart - m_vend;
	D3DXVec2Normalize(&vdir, &vdir);

	onestep = GETTIME * m_fspeed / 2 * frate;
	m_vpos += vdir * onestep;

	if(CMyMath::pos_distance(m_vpos , m_vstart) <= onestep*onestep)
	{
		m_vpos = m_vstart;
		m_is_exitanim = false;
		m_exit_complete = true;
	}
	else
	{
		m_exit_complete = false;
	}
}
bool CUIRoot::UI_ptinrect(const D3DXVECTOR2 vpos)
{/*
	UI_ITER iter = m_uilist.begin();
	UI_ITER iter_end = m_uilist.end();

	for( ; iter != iter_end; ++iter)
		if(true == (*iter)->UI_ptinrect(vpos))
		{
			(*iter)->UI_Reaction();
			return false;
		}*/

	return false;
}
void CUIRoot::Init_State(void)
{
	//UI_ITER iter = m_uilist.begin();
	//UI_ITER iter_end = m_uilist.end();

	//for( ; iter != iter_end; ++iter)
	//		(*iter)->Init_State();
}
void CUIRoot::SetEntry(bool bentry)
{
	m_is_entryanim = bentry;
}

void CUIRoot::SetExit(bool bexit)
{
	m_is_exitanim = bexit;
}

bool CUIRoot::GetEntryComplete(void)
{
	return m_entry_complete;
}

bool CUIRoot::GetExitComplete(void)
{
	return m_exit_complete;
}

