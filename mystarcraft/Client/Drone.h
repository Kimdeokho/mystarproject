#pragma once
#include "unit.h"

class CComponent;
class CDrone :
	public CUnit
{
private:	
private:
	/*�迭�̳� ���ͷ� ���� �����̴�*/
	//CComponent*				    m_comfog;		
	//CComponent*				    m_compathfind;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Release(void);
public:
	CDrone(void);
	virtual ~CDrone(void);
};
