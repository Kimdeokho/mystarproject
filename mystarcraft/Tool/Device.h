#pragma once

#include "Include.h"

class CDevice
{
public:
	DECLARE_SINGLETON(CDevice)

private:
	LPDIRECT3D9			m_p3D;
	LPDIRECT3DDEVICE9	m_pDevice;

	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;

public:
	LPDIRECT3DDEVICE9 GetDevice(void);
	LPD3DXSPRITE GetSprite(void);
	LPD3DXFONT GetFont(void);
	LPD3DXLINE GetLine(void);

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp);

public:
	HRESULT InitDevice(void);
	void Release(void);
	void Render_Begin(void);
	void Render_End(void);

public:
	~CDevice(void);

private:
	CDevice(void);
};
