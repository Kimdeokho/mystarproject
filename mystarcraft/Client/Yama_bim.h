#pragma once
#include "bullet.h"

class CYama_bim :
	public CBullet
{
public:
	SORT_ID			m_hit_sort;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CYama_bim(const int& target_id ,const D3DXVECTOR2& vdesetpos);
	~CYama_bim(void);
};
