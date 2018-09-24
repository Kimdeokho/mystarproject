#pragma once
#include "component.h"

class CCom_fog :
	public CComponent
{
private:
	//list<int>			m_Sightoff_List;
	vector<int>			m_Sightoff_List;
	bool				m_sightoffsw;
	bool				m_InitSight;
	bool				m_fogsearch[SQ_TILECNTY*SQ_TILECNTX];

	const int*			m_iSightrange;
	float				m_fogtime;
	const int&			m_rcuridx32;
	int					m_oldidx32;

	TEAM_NUMBER			m_eteamnumber;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Release(void);
public:
	//���۷��� ��������� �ʱ�ȭ�Ҷ�
	//�����ڰ� �����ε��� ���� 
	//�ʱ�ȭ�� �Ͼ�� �ʴ� ������ ������ �ȵȴ�.
	CCom_fog(const int& curidx , const int* irange);
	~CCom_fog(void);
};