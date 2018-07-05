#pragma once
#include "obj.h"

class CMineral :
	public CObj
{
private:
	D3DXMATRIX				m_matshadow;
	const vector<TEXINFO*>*	m_vecTex;
	//const CObj*				m_workman;
	list<CObj*>				m_workman_list;

	int						m_mineral_amount;
	int						m_workmancnt;
public:
	bool Setworkman(CObj* pworkman);
	void SetMineral_amount(const int& iamount);
	
	bool Getworkman(CObj* pworkman);
	unsigned	 Getworkman_count(void);

	bool Check_workman(CObj* pworkman);
	bool decrease_workman(CObj* pworkman);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CMineral(void);
	~CMineral(void);
};
