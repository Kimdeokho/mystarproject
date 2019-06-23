#pragma once
#include "com_animation.h"

class CCom_PartAnim :
	public CCom_Animation
{
private:
	const vector<TEXINFO*>*	m_bigbuild_tex;
	float					m_fbuildtime;
	

	const vector<TEXINFO*>* m_subgeneral_tex;
	TEXINFO*				m_linktex;
	FRAME					m_subframe;
	int						m_playdir;
	bool					m_sub_on;
	
	const TCHAR*			m_link_texkey;

	bool					m_bsighton;
	bool					m_isescape;

	TEXINFO*				m_updateTex;
	TEXINFO*				m_staticTex;

	D3DXVECTOR2				m_staticPos;
	D3DXMATRIX				m_curMat;
public:
	void sub_update(void);
	void play_direction(const int& dir/*1¶Ç´Â -1*/);
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_PartAnim(const TCHAR* objkey , const TCHAR* linktexkey , D3DXMATRIX& objmat );
	~CCom_PartAnim(void);
};
