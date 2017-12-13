#pragma once
#include "bullet.h"

class CVulture_Bullet :
	public CBullet
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CVulture_Bullet(const int& target_id ,const D3DXVECTOR2& vdesetpos);
	~CVulture_Bullet(void);
};
