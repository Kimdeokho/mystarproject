#pragma once
#include "ui.h"

class CUI_MainInterface :
	public CUI
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_MainInterface::CUI_MainInterface(const TCHAR* statekey);
	~CUI_MainInterface(void);
};
