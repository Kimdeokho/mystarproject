#pragma once
#include "bullet.h"

class CNuclear_Bullet :
	public CBullet
{
private:
	list<CObj*>		m_injurelist;
	float			m_splash_range[3];
	float			m_ftrail_time;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CNuclear_Bullet(const D3DXVECTOR2& vdesetpos);
	~CNuclear_Bullet(void);
};
