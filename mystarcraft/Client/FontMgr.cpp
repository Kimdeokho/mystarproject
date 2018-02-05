#include "StdAfx.h"
#include "FontMgr.h"
#include "Device.h"

#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CFontMgr)
CFontMgr::CFontMgr(void)
{
}

CFontMgr::~CFontMgr(void)
{
}

void CFontMgr::Initialize(void)
{
	lstrcpy(m_szfont , L"");
	D3DXMatrixIdentity(&m_matfont);
	m_pDevice = CDevice::GetInstance();
	m_pSprite = CDevice::GetInstance()->GetSprite();
	m_pFont   = CDevice::GetInstance()->GetFont();

	m_vecbatchfont.reserve(600);
		
}

void CFontMgr::FontRender(const TCHAR*	szfont , float posX , float posY , D3DCOLOR _color)
{
	lstrcpy(m_szfont , szfont);
	int len = lstrlen(m_szfont);
	m_matfont._41 = posX - len*10/2;
	m_matfont._42 = posY;
	m_fontcolor	  = _color;

	RECT rc = {0};

	m_pSprite->SetTransform(&m_matfont);
	m_pFont->DrawTextW(m_pSprite , m_szfont, len,
		&rc ,  DT_NOCLIP , m_fontcolor);
}
void CFontMgr::FontRender(void)
{
	RECT rc = {0};

	if(!m_render_fontlist.empty())
	{
		list<FONT_INFO>::iterator iter = m_render_fontlist.begin();
		list<FONT_INFO>::iterator iter_end = m_render_fontlist.end();

		for( ; iter != iter_end; ++iter)
		{
			m_matfont._41 = (*iter).fX;
			m_matfont._42 = (*iter).fY;

			m_pSprite->SetTransform(&m_matfont);
			m_pFont->DrawTextW(m_pSprite, (*iter).font , lstrlen((*iter).font) , &rc , DT_NOCLIP , (*iter).font_color );


		}
		//m_render_fontlist.clear();
	}


	if(!m_keyrender.empty())
	{
		list<FONT_INFO>::iterator iter = m_keyrender.begin();
		list<FONT_INFO>::iterator iter_end = m_keyrender.end();

		for( ; iter != iter_end; ++iter)
		{
			m_matfont._41 = (*iter).fX;
			m_matfont._42 = (*iter).fY;

			m_pSprite->SetTransform(&m_matfont);
			m_pFont->DrawTextW(m_pSprite, (*iter).font , lstrlen((*iter).font) , &rc , DT_NOCLIP , (*iter).font_color );

		}
	}


	if(!m_vecbatchfont.empty())
	{
		size_t loopcnt = m_vecbatchfont.size();

		for(size_t i = 0; i < loopcnt; ++i)
		{
			m_matfont._41 = m_vecbatchfont[i].fX - lstrlen( m_vecbatchfont[i].font )/2*10;;
			m_matfont._42 = m_vecbatchfont[i].fY;

			m_pSprite->SetTransform(&m_matfont);
			m_pFont->DrawTextW(m_pSprite, m_vecbatchfont[i].font , lstrlen(m_vecbatchfont[i].font) , &rc , DT_NOCLIP , m_vecbatchfont[i].font_color );
		}
		m_vecbatchfont.clear();
	}

	if(!m_list_noticefont.empty())
	{
		list<FONT_INFO>::iterator iter = m_list_noticefont.begin();
		list<FONT_INFO>::iterator iter_end = m_list_noticefont.end();

		for( ; iter != iter_end; )
		{
			(*iter).fnotice_time += GETTIME;

			if( (*iter).fnotice_time > 5.0f)
			{
				iter = m_list_noticefont.erase(iter);
			}
			else
			{
				m_matfont._41 = (*iter).fX - lstrlen((*iter).font)/2*10;
				m_matfont._42 = (*iter).fY;
				m_pSprite->SetTransform(&m_matfont);
				m_pFont->DrawTextW(m_pSprite, (*iter).font , lstrlen((*iter).font) , &rc , DT_NOCLIP , (*iter).font_color );
				++iter;
			}
		}
	}

	if(!m_infomation_font.empty())
	{
		list<FONT_INFO>::iterator iter = m_infomation_font.begin();
		list<FONT_INFO>::iterator iter_end = m_infomation_font.end();

		for( ; iter != iter_end; )
		{
			m_matfont._11 = 0.7f;
			m_matfont._22 = 0.7f;
			m_matfont._41 = (*iter).fX - lstrlen((*iter).font)/2*10;
			m_matfont._42 = (*iter).fY;
			m_pSprite->SetTransform(&m_matfont);
			m_pFont->DrawTextW(m_pSprite, (*iter).font , lstrlen((*iter).font) , &rc , DT_NOCLIP , (*iter).font_color );
			++iter;
		}
	}
}
void CFontMgr::Setnumber_combine_Font(const TCHAR* szfont , const int& font_number , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	m_render_fontlist.clear();

	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;
	
	wsprintf(ptemp.font , szfont , font_number);

	m_render_fontlist.push_back(ptemp);
}
void CFontMgr::Set_KeyInput_Font(const TCHAR* szfont , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/ )
{
	FONT_INFO ptemp;

	float startpos = 500;

	if(m_keyrender.size() >= 30)
	{
		m_keyrender.pop_front();
	}

	if(!m_keyrender.empty())
	{
		for(list<FONT_INFO>::iterator iter = m_keyrender.begin(); iter != m_keyrender.end(); ++iter)
			(*iter).fY -= 15;
	}

	lstrcpy(ptemp.font , szfont);
	ptemp.fX = 10;
	ptemp.fY = startpos;
	ptemp.font_color = _color;

	m_keyrender.push_back(ptemp);



}
void CFontMgr::Setbatch_Font(const TCHAR* szfont , const int& font_number , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
		/*업데이트단계에서 지속적으로 입력받는 폰트
	입력받고 렌더 후 지우는 방식이다*/
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;

	wsprintf(ptemp.font , szfont , font_number);

	m_vecbatchfont.push_back(ptemp);
}
void CFontMgr::Setbatch_Font(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	/*업데이트단계에서 지속적으로 입력받는 폰트
	입력받고 렌더 후 지우는 방식이다*/
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;
	lstrcpy(ptemp.font , szfont);
	m_vecbatchfont.push_back(ptemp);
}
void CFontMgr::Setbatch_Font(const TCHAR* szfont ,const int& num1, const int& num2, float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	/*업데이트단계에서 지속적으로 입력받는 폰트
	입력받고 렌더 후 지우는 방식이다*/
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;
	
	wsprintf(ptemp.font , szfont , num1, num2);

	m_vecbatchfont.push_back(ptemp);
}
void CFontMgr::SetFontInfo(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	/*한번 입력받고 새로운 폰트가 들어오지 않는이상 입력받은 폰트를 계속 띄운다*/

	m_render_fontlist.clear();

	FONT_INFO ptemp;

	lstrcpy(ptemp.font , szfont);
	int len = lstrlen(ptemp.font);
	ptemp.fX = posX - len*10/2;
	ptemp.fY = posY;
	m_fontcolor	  = _color;

	m_render_fontlist.push_back(ptemp);
}
void CFontMgr::SetNoticeFont(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,255,255,255)*/)
{
	/*한번 입력받고 몇초동안 지속적으로 렌더 후 삭제되는 폰트*/
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;

	lstrcpy(ptemp.font , szfont);

	m_list_noticefont.push_back(ptemp);
}
void CFontMgr::SetNoticeFont(const TCHAR* szfont , const int& fontnumber, float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,255,255,255)*/)
{
	/*한번 입력받고 몇초동안 지속적으로 렌더 후 삭제되는 폰트*/
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;

	wsprintf(ptemp.font , szfont , fontnumber);

	m_list_noticefont.push_back(ptemp);
}
void CFontMgr::renewal_infomation_font(void)
{
	m_infomation_font.clear();
}
void CFontMgr::SetInfomation_font(const TCHAR* szfont , const int& number1, const int& number2, const float& posX , const float& posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,255,255,255)*/)
{
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;

	wsprintf(ptemp.font , szfont , number1 , number2);

	m_infomation_font.push_back(ptemp);
}
void CFontMgr::SetInfomation_font(const TCHAR* szfont ,const float& posX ,const float& posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,255,255,255)*/)
{
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;

	lstrcpy(ptemp.font , szfont);

	m_infomation_font.push_back(ptemp);
}
void CFontMgr::Release(void)
{
	m_render_fontlist.clear();
}