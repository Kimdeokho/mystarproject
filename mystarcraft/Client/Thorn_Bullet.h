#pragma once
#include "bullet.h"

class CThorn_Bullet :
	public CBullet
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Dead(void);
	virtual void Release(void);	
public:
	CThorn_Bullet(void);
	~CThorn_Bullet(void);
};
