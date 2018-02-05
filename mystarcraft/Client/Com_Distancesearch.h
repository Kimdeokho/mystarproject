#pragma once
#include "com_targetsearch.h"

class CCom_Distancesearch :
	public CCom_Targetsearch
{
private:
	CObj*			m_otherobj; //��ũ�� �񸮾Ѱ��� �и��� ������Ʈ ����

	float			m_search_time;
	bool			m_btarget_search;

	int				m_irange;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Distancesearch(TARGET_SEARCH_TYPE esearch_type , CObj* otherobj = NULL);
	~CCom_Distancesearch(void);
};
