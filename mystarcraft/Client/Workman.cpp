#include "StdAfx.h"
#include "Workman.h"

#include "UI_MiniUnitDisplay.h"
#include "ComanderMgr.h"
#include "Mineral.h"

CWorkman::CWorkman(void)
{
	m_select_ui = NULL;
	m_miniunit_display = NULL;

	m_pgas = NULL;
	m_pgas_fragment = NULL;
	m_pmineral = NULL;
	m_pmineral_fragment = NULL;

	m_charge_building = NULL;

	m_is_preview = false;
}

CWorkman::~CWorkman(void)
{
	Release();
}

void CWorkman::Initialize(void)
{
	m_miniunit_display = new CUI_MiniUnitDisplay(m_vPos);
	m_miniunit_display->Initialize();
	CComanderMgr::GetInstance()->SetMiniUnit_display(m_miniunit_display);
}

void CWorkman::Update(void)
{

}

void CWorkman::Render(void)
{

}

void CWorkman::Release(void)
{
	if(NULL != m_miniunit_display)
		m_miniunit_display->SetDestroy(true);

	if(NULL != m_select_ui)
		m_select_ui->SetDestroy(true);

	if( NULL != m_pmineral_fragment)
		m_pmineral_fragment->SetDestroy(true);

	if( NULL != m_pgas_fragment)
		m_pgas_fragment->SetDestroy(true);
}

void CWorkman::Inputkey_reaction(const int& nkey)
{

}

void CWorkman::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CWorkman::Dead(void)
{

}
void CWorkman::SetGas_mark(CObj* pgas)
{
	m_pgas = pgas;
}
void CWorkman::SetMineral_mark(CObj* pmienral)
{
	if(NULL == pmienral && NULL != m_pmineral)
	{
		((CMineral*)m_pmineral)->decrease_workman();
	}
	m_pmineral = pmienral;
}

void CWorkman::setmineral_fragment(CObj* pmienral_frag)
{
	m_pmineral_fragment = pmienral_frag;
}

void CWorkman::setgas_fragment(CObj* pgas_frag)
{

	m_pgas_fragment = pgas_frag;
}

void CWorkman::destroy_frag(void)
{
	if(NULL != m_pmineral_fragment)
	{
		m_pmineral_fragment->SetDestroy(true);
		m_pmineral_fragment = NULL;

		CComanderMgr::GetInstance()->SetMineral(8);
		//여기에 미네랄 자원 ++
	}

	if(NULL != m_pgas_fragment)
	{
		m_pgas_fragment->SetDestroy(true);
		m_pgas_fragment = NULL;

		CComanderMgr::GetInstance()->SetGas(8);
		//여기에 가스 자원 ++
	}
}

void CWorkman::Fragment_Pos_update(void)
{
	if( NULL != m_pmineral_fragment)
	{		
		m_pmineral_fragment->SetPos( m_vPos + m_vcurdir*15);
	}

	if( NULL != m_pgas_fragment)
	{		
		m_pgas_fragment->SetPos( m_vPos + m_vcurdir*15);
	}
}
void CWorkman::Create_Building(void)
{

}

void CWorkman::Create_Building(CObj* pgas_resorce)
{

}

bool CWorkman::ismineral_fragment(void)
{
	if(NULL != m_pmineral_fragment)
		return true;
	else
		return false;
}
