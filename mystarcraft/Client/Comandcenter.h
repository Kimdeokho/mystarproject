#pragma once
#include "terran_building.h"

class CComandcenter :
	public CTerran_building
{
private:
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	CComandcenter(void);
	~CComandcenter(void);
};
