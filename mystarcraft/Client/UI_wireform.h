#pragma once
#include "ui.h"

class CObj;
class CUI_wireform :
	public CUI
{
private:
	bool	m_is_active;
	CObj*	m_objaddress;
public:
	void set_texturekey(const TCHAR* texkey);
	void set_objaddress(CObj* const pobj);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_wireform(void);
	~CUI_wireform(void);
};
