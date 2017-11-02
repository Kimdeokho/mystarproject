#pragma once
#include "com_animation.h"

class CCom_SupplyAnim :
	public CCom_Animation
{
private:
	const vector<TEXINFO*>*	m_supply_parttex;
	float					m_fbuildtime;

	FRAME					m_partframe;
	TEXINFO*				m_part_tex;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_SupplyAnim(D3DXMATRIX& objmat , float fbuildtime);
	~CCom_SupplyAnim(void);
};
