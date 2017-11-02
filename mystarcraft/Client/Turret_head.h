#pragma once
#include "Terran_building.h"

class CTurret_head :
	public CTerran_building //부모를 누구로 해야할까
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	CTurret_head(void);
	~CTurret_head(void);
};
