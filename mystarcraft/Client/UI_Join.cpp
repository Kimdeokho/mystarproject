#include "StdAfx.h"
#include "UI_Join.h"

#include "Device.h"
#include "FontMgr.h"
#include "TextureMgr.h"
#include "MouseMgr.h"
#include "Loby_UIMgr.h"
#include "Session_Mgr.h"
CUI_Join::CUI_Join(const D3DXVECTOR2&	vpos)
{
	m_vpos = vpos;
}

CUI_Join::~CUI_Join(void)
{
	Release();
}

void CUI_Join::Initialize(void)
{
	D3DXMatrixIdentity(&m_matfont);
	lstrcpy(m_text , L"O K");

	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"OK");
	m_vcenter.x = float(m_texinfo->ImgInfo.Width);
	m_vcenter.y = float(m_texinfo->ImgInfo.Height);
	m_vcenter.z = 0.f;
}

void CUI_Join::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	m_matfont._41 = m_matworld._41 + 80;
	m_matfont._42 = m_matworld._42 + 50;

	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + 160;
	m_rect.top = m_matworld._42 + 44;
	m_rect.bottom = m_matworld._42 + 70;

	if(MyPtInrect(CMouseMgr::GetInstance()->GetScreenMousePt() , &m_rect))
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matfont._41, m_matfont._42
			,D3DCOLOR_ARGB(255,0,255,0));
	}
	else
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matfont._41, m_matfont._42
			,D3DCOLOR_ARGB(255,85,185,23));
	}
}

void CUI_Join::Render(void)
{

	if(NULL != m_texinfo)
	{
		m_pSprite->SetTransform(&m_matworld);
		m_pSprite->Draw(m_texinfo->pTexture , NULL , &D3DXVECTOR3(0,0,0) , NULL , 
			m_color);
	}
}

bool CUI_Join::UI_ptinrect(const D3DXVECTOR2 vpos)
{

	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + 160;
	m_rect.top = m_matworld._42 + 44;
	m_rect.bottom = m_matworld._42 + 70;

	if(MyPtInrect(vpos , &m_rect))
	{
		UI_Reaction();
		return true;
	}

	return false;
}
void CUI_Join::UI_Reaction(void)
{
	//¹æ ÀÔÀå
	if(CLoby_UIMgr::GetInstance()->GetRoomIdx() >= 0)
	{
		CLoby_UIMgr::GetInstance()->Exit(LB_ROOM_JOIN);
		CLoby_UIMgr::GetInstance()->Room_Popup(false);
	}
}

void CUI_Join::Release(void)
{

}
