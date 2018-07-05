#pragma once

#include "UI.h"


class CUIRoot : public CUI
{
private:
	typedef	list<CUI*>::iterator	UI_ITER;
	list<CUI*>	m_uilist;
private:	
	D3DXVECTOR2	m_vstart;	
	D3DXVECTOR2	m_vend;

	float		m_init_dt;
	float		m_fspeed;

	bool		m_is_entryanim;
	bool		m_is_exitanim;

	bool		m_entry_complete;
	bool		m_exit_complete;
public:
	void	Set_texturekey(const TCHAR* texkey);
	void	SetStartEndPos(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend);
	void	Animation();
	void	Entry_Animation(void);
	void	Exit_Animation(void);
	void	SetEntry(bool bentry);
	void	SetExit(bool bexit);

	bool	GetEntryComplete(void);
	bool	GetExitComplete(void);
	
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual	void Init_State(void);	
	virtual void Release(void);
public:
	CUIRoot(void);
	virtual ~CUIRoot(void);
};
