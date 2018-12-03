#pragma once
#include "bullet.h"

class CEnsnare_Bullet :
	public CBullet
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Dead(void);
	virtual void Release(void);
public:
	CEnsnare_Bullet(const D3DXVECTOR2& vdesetpos);
	~CEnsnare_Bullet(void);
};
