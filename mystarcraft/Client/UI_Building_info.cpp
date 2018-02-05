#include "StdAfx.h"
#include "UI_Building_info.h"

#include "UI_form.h"
#include "TimeMgr.h"
CUI_Building_info::CUI_Building_info( const D3DXVECTOR2& vpos)
{
	m_vpos = vpos;
}

CUI_Building_info::~CUI_Building_info(void)
{
	for(int i = 0; i < 5; ++i)
	{
		Safe_Delete(m_backform[i]);
		Safe_Delete(m_btn_form[i]);
	}
}

void CUI_Building_info::Initialize(void)
{
	m_is_active = false;

	m_backform[0] = new CUI_form;
	((CUI_form*)m_backform[0])->set_texture(L"EDGE" , D3DXVECTOR2(m_vpos.x + 39, m_vpos.y));

	for(int i = 1; i < 5; ++i)
	{
		m_backform[i] = new CUI_form;
		((CUI_form*)m_backform[i])->set_texture(L"EDGE" , D3DXVECTOR2(m_vpos.x + (float)i*39 , m_vpos.y + 39));
	}


	TCHAR temp[32];

	m_vbtn_pos[0] = D3DXVECTOR2(m_vpos.x + 39 , m_vpos.y );
	m_vnumberfont_pos[0] = D3DXVECTOR2(m_vpos.x + 47 , m_vpos.y + 8);

	m_btn_form[0] = new CUI_form;
	((CUI_form*)m_btn_form[0])->set_texture(L"PRODUCTION_NUM1" , m_vnumberfont_pos[0]);

	for(int i = 1; i < 5; ++i)
	{
		m_vbtn_pos[i] = D3DXVECTOR2(m_vpos.x + 39*i , m_vpos.y + 39);
		m_vnumberfont_pos[i] = D3DXVECTOR2(m_vpos.x + 39*i + 8 , m_vpos.y + 8 + 39);

		m_btn_form[i] = new CUI_form;
		wsprintf( temp, L"PRODUCTION_NUM%d" , i+1);
		((CUI_form*)m_btn_form[i])->set_texture(temp , m_vnumberfont_pos[i]);
	}
}

void CUI_Building_info::Update(void)
{
}

void CUI_Building_info::Render(void)
{
	if(false == m_is_active)
		return;

	for(int i = 0; i < 5; ++i)
	{
		m_backform[i]->Render();
		m_btn_form[i]->Render();
	}
}

void CUI_Building_info::Release(void)
{

}

void CUI_Building_info::SetActive(bool bactive)
{
	m_is_active = bactive;
}

void CUI_Building_info::InitNumber(void)
{
	TCHAR temp[32];

	for(int i = 0; i < 5; ++i)
	{
		wsprintf( temp, L"PRODUCTION_NUM%d" , i+1);
		((CUI_form*)m_btn_form[i])->set_texture(temp  , m_vnumberfont_pos[i]);
	}
}
void CUI_Building_info::SetBuilding_info(const int& idx , const TCHAR* texkey)
{
	((CUI_form*)m_btn_form[ idx ])->set_texture(texkey , m_vbtn_pos[idx]);
}
