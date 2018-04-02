#include "StdAfx.h"
#include "Skill.h"

CSkill::CSkill(void)
{
	m_bdestroy = false;
	m_skillname = SN_NONE;

	D3DXMatrixIdentity(&m_matWorld);
}

CSkill::~CSkill(void)
{
}

void CSkill::Initialize(void)
{

}

void CSkill::Update(void)
{

}

void CSkill::SetDestroy(bool bdestroy)
{
	m_bdestroy = bdestroy;
}

bool CSkill::GetDestroy(void)
{
	return m_bdestroy;
}

SKILL_NAME CSkill::GetSkillName(void)
{
	return m_skillname;
}

void CSkill::Render(void)
{

}

void CSkill::Use(const int& objid , const D3DXVECTOR2& vpt)
{

}

void CSkill::Release(void)
{

}
