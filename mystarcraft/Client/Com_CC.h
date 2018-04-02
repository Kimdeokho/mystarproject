#pragma once
#include "component.h"

class CSkill;

class CCom_CC :
	public CComponent
{
private:
	typedef boost::unordered_map<SKILL_NAME , CSkill*>	 SKILL_PAIR;
	typedef boost::unordered_map<SKILL_NAME , CSkill*>::iterator SKILL_ITER;

	boost::unordered_map<SKILL_NAME , CSkill*>		m_buffs;
	boost::unordered_map<SKILL_NAME , CSkill*>		m_debuffs;

	list<CSkill*>		m_overlap_buffs; //�ߺ� ������ ������
	list<CSkill*>		m_overlap_debuffs; //�ߺ� ����

	//���̿��н���, ��ũ���� ���� �Ǻ��� �� �ִ� ����
	//int storm ,b , c
	CSkill*				m_defensive;
public:
	void AddBuff(CSkill* pskill);
	void AddDebuff(CSkill* pskill);

	void AddOverlap_Buff(CSkill* pskill);
	void AddOverlap_DeBuff(CSkill* pskill);
public:
	CSkill* GetDefensive(void);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_CC(void);
	~CCom_CC(void);
};
