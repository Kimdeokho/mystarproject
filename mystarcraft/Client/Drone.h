#pragma once
#include "workman.h"

class CComponent;
class CDrone :
	public CWorkman
{
private:	
private:
	/*배열이나 벡터로 만들 생각이다*/
	//CComponent*				    m_comfog;		
	//CComponent*				    m_compathfind;
	CComponent*						m_com_pathfind;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Input_cmd(const int& nkey , bool* waitkey);
	virtual void Input_cmd(const int& firstkey , const int& secondkey);

	virtual void Create_Building(void);
	virtual void Create_Building(CObj* pgas_resorce);
	virtual void Update_Cmdbtn(void);

	virtual void Dead(void);
	virtual void Release(void);
public:
	CDrone(void);
	virtual ~CDrone(void);
};
