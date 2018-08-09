#include "StdAfx.h"
#include "UI_Combobox.h"

#include "TextureMgr.h"
#include "FontMgr.h"
#include "RoomSession_Mgr.h"
#include "Device.h"
#include "Session_Mgr.h"
CUI_Combobox::CUI_Combobox(void)
{
	m_combolist.reserve(10);
	lstrcpy(m_represent , L"");
	m_slotidx = -1;
}

CUI_Combobox::CUI_Combobox(const int& idx)
{
	m_slotidx = idx;
}

CUI_Combobox::~CUI_Combobox(void)
{
	Release();
}

void CUI_Combobox::Initialize(void)
{
	m_texcombo_slot = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" ,L"COMBO_SLOT");
	m_texcombo_middle = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" ,L"COMBO_MIDDLE");
	m_texcombo_end = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" ,L"COMBO_END");
	m_texcombo_arrow = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" ,L"ARROW");

	m_active_combolist = false;

	m_texwidth = m_texcombo_slot->ImgInfo.Width;
	m_texheight = m_texcombo_slot->ImgInfo.Height;


}

void CUI_Combobox::Update(void)
{
	if(false == m_active)
		return;

	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
	if( NULL != m_parentmat)
		m_matworld *= *m_parentmat;


	//CFontMgr::GetInstance()->Setbatch_Font(m_represent , m_matworld._41 , 
	//	m_matworld._42 , D3DCOLOR_ARGB(255,0,255,0) , true);

	//for(DWORD i = 0; i < m_combolist.size(); ++i)
	//{
	//	m_rect.left = m_matworld._41;
	//	m_rect.right = m_matworld._41 + m_texwidth;
	//	m_rect.top = m_matworld._42 + float(i) * m_texheight;
	//	m_rect.bottom = m_matworld._42 + float(i) * m_texheight + m_texheight;

	//	if(false == m_active_combolist)
	//		return;

	//	CFontMgr::GetInstance()->Setbatch_Font(m_combolist[i] , m_matworld._41 , 
	//		m_matworld._42 + float(i+1)*m_texheight, D3DCOLOR_ARGB(255,0,255,0) , true);
	//}


}

void CUI_Combobox::Render(void)
{
	if(false == m_active)
		return;

	D3DXMATRIX tempmat , arrowmat;
	D3DXMatrixIdentity(&tempmat);
	D3DXMatrixIdentity(&arrowmat);
	tempmat = m_matworld;
	arrowmat = m_matworld;


	m_pSprite->SetTransform(&tempmat);
	m_pSprite->Draw(m_texcombo_slot->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
		m_color);

	if(m_slotidx >= 0 &&
		m_slotidx == CRoomSession_Mgr::GetInstance()->GetMyRoomslot())
	{
		arrowmat._41 += 95;
		m_pSprite->SetTransform(&arrowmat);
		m_pSprite->Draw(m_texcombo_arrow->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
			m_color);
	}

	RECT rc = {0};
	m_pSprite->SetTransform(&tempmat);
	CDevice::GetInstance()->GetFont()->DrawTextW(m_pSprite, m_represent , lstrlen(m_represent) , 
		&rc , DT_NOCLIP , D3DCOLOR_ARGB(255,0,255,0) );

	if(false == m_active_combolist)
		return;

	for(DWORD i = 0; i < m_combolist.size(); ++i)
	{
		if( m_combolist.size() - 1 == i)
		{
			tempmat._42 += m_texheight;
			m_pSprite->SetTransform(&tempmat);
			m_pSprite->Draw(m_texcombo_end->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
				m_color);

		}
		else
		{
			tempmat._42 = m_matworld._42 + float(i+1)*m_texheight;
			m_pSprite->SetTransform(&tempmat);
			m_pSprite->Draw(m_texcombo_middle->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
				m_color);
		}
		
		m_pSprite->SetTransform(&tempmat);
		CDevice::GetInstance()->GetFont()->DrawTextW(m_pSprite, m_combolist[i] , lstrlen(m_combolist[i]) , 
			&rc , DT_NOCLIP , D3DCOLOR_ARGB(255,0,255,0) );
	}
}

bool CUI_Combobox::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	if(m_slotidx >= 0 &&
		m_slotidx != CRoomSession_Mgr::GetInstance()->GetMyRoomslot())
		return false;

	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + m_texwidth;
	m_rect.top = m_matworld._42;
	m_rect.bottom = m_matworld._42 + m_texheight;

	if(MyPtInrect(vpos, &m_rect))
	{
		//¹Ú½º ÆîÄ¡±â
		if(false == m_active_combolist)
			m_active_combolist = true;
		else
			m_active_combolist = false;

		return true;
	}

	if(true == m_active_combolist)
	{
		for(DWORD i = 0; i < m_combolist.size(); ++i)
		{
			m_rect.top = m_matworld._42 + float(i+1) * m_texheight;
			m_rect.bottom = m_matworld._42 + float(i+1) * m_texheight + m_texheight;

			if(MyPtInrect(vpos, &m_rect))
			{
				lstrcpy( m_represent , m_combolist[i]);
				m_active_combolist = false;

				USHORT slotidx = CRoomSession_Mgr::GetInstance()->GetMyRoomslot();
				WRITE_TCP_PACKET(PT_ROOM_TRIBE_CHANGE , WriteBuffer , 
					WRITE_PT_ROOM_TRIBE_CHANGE(WriteBuffer , m_represent,slotidx));
				return true;
			}
		}
	}

	return false;
}
void CUI_Combobox::AddData(const TCHAR* _data)
{
	TCHAR* tempsz = new TCHAR[32];
	lstrcpy(tempsz , _data);
	m_combolist.push_back(tempsz);
	
	if(1 == m_combolist.size())
		lstrcpy(m_represent , tempsz);
}

void CUI_Combobox::Init_State(void)
{
}
void CUI_Combobox::UI_Reaction(void)
{
	//ÄÞº¸ ¸®½ºÆ®¸¦ ÆîÄ£´Ù
}

void CUI_Combobox::Release(void)
{
	for(DWORD i = 0; i < m_combolist.size(); ++i)
	{
		Safe_Delete(m_combolist[i]);
	}
	m_combolist.clear();
	vector<TCHAR*> temp;
	temp.swap(m_combolist);
}

void CUI_Combobox::SetRepresentData(const WCHAR* _data)
{
	lstrcpy(m_represent , _data);
}

const WCHAR* CUI_Combobox::GetRepresent_data(void)
{
	return m_represent;
}
