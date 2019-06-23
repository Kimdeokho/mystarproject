#pragma once
#include "com_animation.h"

class CCom_TBuildingAnim :
	public CCom_Animation
{
private:
	const vector<TEXINFO*>*	m_bigbuild_tex;
	float					m_fbuildtime;

	D3DXVECTOR2				m_vairpos;
	D3DXVECTOR2				m_vgroundpos;

	bool					m_bsighton;
	bool					m_isescape;

	TEXINFO*				m_updateTex;
	TEXINFO*				m_staticTex;

	D3DXVECTOR2				m_staticPos;
	D3DXMATRIX				m_curMat;

	int						m_staticidx;
public:
	void SetAirpos(const D3DXVECTOR2& vpos);
	void SetGroundpos(const D3DXVECTOR2& vpos);
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_TBuildingAnim(const TCHAR* objname , D3DXMATRIX& objmat);
	~CCom_TBuildingAnim(void);
};
