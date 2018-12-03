#pragma once
#include "bullet.h"

class CPlague_Bullet :
	public CBullet
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Dead(void);
	virtual void Release(void);
public:
	CPlague_Bullet(const D3DXVECTOR2& vdesetpos);
	~CPlague_Bullet(void);
};
