#include "StdAfx.h"
#include "ComanderMgr.h"
#include "Device.h"

#include "TextureMgr.h"

#include "UI_MainInterface.h"
#include "Minimap.h"
#include "ScrollMgr.h"
#include "Building_Preview.h"

IMPLEMENT_SINGLETON(CComanderMgr)
CComanderMgr::CComanderMgr(void)
{
	m_total_mineral = 0;
	m_total_gas = 0;
	
}

CComanderMgr::~CComanderMgr(void)
{
	Safe_Delete(m_main_interface);
	Safe_Delete(m_minimap);
	//Safe_Delete(m_preview_building);
	//Safe_Delete(m_sub_preview_building);

}

void CComanderMgr::Initialize(void)
{
	//m_preview_building = NULL;

	m_main_interface = new CUI_MainInterface(L"main_interface");
	m_main_interface->Initialize();

	m_minimap = new CMinimap;
	m_minimap->Initialize();

	//m_preview_building = new CBuilding_Preview;
	//m_preview_building->Initialize();
	//m_sub_preview_building = new CBuilding_Preview;
}

void CComanderMgr::Update(void)
{
	m_minimap->Update();

	//m_preview_building->Update();
	//m_sub_preview_building->Update();
}

void CComanderMgr::Render(void)
{
	if(!m_vec_preview.empty())
	{
		for(size_t i = 0; i < m_vec_preview.size(); ++i)
		{
			m_vec_preview[i]->Render();
		}
		m_vec_preview.clear();
	}

	m_main_interface->Render();

	m_minimap->Render();
}

void CComanderMgr::SetMiniUnit_display(CUI* pui)
{
	((CMinimap*)m_minimap)->Setminiunit(pui);
}


void CComanderMgr::SetMinimapCamPos(const D3DXVECTOR2& vmousepos)
{
	((CMinimap*)m_minimap)->SetMinimapCamPos(vmousepos);
}
void CComanderMgr::Minimappos_to_screen(D3DXVECTOR2& vmousepos)
{
	((CMinimap*)m_minimap)->Minimappos_to_screen(vmousepos);
}
bool	CComanderMgr::intersect_minimap_mousept(const D3DXVECTOR2& vmouse)
{

	if( MyPtInrect(vmouse , &(m_minimap->GetMyRect())) )
		return true;

	return false;
}
void CComanderMgr::SetMineral(const int& amount)
{
	m_total_mineral += amount;
}

void CComanderMgr::SetGas(const int& amount)
{
	m_total_gas += amount;
}

void CComanderMgr::SetPreview(CUI* building_preview)
{
	m_vec_preview.push_back(building_preview);
}

//CUI* CComanderMgr::GetPreview(void)
//{
//	//return m_preview_building;
//}
//
//CUI* CComanderMgr::GetSubPreview(void)
//{
//	//return m_sub_preview_building;
//}
//
//void CComanderMgr::SetPreviewPos(const D3DXVECTOR2& vpos)
//{
//	//((CBuilding_Preview*)m_preview_building)->SetPos(vpos);
//}
//
//void CComanderMgr::SetSubPreviewPos(const D3DXVECTOR2& vpos)
//{
//	//((CBuilding_Preview*)m_sub_preview_building)->SetPos(vpos);
//}
//
//bool CComanderMgr::GetPreview_Active(void)
//{
//	//return ((CBuilding_Preview*)m_preview_building)->GetActive();
//}
