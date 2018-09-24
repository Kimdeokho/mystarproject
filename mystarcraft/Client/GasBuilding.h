#pragma once
#include "obj.h"

class CCom_Animation;
class CGasBuilding :
	public CObj
{
protected:
	TEXINFO*						m_curtex;
protected:
	CObj*			m_pworkman;
	CObj*			m_pgas_resource;
protected:
	bool			m_bcome;
protected:
	CCom_Animation*		m_com_anim;
	CComponent*			m_com_targetsearch;
	CComponent*			m_com_pathfind;
	CComponent*			m_com_collision;
protected:
	D3DXMATRIX		m_matshadow;

	int				m_icol;
	int				m_irow;
	D3DXVECTOR2		m_weight;
	vector<int>		m_areaidx_vec;
	vector<int>		m_old_areaidx_vec;
protected:
	float			m_fgather_time;

	float			m_fbuild_tick;
	float			m_build_hp;

public:
	void building_area_Initialize(const int& col , const int& row);
	void area_release(void);
	CObj* Getworkman(void);
	void Setworkman(CObj* pobj);
	void Setgas_resource(CObj* pobj);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
public:
	CGasBuilding(void);
	virtual ~CGasBuilding(void);
};
