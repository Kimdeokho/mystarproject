#include "StdAfx.h"
#include "UI_armyunitlist.h"

#include "Obj.h"
#include "UI_form.h"
CUI_armyunitlist::CUI_armyunitlist(const D3DXVECTOR2& vpos)
{
	m_vpos = vpos;
}

CUI_armyunitlist::~CUI_armyunitlist(void)
{
	Release();
}

void CUI_armyunitlist::Initialize(void)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			m_backform[i][j] = new CUI_form;
			m_backform[i][j]->set_texture(L"ARMY_EDGE" , D3DXVECTOR2(m_vpos.x + j*37 , m_vpos.y + i*37));
			m_backform[i][j]->set_active(false);

			m_unitform[i][j] = new CUI_form(D3DXVECTOR2(0.5f , 0.5f));
			m_unitform[i][j]->set_texture(L"" , D3DXVECTOR2(m_vpos.x + j*37 + 1 , m_vpos.y + i*37 + 1));
			m_unitform[i][j]->set_active(false);
		}
	}
}

void CUI_armyunitlist::Update(void)
{
	if(!m_armyunit_list->empty())
	{
		if( 1 < m_armyunit_list->size())
		{
			int icnt = 0;
			int irow = 0;
			int icol = 0;

			list<CObj*>::iterator iter = m_armyunit_list->begin();
			list<CObj*>::iterator iter_end = m_armyunit_list->end();

			for( ; iter != iter_end; ++iter)
			{
				if( 12 <= icnt)
					break;

				icol = icnt % 2;
				irow = icnt / 2;

				((CUI_form*)m_backform[icol][irow])->set_active(true);
				((CUI_form*)m_unitform[icol][irow])->set_active(true);
				set_wireframe((*iter)->GetOBJNAME() , icol, irow);

				++icnt;
			}

			for(int i = icnt; i < 12; ++i)
			{
				icol = i % 2;
				irow = i / 2;

				((CUI_form*)m_backform[icol][irow])->set_active(false);
				((CUI_form*)m_unitform[icol][irow])->set_active(false);
			}
		}
		else
		{
			for(int i = 0; i < 2; ++i)
			{
				for(int j = 0; j < 6; ++j)
				{
					((CUI_form*)m_backform[i][j])->set_active(false);
					((CUI_form*)m_unitform[i][j])->set_active(false);
				}
			}
		}
	}
}

void CUI_armyunitlist::Render(void)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			m_backform[i][j]->Render();
			m_unitform[i][j]->Render();
		}
	}
}


void CUI_armyunitlist::set_armylist(list<CObj*>* armylist)
{
	m_armyunit_list = armylist;
}
void CUI_armyunitlist::set_wireframe(OBJID eid , const int& icol , const int& irow)
{
	if(OBJ_SCV == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_SCV");
	else if(OBJ_MARINE == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_MARINE");
	else if(OBJ_GOLIATH == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_GOLIATH");
	else if(OBJ_MEDIC == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_MEDIC");
	else if(OBJ_VULTURE == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_VULTURE");
	else if(OBJ_BATTLE == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_BATTLE");
	else if(OBJ_WRAITH == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_WRAITH");
	else if(OBJ_TANK == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_TANK");
	else if(OBJ_SIEGETANK == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_SIEGETANK");
	else if(OBJ_FIREBAT == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_FIREBAT");
	else if(OBJ_DROPSHIP == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_DROPSHIP");
	else if(OBJ_GHOST == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_GHOST");
	else if(OBJ_VESSEL == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_VESSEL");

	else if(OBJ_ZERGLING == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_ZERGLING");
	else if(OBJ_DRONE == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_DRONE");
	else if(OBJ_HYDRA == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_HYDRA");
	else if(OBJ_MUTAL == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_MUTAL");
	else if(OBJ_QUEEN == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_QUEEN");
	else if(OBJ_SCOURGE == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_SCOURGE");
	else if(OBJ_ULTRA == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_ULTRA");
	else if(OBJ_DEFILER == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_DEFILER");
	else if(OBJ_OVERLOAD == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_OVERLOAD");
	else if(OBJ_LARVA== eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_LARVA");
	else if(OBJ_LURKER == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_LURKER");
	else if(OBJ_LARVA_EGG == eid)
		((CUI_form*)m_unitform[icol][irow])->set_texture(L"WIRE_EGG");
}
void CUI_armyunitlist::Release(void)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			Safe_Delete(m_backform[i][j]);
			Safe_Delete(m_unitform[i][j]);
		}
	}
}
