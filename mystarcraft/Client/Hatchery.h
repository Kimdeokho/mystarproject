#pragma once
#include "Zerg_Building.h"

class CHatchery :
	public CZerg_Building
{
private:
	//int m_top_line[LINE_END];
	//int m_bot_line[LINE_END];
	//int m_left_line[UPDOWN_END];
	//int m_right_line[UPDOWN_END];

	//float m_timer;
	//int m_tickcnt;

	//int m_loopcnt1;
	//int m_loopcnt2;
	//int m_irange;
	//int m_offset;
	//bool m_bswitch;
	//bool m_once;

	//TILE**			m_sqTile;
	//CREEP_INFO**	m_creepTile;
	//list<int>		m_creep_list;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CHatchery(void);
	virtual ~CHatchery(void);
};
