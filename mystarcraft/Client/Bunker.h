#pragma once
#include "terran_building.h"

class CBunker :
	public CTerran_building
{
private:
	CComponent*		m_com_transport;
	list<CObj*>		m_unitlist;
public:
	bool UnitEnter_Bunker(CObj* pobj);

public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CBunker(void);
	~CBunker(void);
};
