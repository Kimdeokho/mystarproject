#pragma once
#include "bullet.h"

class CSpore_Bullet :
	public CBullet
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Dead(void);
	virtual void Release(void);
public:
	CSpore_Bullet(const int target_id ,const D3DXVECTOR2& vdesetpos);
	~CSpore_Bullet(void);
};
