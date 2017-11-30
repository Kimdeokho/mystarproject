#pragma once
#include "bullet.h"

class CTurret_bullet :
	public CBullet
{
private:
	float	m_trail_time;
	float	m_accel;
	float	m_accel2;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CTurret_bullet(const int& ptarget_id , const D3DXVECTOR2& vdesetpos);
	~CTurret_bullet(void);
};
