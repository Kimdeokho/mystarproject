#include "StdAfx.h"
#include "Workman.h"

#include "UI_MiniUnitDisplay.h"
#include "Ingame_UIMgr.h"
#include "Mineral.h"

#include "Building_Preview.h"
CWorkman::CWorkman(void)
{
	m_select_ui = NULL;
	m_miniunit_display = NULL;

	m_pgas = NULL;
	m_pgas_fragment = NULL;
	m_pmineral = NULL;
	m_pmineral_fragment = NULL;

	m_charge_building = NULL;
	m_upg_info = NULL;

	m_is_preview = false;

	m_main_preview = new CBuilding_Preview;

	m_ecmd_state = CMD_BASIC;

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

CWorkman::~CWorkman(void)
{
	CIngame_UIMgr::GetInstance()->ClearPreview();
	Safe_Delete(m_main_preview);
	Release();
}

void CWorkman::Initialize(void)
{
	m_miniunit_display = new CUI_MiniUnitDisplay(m_vPos);
	m_miniunit_display->Initialize();
	CIngame_UIMgr::GetInstance()->SetMiniUnit_display(m_miniunit_display);
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

	if(NULL != m_pmineral)
	{
		((CMineral*)m_pmineral)->decrease_workman(this);
		m_pmineral = NULL;
	}

	if(NULL != m_select_ui)
		Safe_Delete(m_select_ui);

	if(NULL != m_energybar_ui)
		Safe_Delete(m_energybar_ui);
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
	//if(NULL == pmienral && NULL != m_pmineral)
	//{
	//	((CMineral*)m_pmineral)->decrease_workman(this);
	//}
	m_pmineral = pmienral;
}
bool CWorkman::GetMineral_mark(void)
{
	if(NULL == m_pmineral)
		return false;
	else
		return true;
}
void CWorkman::setmineral_fragment(CObj* pmienral_frag)
{
	if(NULL == m_pmineral_fragment)
		m_pmineral_fragment = pmienral_frag;
}

void CWorkman::setgas_fragment(CObj* pgas_frag)
{
	if(NULL == m_pgas_fragment)
		m_pgas_fragment = pgas_frag;
}

void CWorkman::destroy_frag(void)
{
	if(NULL != m_pmineral_fragment)
	{
		m_pmineral_fragment->SetDestroy(true);
		m_pmineral_fragment = NULL;

		CIngame_UIMgr::GetInstance()->SetMineral(8);
		//여기에 미네랄 자원 ++
	}

	if(NULL != m_pgas_fragment)
	{
		m_pgas_fragment->SetDestroy(true);
		m_pgas_fragment = NULL;

		CIngame_UIMgr::GetInstance()->SetGas(8);
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
