#include "StdAfx.h"
#include "ComanderMgr.h"
#include "Device.h"

#include "TextureMgr.h"

#include "UI_MainInterface.h"
#include "Minimap.h"
#include "ScrollMgr.h"

IMPLEMENT_SINGLETON(CComanderMgr)
CComanderMgr::CComanderMgr(void)
{
}

CComanderMgr::~CComanderMgr(void)
{
	Safe_Delete(m_main_interface);
	Safe_Delete(m_minimap);
}

void CComanderMgr::Initialize(void)
{
	m_main_interface = new CUI_MainInterface(L"main_interface");
	m_main_interface->Initialize();

	m_minimap = new CMinimap;
	m_minimap->Initialize();
}

void CComanderMgr::Update(void)
{
	m_minimap->Update();
}

void CComanderMgr::Render(void)
{
	m_main_interface->Render();

	m_minimap->Render();
}

void CComanderMgr::SetMiniUnit_display(CUI* pui)
{
	((CMinimap*)m_minimap)->Setminiunit(pui);
}


void CComanderMgr::SetMinimapCamPos(const D3DXVECTOR2& vmousepos)
{
	//MyPtInrect(vmousepos , m_minimap->GetMyRect())
}
