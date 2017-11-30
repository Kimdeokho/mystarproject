#pragma once
#include "Effect.h"

class CMultiEff :
	public CEffect
{
private:
	//const vector<TEXINFO*>*	m_multiefftex[DIR_CNT];
	const vector<TEXINFO*>*	m_multiefftex;
	const TCHAR*	m_texkey;
	int				m_curdiridx;
	TEXINFO*						m_curtex;
	D3DXVECTOR3		m_vcenter;

	int				m_cur_loopcnt;
	int				m_end_loopcnt;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CMultiEff(const TCHAR*	texkey , const int& curdiridx , const float& framespped ,const int& loopcnt , SORT_ID esortid);
	~CMultiEff(void);
};
