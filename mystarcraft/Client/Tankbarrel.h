#pragma once
#include "unit.h"

CComponent;
class CTankbarrel :
	public CUnit
{
private:
	CObj*	m_tankbody;
	CComponent* m_com_weapon;
	bool	m_bsiegemode;
	bool	m_btransform_ready;
	D3DXVECTOR2	m_vbarrelpos;

	CComponent*	m_com_siegeanim;
	CComponent*	m_com_tankanim;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	bool	GetTransformReady(void);
	void	SetTransformReady(bool btransform);
	void	TransformSiegebarrel(void);
	void	TransformTankbarrel(void);
	void    SetbarrelPos(const D3DXVECTOR2& vpos , const D3DXVECTOR2& vbarrelpos);
	const D3DXVECTOR2&	GetbarrelPos(void);
public:
	CTankbarrel(void);
	CTankbarrel(CObj* tankbody);
	~CTankbarrel(void);
};
