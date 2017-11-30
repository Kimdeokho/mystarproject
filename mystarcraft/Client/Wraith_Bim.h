#pragma once
#include "bullet.h"

class CWraith_Bim :
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
	CWraith_Bim(const int& ptarget_id , const D3DXVECTOR2& vdesetpos );
	~CWraith_Bim(void);
};
