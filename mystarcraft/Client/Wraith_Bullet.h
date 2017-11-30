#pragma once
#include "bullet.h"

class CWraith_Bullet :
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
	CWraith_Bullet(const int& ptarget_id , const D3DXVECTOR2& vdesetpos);
	~CWraith_Bullet(void);
};
