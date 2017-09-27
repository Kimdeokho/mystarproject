#pragma once
#include "ui.h"

class CUI_Select :
	public CUI
{
private:
	const D3DXMATRIX&	m_objmat;
public:
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Select(const TCHAR* statekey , const D3DXMATRIX& matworld);
	~CUI_Select(void);
};
