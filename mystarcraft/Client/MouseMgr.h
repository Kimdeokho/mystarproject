#pragma once

#include "Include.h"
class CMouseMgr
{
	DECLARE_SINGLETON(CMouseMgr)
public:
	enum	MOUSE_STATE
	{
		MS_NONE,
		MS_IDLE,
		MS_AIM,
		MS_CLICK_MARK,
		MS_DETECT,
	};
private:
	POINT m_mousept;
	D3DXVECTOR2 m_vmousept;
	D3DXVECTOR2 m_click_pos;

private:
	FRAME						m_frame;
	const vector<TEXINFO*>*		m_tex_set;
	TEXINFO*					m_curtex;
	D3DXVECTOR3					m_vcenter;
	LPD3DXSPRITE				m_pSprite;
	D3DXMATRIX					m_mouse_mat;

	bool						m_is_click_mark;
	float						m_fclick_size;
	D3DXMATRIX					m_click_mat;
	D3DXVECTOR3					m_vclick_center;
	TEXINFO*					m_clicktex;
	float						m_fsize_sign;

	
	MOUSE_STATE					m_cur_ms;
	const TCHAR*				m_mouse_texkey;	
	D3DCOLOR					m_mouse_color;
public:
	void Initialize(void);
	void Lobby_Update(void);
	void Update(void);
	void Render(void);
	void Init_clickpos(void);
	void SetMouseState(const MOUSE_STATE estate);
	void SetMouseTexture(const TCHAR* texkey);
	bool Unitdetect(void);
public:
	 const POINT& GetMousePt(void);
	 const D3DXVECTOR2& GetAddScrollvMousePt(void);
	 const D3DXVECTOR2& GetScreenMousePt(void);
	 const D3DXVECTOR2& GetClick_Pos(void);
	 const int GetMousePt_to_idx(const int& tilesize);
public:
	CMouseMgr(void);
	~CMouseMgr(void);
};
