#include "StdAfx.h"
#include "Com_CC.h"

#include "Skill.h"
CCom_CC::CCom_CC(void)
{
}

CCom_CC::~CCom_CC(void)
{
	Release();
}

void CCom_CC::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;
	m_defensive = NULL;
}

void CCom_CC::Update(void)
{
	if(!m_buffs.empty())
	{
		SKILL_ITER iter = m_buffs.begin();
		SKILL_ITER iter_end = m_buffs.end();

		for( ; iter != iter_end; )
		{
			if( true == iter->second->GetDestroy() )
			{
				if(SN_DEFENSIVE == iter->second->GetSkillName())
					m_defensive = NULL;

				Safe_Delete(iter->second);
				iter = m_buffs.erase(iter);
			}
			else
			{
				iter->second->Update();
				++iter;
			}
		}
	}
	if(!m_debuffs.empty())
	{
		SKILL_ITER iter = m_debuffs.begin();
		SKILL_ITER iter_end = m_debuffs.end();

		for( ; iter != iter_end; )
		{
			if( true == iter->second->GetDestroy() )
			{
				Safe_Delete(iter->second);
				iter = m_buffs.erase(iter);
			}
			else
			{
				iter->second->Update();
				++iter;
			}
		}
	}

	if(!m_overlap_buffs.empty())
	{
		list<CSkill*>::iterator iter = m_overlap_buffs.begin();
		list<CSkill*>::iterator iter_end = m_overlap_buffs.end();

		for( ; iter != iter_end; )
		{
			if( true == (*iter)->GetDestroy() )
			{
				Safe_Delete((*iter));
				iter = m_overlap_buffs.erase(iter);
			}
			else
			{
				(*iter)->Update();
				++iter;
			}
		}
	}
	if(!m_overlap_debuffs.empty())
	{
		list<CSkill*>::iterator iter = m_overlap_debuffs.begin();
		list<CSkill*>::iterator iter_end = m_overlap_debuffs.end();

		for( ; iter != iter_end; )
		{
			if( true == (*iter)->GetDestroy() )
			{
				Safe_Delete((*iter));
				iter = m_overlap_debuffs.erase(iter);
			}
			else
			{
				(*iter)->Update();
				++iter;
			}
		}
	}
}
void CCom_CC::Render(void)
{
	if(!m_buffs.empty())
	{
		SKILL_ITER iter = m_buffs.begin();
		SKILL_ITER iter_end = m_buffs.end();

		for( ; iter != iter_end; ++iter)
		{
			iter->second->Render();
		}
	}
	if(!m_debuffs.empty())
	{
		SKILL_ITER iter = m_debuffs.begin();
		SKILL_ITER iter_end = m_debuffs.end();

		for( ; iter != iter_end; ++iter)
		{
			iter->second->Render();
		}
	}

	if(!m_overlap_buffs.empty())
	{
		list<CSkill*>::iterator iter = m_overlap_buffs.begin();
		list<CSkill*>::iterator iter_end = m_overlap_buffs.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}
	if(!m_overlap_debuffs.empty())
	{
		list<CSkill*>::iterator iter = m_overlap_debuffs.begin();
		list<CSkill*>::iterator iter_end = m_overlap_debuffs.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}
}
void CCom_CC::Release(void)
{
	SKILL_ITER iter;
	SKILL_ITER iter_end;

	if(!m_buffs.empty())
	{
		iter = m_buffs.begin();
		iter_end = m_buffs.end();

		for( ; iter != iter_end; ++iter)
			Safe_Delete(iter->second);

		m_buffs.clear();
	}
	if(!m_debuffs.empty())
	{
		iter = m_debuffs.begin();
		iter_end = m_debuffs.end();

		for( ; iter != iter_end; ++iter)
			Safe_Delete(iter->second);

		m_debuffs.clear();
	}

	if(!m_overlap_buffs.empty())
	{
		list<CSkill*>::iterator iter = m_overlap_buffs.begin();
		list<CSkill*>::iterator iter_end = m_overlap_buffs.end();

		for( ; iter != iter_end; ++iter)
			Safe_Delete( (*iter) );

		m_overlap_buffs.clear();
	}
	if(!m_overlap_debuffs.empty())
	{
		list<CSkill*>::iterator iter = m_overlap_debuffs.begin();
		list<CSkill*>::iterator iter_end = m_overlap_debuffs.end();

		for( ; iter != iter_end; ++iter)
			Safe_Delete( (*iter) );

		m_overlap_debuffs.clear();
	}
}

void CCom_CC::AddBuff(CSkill* pskill)
{
	SKILL_NAME sn = pskill->GetSkillName();
	SKILL_ITER iter = m_buffs.find(sn);

	if(SN_DEFENSIVE == sn)
		m_defensive = pskill;

	if(iter != m_buffs.end())
	{
		Safe_Delete(iter->second);
		m_buffs.erase(iter);
	}

	m_buffs.insert(SKILL_PAIR::value_type(sn , pskill));
}

void CCom_CC::AddDebuff(CSkill* pskill)
{
	SKILL_NAME sn = pskill->GetSkillName();
	SKILL_ITER iter = m_debuffs.find(sn);

	if(iter != m_debuffs.end())
	{
		Safe_Delete(iter->second);
		m_debuffs.erase(iter);
	}

	m_debuffs.insert(SKILL_PAIR::value_type(sn , pskill));
}

void CCom_CC::AddOverlap_Buff(CSkill* pskill)
{
	m_overlap_buffs.push_back(pskill);
}

void CCom_CC::AddOverlap_DeBuff(CSkill* pskill)
{
	m_overlap_debuffs.push_back(pskill);
}

CSkill* CCom_CC::GetDefensive(void)
{
	return m_defensive;
}

