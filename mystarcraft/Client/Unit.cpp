#include "StdAfx.h"
#include "Unit.h"

#include "TextureMgr.h"
#include "TileManager.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "MouseMgr.h"
#include "FontMgr.h"
#include "ScrollMgr.h"

#include "Astar.h"
#include "Area_Mgr.h"

#include "Device.h"

#include "UI_MiniUnitDisplay.h"
#include "ComanderMgr.h"
#include "Mineral.h"
#include "Skill.h"

CUnit::CUnit(void)
{
	m_com_usingskill = NULL;
	m_select_ui = NULL;
	m_miniunit_display = NULL;
	m_upg_info = NULL;
	memset(m_upg_feedback , 0 , sizeof(m_upg_feedback));
	m_upg_info = CComanderMgr::GetInstance()->GetUpginfo();
}

CUnit::~CUnit(void)
{
	Release();

}

void CUnit::Initialize(void)
{
	m_miniunit_display = new CUI_MiniUnitDisplay(m_vPos);
	m_miniunit_display->Initialize();
	CComanderMgr::GetInstance()->SetMiniUnit_display(m_miniunit_display);
}

void CUnit::Update(void)
{

}
void CUnit::Render(void)
{		
}

void CUnit::Release(void)
{
	//Safe_Delete(m_Astar);

	if(NULL != m_miniunit_display)
		m_miniunit_display->SetDestroy(true);

	if(NULL != m_select_ui)
		Safe_Delete(m_select_ui);

	if(NULL != m_energybar_ui)
		Safe_Delete(m_energybar_ui);
}

void CUnit::Dead(void)
{

}

void CUnit::Inputkey_reaction(const int& nkey)
{

}

void CUnit::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}