#include "StdAfx.h"
#include "UI_RoomPopup.h"

#include "TextureMgr.h"
#include "UI_RCreateOK.h"
#include "UI_RCreateC.h"
#include "UI_IME.h"
CUI_RoomPopup::CUI_RoomPopup(void)
{
}

CUI_RoomPopup::~CUI_RoomPopup(void)
{
	Release();
}

void CUI_RoomPopup::Initialize(void)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"POPUP");
	m_vcenter.x = 0.f;//float(m_texinfo->ImgInfo.Width / 2);
	m_vcenter.y = 0.f;//float(m_texinfo->ImgInfo.Height / 2);
	m_vcenter.z = 0.f;

	m_active = false;

	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	m_rect.left = 0;
	m_rect.right = BACKBUFFER_SIZEY;
	m_rect.top = 0;
	m_rect.bottom = BACKBUFFER_SIZEY;

	m_create_ok = new CUI_RCreateOK;
	m_create_ok->SetPos( D3DXVECTOR2(18 , 160 ));
	m_create_ok->SetParentMat(&m_matworld);
	m_create_ok->Initialize();
	m_list.push_back(m_create_ok);


	m_create_cancel = new CUI_RCreateC;
	m_create_cancel->SetPos( D3DXVECTOR2(185 , 160 ));
	m_create_cancel->SetParentMat(&m_matworld);
	m_create_cancel->Initialize();
	m_list.push_back(m_create_cancel);

	m_roomtitle_ime = new CUI_IME(D3DXVECTOR2(20, 76) , D3DXVECTOR2(320 , 25));
	m_roomtitle_ime->SetParentMat(&m_matworld);
	m_roomtitle_ime->Initialize();
	m_list.push_back(m_roomtitle_ime);

	m_create_cancel->SetTitleIME(m_roomtitle_ime);
}

void CUI_RoomPopup::Update(void)
{
	if(!m_active)
		return;

	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for(; iter != iter_end; ++iter)
		(*iter)->Update();
}

void CUI_RoomPopup::Render(void)
{
	if(!m_active)
		return;

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , 
		m_color);

	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for(; iter != iter_end; ++iter)
		(*iter)->Render();
}
bool CUI_RoomPopup::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	if(!m_active)
		return false;


	if(MyPtInrect(vpos , &m_rect))
	{
		UI_ITER iter = m_list.begin();
		UI_ITER iter_end = m_list.end();
		for(; iter != iter_end; ++iter)
		{
			if((*iter)->UI_ptinrect(vpos))
				break;
		}
		return true;
	}

	return false;
}

void CUI_RoomPopup::Init_State(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for(; iter != iter_end; ++iter)
		(*iter)->Init_State();
}

void CUI_RoomPopup::Release(void)
{
	UI_ITER iter = m_list.begin();
	UI_ITER iter_end = m_list.end();
	for(; iter != iter_end; ++iter)
		Safe_Delete((*iter));

	m_list.clear();
}

const TCHAR* CUI_RoomPopup::GetRoomTitle(void)
{
	return m_roomtitle_ime->GetMessage();
}

