#pragma once
#include "bullet.h"

class CParasite_Bullet :
	public CBullet
{
private:
	TEAM_NUMBER		m_eteam;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Dead(void);
	virtual void Release(void);
public:
	CParasite_Bullet(TEAM_NUMBER eteam ,const int& ptarget_id , const D3DXVECTOR2& vdesetpos);
	~CParasite_Bullet(void);
};
