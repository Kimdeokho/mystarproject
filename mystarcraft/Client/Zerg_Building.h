#pragma once
#include "obj.h"

enum LINE_DIR{LINE_LDIR , LINE_RDIR , LINE_END};
enum UPDOWN_DIR{LINE_UP , LINE_DOWN , UPDOWN_END};
class CZerg_building :
	public CObj
{
protected:
	int m_top_line[LINE_END];
	int m_bot_line[LINE_END];
	int m_left_line[UPDOWN_END];
	int m_right_line[UPDOWN_END];

	float	m_creeptimer;
	float   m_creepoff_timer;


	int		m_loopcnt1;
	int		m_loopcnt2;
	int		m_irange;
	int		m_offset;
	bool	m_bswitch;
	bool	m_bcreep_complete;
	bool	m_bdestroy;

	TILE**			m_sqTile;
	list<int>		m_creep_list;
	list<int>		m_creepoff_list;

public:
	void Creep_Initialize(bool bcomplete);
	void Creep_expansion(void);
	void Creep_decrase(void);
public:
	void SetDestroy(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CZerg_building(void);
	virtual ~CZerg_building(void);
};
