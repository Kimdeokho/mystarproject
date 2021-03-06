#pragma once
#include "obj.h"

class CBullet :
	public CObj
{
protected:
	CObj*			m_ptarget;
	int				m_target_id;
	D3DXVECTOR2		m_vdest_pos;
	D3DXVECTOR2		m_old_targetpos;

	UPG_INFO*		m_upg_info;

	float			m_ftick_distance;
	int				m_objcnt_num;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CBullet(void);
	virtual ~CBullet(void);
};
