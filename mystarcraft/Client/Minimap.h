#pragma once
#include "ui.h"

class CMinimap :
	public CUI
{
private:
	LPDIRECT3DTEXTURE9	m_MinimapTexture;
	LPDIRECT3DTEXTURE9	m_MinifogTexture;

	list<CUI*>			m_miniunit_display;
	float				m_fupdatetime;

	D3DXVECTOR2			m_vweight;
public:
	void Setminiunit(CUI* pui);
	void SetMinimapCamPos(const D3DXVECTOR2& vmousepos);
	void Minimappos_to_screen(D3DXVECTOR2& vmousept);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CMinimap(void);
	~CMinimap(void);
};
