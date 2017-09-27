#pragma once
#include "obj.h"

class CCorpse :
	public CObj
{
private:
	D3DXVECTOR3		m_vcenter;
	FRAME			m_frame;
	const vector<TEXINFO*>*	m_generaltex;
	const TEXINFO*	m_curtex;
	const TCHAR*	m_texname;
	const TCHAR*	m_wreckage_name;
	float			m_duration;

	bool			m_bwreckage;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCorpse(const TCHAR* texname, const TCHAR* wreckagename);
	~CCorpse(void);
};
