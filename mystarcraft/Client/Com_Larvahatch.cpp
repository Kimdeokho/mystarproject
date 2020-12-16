#include "StdAfx.h"
#include "Com_Larvahatch.h"

#include "TimeMgr.h"
#include "ObjMgr.h"

#include "Larva.h"

#include "UnitMgr.h"
CCom_Larvahatch::CCom_Larvahatch(int larvacnt)
{
	m_Init_larvacnt = larvacnt;
}

CCom_Larvahatch::~CCom_Larvahatch(void)
{
	Release();


}

void CCom_Larvahatch::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	AddLarva(m_Init_larvacnt);

	m_hatchtimer = 0.f;
}

void CCom_Larvahatch::Update(void)
{
	m_hatchtimer += GETTIME;

	if( 2 < m_hatchtimer)
	{
		m_hatchtimer = 0.f;
		//부화

		AddLarva(1);
	}
}

void CCom_Larvahatch::AddLarva(const int loopcnt)
{
	for(int i = 0; i < loopcnt; ++i)
	{
		unsigned short larvasize = m_larvalist.size();


		if(larvasize < 3)
		{
			MYRECT<float> hatchvtx = m_pobj->GetVertex();
			D3DXVECTOR2 hatchpos = m_pobj->GetPos();

			hatchpos.x = hatchpos.x - (hatchvtx.left/2) + (larvasize * 26);
			hatchpos.y = hatchpos.y + hatchvtx.bottom + 8;

			CLarva* plarva = new CLarva(m_pobj->GetObjNumber()); //생성자에 해처리 주소를 줘서 라바가 사라질때 여기에 정보를주자

			plarva->SetPos(hatchpos);
			plarva->SetTeamNumber(m_pobj->GetTeamNumber());
			plarva->Initialize();
			CObjMgr::GetInstance()->AddObject(plarva, OBJ_LARVA);
			m_larvalist.push_back(plarva);
		}
	}
}

void CCom_Larvahatch::Decrease_Larva(CObj* plarva)
{
	list<CObj*>::iterator iter = m_larvalist.begin();
	list<CObj*>::iterator iter_end = m_larvalist.end();

	for( ; iter != iter_end; ++iter)
	{
		if( (*iter) == plarva )
		{
			m_larvalist.erase(iter);
			break;
		}
	}
}
DWORD CCom_Larvahatch::GetLarvacount(void)
{
	return m_larvalist.size();
}
void CCom_Larvahatch::Release(void)
{
	m_larvalist.clear();
}

void CCom_Larvahatch::Select_Larva(void)
{
	list<CObj*>::iterator iter = m_larvalist.begin();
	list<CObj*>::iterator iter_end = m_larvalist.end();

	for( ; iter != iter_end; ++iter)
	{
		CUnitMgr::GetInstance()->SetUnit((*iter));
		(*iter)->SetSelect(GENERAL_SELECT);
	}
}
