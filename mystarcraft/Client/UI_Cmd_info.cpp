#include "StdAfx.h"
#include "UI_Cmd_info.h"

#include "Cmd_btn.h"
#include "Obj.h"

#include "Ingame_UIMgr.h"
CUI_Cmd_info::CUI_Cmd_info(void)
{
}

CUI_Cmd_info::~CUI_Cmd_info(void)
{
	Release();
}

void CUI_Cmd_info::Initialize(void)
{
	m_cmdbtn_list.reserve(9);
	m_cmdbtn_list.resize(9);

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			m_vcmdbtn_pos[ i * 3 + j ].x = float(m_vpos.x + j * 46);
			m_vcmdbtn_pos[ i * 3 + j ].y = float(m_vpos.y + i * 41);
		}
	}

	for(int i = 0; i < 9; ++i)
	{
		m_cmdbtn_list[i] = new CCmd_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , false);
	}

	m_clicked_cmd_btn = BTN_NONE;
}

void CUI_Cmd_info::Update(void)
{

}

void CUI_Cmd_info::Render(void)
{
	for(int i = 0; i < 9; ++i) 
	{
		if(NULL == m_cmdbtn_list[i])
			continue;

		m_cmdbtn_list[i]->Render();
	}
}

void CUI_Cmd_info::Release(void)
{
	if(!m_cmdbtn_list.empty())
	{
		for(int i = 0; i < 9; ++i)
		{
			Safe_Delete( m_cmdbtn_list[i] );
		}
		m_cmdbtn_list.clear();
	}
}
void CUI_Cmd_info::Update_Cmdbtn(CObj* pobj)
{
	//pobj 동적 바인딩으로 변경가능
	CUI* pui = NULL;

	if(NULL == pobj)
	{
		clear_btn();

		//서로다른 유닛 여러마리
		Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
		Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
		Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
		Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
		Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);
	}
	else
	{
		//같은종류
		clear_btn();

		pobj->Update_Cmdbtn();
	}
}
void CUI_Cmd_info::T_Cmdbtn_B_buildsetting(void)
{
	//단축키 b를 누르면 나오는 종족 테크트리

	CUI* pui = NULL;
	for(int i = 0; i < 9; ++i)
	{
		pui = m_cmdbtn_list[i];
		((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
	}

	Create_Cmdbtn(0 , L"BTN_COMMAND_CENTER" , BTN_COMAND_CENTER);
	Create_Cmdbtn(1 , L"BTN_SUPPLY" , BTN_SUPPLY );
	Create_Cmdbtn(2 , L"BTN_T_GAS" , BTN_T_GAS );

	if(0 < CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_COMMANDCENTER))
	{
		Create_Cmdbtn(3 , L"BTN_BARRACK" , BTN_BARRACK , true);
		Create_Cmdbtn(4 , L"BTN_EB" , BTN_EB , true);		
	}
	else
	{
		Create_Cmdbtn(3 , L"BTN_BARRACK" , BTN_BARRACK , false);
		Create_Cmdbtn(4 , L"BTN_EB" , BTN_EB , false);		
	}

	if( 0 < CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_BARRACK))
	{
		Create_Cmdbtn(6 , L"BTN_ACADEMY" , BTN_ACADEMY);
		Create_Cmdbtn(7 , L"BTN_BUNKER" , BTN_BUNKER);
	}
	else
	{
		Create_Cmdbtn(6 , L"BTN_ACADEMY" , BTN_ACADEMY , false);
		Create_Cmdbtn(7 , L"BTN_BUNKER" , BTN_BUNKER , false);
	}

	if( 0 < CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_EB) )
	{
		Create_Cmdbtn(5 , L"BTN_TURRET" , BTN_TURRET);
	}
	else
		Create_Cmdbtn(5 , L"BTN_TURRET" , BTN_TURRET , false);

}
void CUI_Cmd_info::T_Cmdbtn_V_buildsetting(void)
{
	CUI* pui = NULL;
	for(int i = 0; i < 9; ++i)
	{
		pui = m_cmdbtn_list[i];
		((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
	}

	if( 0 < CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_BARRACK) )
	{
		Create_Cmdbtn(0 , L"BTN_FACTORY" , BTN_FACTORY);
	}
	else
	{
		Create_Cmdbtn(0 , L"BTN_FACTORY" , BTN_FACTORY , false);
	}

	if( 0 < CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_FACTORY) )
	{
		Create_Cmdbtn(1 , L"BTN_STARPORT" , BTN_STARPORT);
		Create_Cmdbtn(3 , L"BTN_ARMORY" , BTN_ARMORY);
	}
	else
	{
		Create_Cmdbtn(1 , L"BTN_STARPORT" , BTN_STARPORT , false);
		Create_Cmdbtn(3 , L"BTN_ARMORY" , BTN_ARMORY , false);
	}

	if( 0 < CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_STARPORT) )
	{
		Create_Cmdbtn(2 , L"BTN_SIENCE" , BTN_SIENCE);
	}
	else
	{
		Create_Cmdbtn(2 , L"BTN_SIENCE" , BTN_SIENCE , false);
	}
}
bool CUI_Cmd_info::active_cmdbtn(const int& idx, CMD_BTN ebtn)
{
	if(NULL == m_cmdbtn_list[idx])
		return false;
	else
	{
		CUI* pui = m_cmdbtn_list[idx];
		if( ebtn == ((CCmd_btn*)pui)->GetCmdbtn_id() )
		{
			return true;
		}
	}
	return false;
}
void CUI_Cmd_info::Create_Cmdbtn(const int& idx ,const TCHAR* texkey, CMD_BTN ebtn , bool is_active)
{
	CUI* pui = NULL;
	if(NULL != m_cmdbtn_list[idx])
	{
		pui = m_cmdbtn_list[idx];
		if(ebtn == ((CCmd_btn*)pui)->GetCmdbtn_id())
			return;
		else
		{
			((CCmd_btn*)pui)->Init_btn(texkey , ebtn , m_vcmdbtn_pos[idx] , is_active);
		}
	}
}
void CUI_Cmd_info::clear_btn(void)
{
	CUI* pui = NULL;
	for(int i = 0; i < 9; ++i)
	{
		pui = m_cmdbtn_list[i];
		((CCmd_btn*)pui)->Init_btn(L"" , BTN_NONE , m_vcmdbtn_pos[i] , true);
	}

}
void CUI_Cmd_info::Click_cmdbtn(const D3DXVECTOR2& vpt)
{
	for(size_t i = 0; i < m_cmdbtn_list.size(); ++i)
	{
		if( MyPtInrect( vpt ,  &(m_cmdbtn_list[i]->GetMyRect()) ) )
		{
			m_clicked_cmd_btn = ((CCmd_btn*)m_cmdbtn_list[i])->GetCmdbtn_id();

			if(BTN_NONE == m_clicked_cmd_btn)
				continue;
			else
				return;
		}
	}

	m_clicked_cmd_btn = BTN_NONE;
}

CMD_BTN CUI_Cmd_info::Get_clicked_btn(void)
{
	return m_clicked_cmd_btn;
}
