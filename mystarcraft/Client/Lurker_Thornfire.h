#pragma once
#include "obj.h"

class CLurker_Thornfire :
	public CObj
{
private:
	int				m_range;
	int				m_curcnt;

	float			m_thorn_delay;
	float			m_splash_range[3];

	D3DXVECTOR2		m_vstart;
	D3DXVECTOR2		m_vend;

	WEAPON_INFO		m_weapon_info;

	CObj*			m_pobj;
	list<CObj*>		m_thorn_list;
	list<CObj*>		m_injure_list;

	UPG_INFO*		m_upg_info;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CLurker_Thornfire(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend,const int range , const WEAPON_INFO& tinfo ,CObj* pobj);
	~CLurker_Thornfire(void);
};
