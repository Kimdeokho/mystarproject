#pragma once
#include "component.h"

class CCom_Production_building :
	public CComponent
{
private:
	int						m_irow;
	int						m_icol;
	const D3DXVECTOR2&		m_vPos;
	const D3DXVECTOR2&		m_weight;

	bool					m_isupdate;
	D3DXVECTOR2				m_rallypoint;
	bool					m_is_rally;
	vector<D3DXVECTOR2>		m_rallypath;
	list<PRODUCTION_INFO>	m_production_list;
public:
	void unit_collocate(CObj* const pobj);
	void rallypoint_pathfinding(void);
	void add_production_info(const float maxtime ,const float popval, OBJID eid , const TCHAR* texkey);
	void update_production(void);
	void create_unit(OBJID eid);
	void show_production_state();
	void set_rallypoint(const D3DXVECTOR2& vpos);
	void set_is_rally(bool isrally);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Production_building(const D3DXVECTOR2& vpos , const D3DXVECTOR2& vweight , const int& icol , const int& irow);
	~CCom_Production_building(void);
};
