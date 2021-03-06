#pragma once
#include "Workman.h"

class CSCV :
	public CWorkman
{
private:

	D3DXVECTOR2		m_vnormal;
	float			m_fterm_time;
	bool			m_moveon;
	float			m_movetime;

	D3DXVECTOR2		m_vbuild_pos[4];
	int				m_build_idx;

public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey, bool* waitkey);
	virtual bool Input_cmd(const int& firstkey , const int& secondkey);
	virtual void Create_Building(void);
	virtual void Create_Building(CObj* pgas_resorce);
	virtual void Release(void);
	virtual void Dead(void);

	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CSCV(void);
	CSCV(const D3DXVECTOR2& vpos);
	~CSCV(void);
};
