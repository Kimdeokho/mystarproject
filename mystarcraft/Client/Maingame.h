#pragma once

#include "Include.h"

class CLoding;
class CDevice;
class CFontMgr;
class CMaingame
{
private:
	CLoding*			m_pLoding;
	TCHAR				m_szFPS[MIN_STR];
	float				m_fTime;
	int					m_iFPS;
	CFontMgr*			m_pFont;
	CDevice*			m_pDevice;
public:
	HRESULT Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	void RenderFPS(void);
public:
	CMaingame(void);
	~CMaingame(void);
};
