#pragma once
#include "bullet.h"

class CGoliath_Bullet :
	public CBullet
{
private:
	D3DXVECTOR2		m_subdir;
	D3DXVECTOR2		m_old_targetpos;
	float			m_fdistance;
	float	m_trail_time;
	float	m_accel;
	float	m_accel2;

	float	m_sign;
	float	m_dirpower;

	DWORD	m_dir;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CGoliath_Bullet(const int& target_id ,const D3DXVECTOR2& vdesetpos , DWORD dir);
	~CGoliath_Bullet(void);
};
