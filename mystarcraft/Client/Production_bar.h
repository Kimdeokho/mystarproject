#pragma once
#include "ui.h"

//생산 진행정도를 나타내는 막대바
class CProduction_bar :
	public CUI
{
protected:
	const TEXINFO*	m_backbar;
	float			m_fratioX;
	bool			m_is_active;
public:
	void SetProduction_info(const D3DXVECTOR2& vpos , const float& ratioX);
	void SetActive(bool is_active);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CProduction_bar(void);
	~CProduction_bar(void);
};
