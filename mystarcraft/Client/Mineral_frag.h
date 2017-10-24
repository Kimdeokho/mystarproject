#pragma once
#include "obj.h"

class CMineral_frag :
	public CObj
{
private:
	const vector<TEXINFO*>*	m_vecTex;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CMineral_frag(void);
	~CMineral_frag(void);
};
