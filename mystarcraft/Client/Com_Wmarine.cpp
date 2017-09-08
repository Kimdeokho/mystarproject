#include "StdAfx.h"
#include "Com_Wmarine.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
CCom_Wmarine::CCom_Wmarine(void)
{
}

CCom_Wmarine::~CCom_Wmarine(void)
{
}

void CCom_Wmarine::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (CCom_Animation*)(m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 2.f;

	m_bfire = false;
}
void CCom_Wmarine::fire(CObj*	ptarget)
{
	if(false == m_bfire)
	{
		m_bfire = true;
		m_animation->SetAnimation(L"ATTACK");
		//�Ѿ˻���
		//Ÿ�ٿ��� ����
	}


}
void CCom_Wmarine::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay) //�������� �̰� �ٰ���
		{
			m_attack_time = 0.f;
			m_bfire = false;
		}
	}
}

void CCom_Wmarine::Render(void)
{

}

void CCom_Wmarine::Release(void)
{

}

