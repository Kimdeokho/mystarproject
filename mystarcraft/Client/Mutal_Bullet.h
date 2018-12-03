#pragma once
#include "bullet.h"

class CMutal_Bullet :
	public CBullet
{
private:
	int				m_collcnt;
	CObj*	const	m_pself;
	vector<int>		m_vec_objid;

	int						m_target_cntnum;
	TEAM_NUMBER				m_eteam;
	ATTACK_SEARCH_TYPE		m_attack_type;

	MOVE_TYPE				m_target_movetype;
public:
	CObj* NextTarget_Search(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CMutal_Bullet(const int& target_id ,const D3DXVECTOR2& vdesetpos , CObj* const pobj);
	~CMutal_Bullet(void);
};
