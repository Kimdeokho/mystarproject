#pragma once
#include "obj.h"

class CFog_object :
	public CObj
{
private:
	int			m_irange;

	float		m_curtime;
	float		m_fogtime;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CFog_object(const int& irange , const float& fogtime , const D3DXVECTOR2& vpos ,const MOVE_TYPE emovetype ,const TEAM_NUMBER eteamnum);
	~CFog_object(void);
};
