#include "StdAfx.h"
#include "UI_Resource.h"

#include "TimeMgr.h"
#include "FontMgr.h"
#include "TextureMgr.h"
#include "RoomSession_Mgr.h"
#include "Session_Mgr.h"

CUI_Resource::CUI_Resource(void)
{
}

CUI_Resource::~CUI_Resource(void)
{
}

void CUI_Resource::Initialize(void)
{
	m_tick = 0.f;

	m_tex_mineral		= CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"ICON_MINERAL");
	m_tex_gas			= CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"ICON_GAS");
	m_tex_population	= CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"ICON_POPULATION");

	m_mineral_pos = D3DXVECTOR2(340,20);
	m_gas_pos = D3DXVECTOR2(440,20);
	m_population_pos = D3DXVECTOR2(540,20);

	memset(m_mineral_amount , 0 , sizeof(m_mineral_amount));
	memset(m_gas_amount , 0 , sizeof(m_gas_amount));
	memset(m_cur_mineral , 0 , sizeof(m_cur_mineral));
	memset(m_cur_gas , 0 , sizeof(m_cur_gas));
	memset(m_population_val , 0 , sizeof(m_population_val));
	memset(m_population_maxval , 0 , sizeof(m_population_maxval));

	for(int i = 0; i < TEAM_END; ++i)
	{
		m_population_maxval[i] = 800;
		m_mineral_amount[i] = 100000;
		m_cur_mineral[i] = m_mineral_amount[i];

		m_gas_amount[i] = 100000;		
		m_cur_gas[i] = m_gas_amount[i];
	}

	m_myteam = CSession_Mgr::GetInstance()->GetTeamNumber();
}

void CUI_Resource::Update(void)
{
	m_tick += GETTIME;

	if(m_tick > 0.005f)
	{
		m_tick = 0.f;

		if(m_mineral_amount[m_myteam] > m_cur_mineral[m_myteam])
			m_cur_mineral[m_myteam] += 1;
		else if(m_mineral_amount[m_myteam] < m_cur_mineral[m_myteam])
			m_cur_mineral[m_myteam] -= 1;

		if(m_gas_amount[m_myteam] > m_cur_gas[m_myteam])
			m_cur_gas[m_myteam] += 1;
		else if(m_gas_amount[m_myteam] < m_cur_gas[m_myteam])
			m_cur_gas[m_myteam] -= 1;
	}

	CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_cur_mineral[m_myteam] , m_mineral_pos.x + 23, m_mineral_pos.y , D3DCOLOR_ARGB(255,255,255,255) , true);
	CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_cur_gas[m_myteam] , m_gas_pos.x + 23, m_gas_pos.y , D3DCOLOR_ARGB(255,255,255,255) , true);
	CFontMgr::GetInstance()->Setbatch_Font(L"%d / %d" ,(int)(m_population_val[m_myteam] + 0.5f) , (int)m_population_maxval[m_myteam], m_population_pos.x + 23, m_population_pos.y , D3DCOLOR_ARGB(255,255,255,255) , true);
}

void CUI_Resource::Render(void)
{
	D3DXMATRIX tempmat;
	D3DXMatrixIdentity(&tempmat);

	tempmat._11 = 1.3f;
	tempmat._22 = 1.3f;
	tempmat._41 = m_mineral_pos.x;
	tempmat._42 = m_mineral_pos.y;
	m_pSprite->SetTransform(&tempmat);
	m_pSprite->Draw(m_tex_mineral->pTexture , NULL , &D3DXVECTOR3(0.f ,0.f,0.f) , NULL , 
		m_color);

	tempmat._41 = m_gas_pos.x;
	tempmat._42 = m_gas_pos.y;
	m_pSprite->SetTransform(&tempmat);
	m_pSprite->Draw(m_tex_gas->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
		m_color);

	tempmat._41 = m_population_pos.x;
	tempmat._42 = m_population_pos.y;
	m_pSprite->SetTransform(&tempmat);
	m_pSprite->Draw(m_tex_population->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
		m_color);
}
bool CUI_Resource::SetResource(const int _mineral ,const int _gas, const TEAM_NUMBER eteam)
{
	if( m_mineral_amount[eteam] + _mineral < 0 ||
		m_gas_amount[eteam] + _gas < 0)
	{
		return false;//자원부족
	}
	else
	{
		m_mineral_amount[eteam] += _mineral;
		m_gas_amount[eteam] += _gas;
		return true; //자원 교환 성공
	}
}

bool CUI_Resource::SetPopvalue(const float _popvalue , const TEAM_NUMBER eteam)
{
	if( m_population_val[eteam] < 0)
	{
		m_population_val[eteam] = 0.f;
		return false;
	}
	else if( int(m_population_val[eteam] + _popvalue) > m_population_maxval[eteam])
	{
		return false;
	}
	else
	{
		m_population_val[eteam] += _popvalue;
		return true;
	}
}

void CUI_Resource::SetMaxPopvalue(const float _popvalue , const TEAM_NUMBER eteam)
{
	m_population_maxval[eteam] += _popvalue;
	if(m_population_maxval[eteam] > 800)
		m_population_maxval[eteam] = 800;
	if(m_population_maxval[eteam] < 0)
		m_population_maxval[eteam] = 0;
}

void CUI_Resource::Release(void)
{

}

