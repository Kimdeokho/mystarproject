#pragma once
#include "bullet.h"

class CGoliath_Bullet :
	public CBullet
{
private:
	D3DXVECTOR2		m_subdir;
	D3DXVECTOR2		m_old_targetpos;
	float	m_trail_time;
	float	m_accel;
	float	m_accel2;

	float	m_sign;
	float	m_dirpower;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CGoliath_Bullet(const int& target_id ,const D3DXVECTOR2& vdesetpos ,const D3DXVECTOR2& subdir);
	~CGoliath_Bullet(void);
};
