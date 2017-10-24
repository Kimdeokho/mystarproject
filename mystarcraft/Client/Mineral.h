#pragma once
#include "obj.h"

class CMineral :
	public CObj
{
private:
	D3DXMATRIX				m_matshadow;
	const vector<TEXINFO*>*	m_vecTex;
	const CObj*				m_workman;

	int						m_mineral_amount;
	int						m_workmancnt;
	
	
	
public:
	void Setworkman(const CObj* pworkman);
	void decrease_workman(void);
	const CObj*  Getworkman(void);
	void SetMineral_amount(const int& iamount);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CMineral(void);
	~CMineral(void);
};
