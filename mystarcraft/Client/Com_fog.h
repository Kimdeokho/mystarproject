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
	//레퍼런스 멤버변수를 초기화할때
	//생성자가 오버로딩에 인해 
	//초기화가 일어나지 않는 현상이 있으면 안된다.
	CCom_fog(const int& curidx , const int* irange);
	~CCom_fog(void);
};