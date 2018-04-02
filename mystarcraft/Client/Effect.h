#pragma once

#include "obj.h"

class CEffect : public CObj
{
protected:
	D3DXVECTOR3		m_vcenter;
	FRAME			m_frame;
	const TCHAR*	m_texname;
	const TCHAR*	m_statekey;
	const TEXINFO*	m_curtex;

	float			m_ftimespeed;
	D3DXVECTOR2		m_vsize;

	D3DCOLOR		m_color;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	//bool GetDestroy(void);
public:
	//void SetPos(const D3DXVECTOR2& vpos);
public:
	CEffect(void);
	virtual ~CEffect(void);
};
