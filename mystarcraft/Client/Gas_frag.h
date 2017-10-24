#pragma once
#include "obj.h"

class CGas_frag :
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
	CGas_frag(void);
	~CGas_frag(void);
};
