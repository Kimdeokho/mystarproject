#pragma once
#include "component.h"

class CCom_fog :
	public CComponent
{
private:
	list<int>			m_Sightoff_List;
	bool				m_sightoffsw;
	bool				m_InitSight;
	bool				m_fogsearch[SQ_TILECNTY*SQ_TILECNTX];

	int					m_iSightrange;
	float				m_fogtime;
	int					m_curidx32;
	int					m_oldidx32;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Release(void);
public:
	CCom_fog(void);
	CCom_fog(const int& iragne);
	~CCom_fog(void);
};
