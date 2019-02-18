#include "StdAfx.h"
#include "UI_Boarding_info.h"

#include "UI_form.h"
#include "UI_wireform.h"
CUI_Boarding_info::CUI_Boarding_info(void)
{
}

CUI_Boarding_info::~CUI_Boarding_info(void)
{
	Release();
}

void CUI_Boarding_info::Initialize(void)
{	

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			m_occupy[i][j] = true;
			m_uiform_pos[i][j] = D3DXVECTOR2(m_vpos.x + j*34 , m_vpos.y + i* 34);

			m_backform[i][j] = new CUI_form;
			((CUI_form*)(m_backform[i][j]))->SetPos(m_uiform_pos[i][j]);

			m_wireform[i][j] = new CUI_wireform;
			((CUI_wireform*)(m_wireform[i][j]))->SetPos(m_uiform_pos[i][j]);
		}
	}

	m_is_active = false;
}

void CUI_Boarding_info::Update(void)
{

}

void CUI_Boarding_info::Render(void)
{
	if(!m_is_active)
		return;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			m_backform[i][j]->Render();
			m_wireform[i][j]->Render();
			//((CUI_form*)(m_backform[i][j]))->set_texture()
		}
	}
}

void CUI_Boarding_info::Release(void)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			Safe_Delete(m_backform[i][j]);
			Safe_Delete(m_wireform[i][j]);
		}
	}
}

void CUI_Boarding_info::set_boarding_infolist(multimap<int , BOARDING_INFO , greater<int> >& infolist , OBJID eid)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			((CUI_form*)(m_backform[i][j]))->set_texture(L"");
			((CUI_wireform*)(m_wireform[i][j]))->set_texturekey(L"");
			m_occupy[i][j] = true;
		}
	}

	if(!infolist.empty())
	{
		m_is_active = true;

		multimap<int ,BOARDING_INFO , greater<int>>::iterator iter = infolist.begin();
		multimap<int ,BOARDING_INFO , greater<int>>::iterator iter_end = infolist.end();

		BOARDING_INFO temp_info;

		TCHAR	tempstr[32] = L"";


		int startcol = 0;

		if(OBJ_BUNKER == eid)
			startcol = 1;

		bool is_escape = false;

		for( ; iter != iter_end; ++iter)
		{
			is_escape = false;
			temp_info = iter->second;

			for(int col = startcol; col < 4; ++col)
			{
				for(int row = 0; row < 2; ++row)
				{
					if(true == m_occupy[row][col])
					{
						wsprintf(tempstr , L"EDGE_%dX%d" , temp_info.irow , temp_info.icol);

						((CUI_form*)(m_backform[row][col]))->set_texture(tempstr);

						((CUI_wireform*)(m_wireform[row][col]))->set_texturekey(temp_info.texkey);
						((CUI_wireform*)(m_wireform[row][col]))->set_objaddress( (CObj*)(temp_info.obj_address) );

						for(int m = 0; m < temp_info.irow; ++m)
						{
							for(int n = 0; n < temp_info.icol; ++n)
							{
								m_occupy[row + m][col + n] = false;
							}
						}
						is_escape = true;
						break;;
					}
				}
				if(true == is_escape)
					break;
			}
		}
	}
}

void CUI_Boarding_info::SetActive(bool is_active)
{
	m_is_active = is_active;
}
