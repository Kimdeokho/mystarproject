#pragma once
#include "bullet.h"

class CBattle_bim :
	public CBullet
{
private:
	int			m_diridx;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CBattle_bim(const int& ptarget_id , const D3DXVECTOR2& vdesetpos );
	~CBattle_bim(void);
};
