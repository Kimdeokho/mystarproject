#include "StdAfx.h"
#include "UI_RoomInfo.h"

#include "TextureMgr.h"
#include "FontMgr.h"
CUI_RoomInfo::CUI_RoomInfo(void)
{
}

CUI_RoomInfo::~CUI_RoomInfo(void)
{
	Release();
}

void CUI_RoomInfo::Initialize(void)
{
	lstrcpy(m_szTitle , L"");
}

void CUI_RoomInfo::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	float startposY = m_matworld._42 + 120;
	CFontMgr::GetInstance()->Setbatch_Font(L"Game name" , 
		m_matworld._41 + 107 , startposY , D3DCOLOR_ARGB(255 , 98 , 106 , 143));

	CFontMgr::GetInstance()->Setbatch_Font(m_szTitle , 
		m_matworld._41 + 107 , startposY + 18 , D3DCOLOR_ARGB(255 , 255 , 255 , 255));


	CFontMgr::GetInstance()->Setbatch_Font(L"Game Type" , 
		m_matworld._41 + 107 , startposY + 44 , D3DCOLOR_ARGB(255 , 98 , 106 , 143));

	CFontMgr::GetInstance()->Setbatch_Font(L"Melee" , 
		m_matworld._41 + 107 , startposY + 62 , D3DCOLOR_ARGB(255 , 255 , 255 , 255));

	CFontMgr::GetInstance()->Setbatch_Font(L"Size : 128 X 128" , 
		m_matworld._41 + 120 , startposY + 100 , D3DCOLOR_ARGB(255 , 255 , 255 , 255));
}

void CUI_RoomInfo::Render(void)
{

}

bool CUI_RoomInfo::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	return false;
}

void CUI_RoomInfo::UI_Reaction(void)
{

}

void CUI_RoomInfo::Release(void)
{

}

void CUI_RoomInfo::SetTitle(const TCHAR* sztitle)
{
	lstrcpy(m_szTitle , sztitle);
}
