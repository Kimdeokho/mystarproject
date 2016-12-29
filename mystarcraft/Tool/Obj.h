#pragma once

#include "Enum.h"

class CObj
{
public:
	virtual void Initialize(void);
	virtual void Render(void)PURE;
	virtual void InitRect(void){};
	virtual void Restore_TIleOption(void){};
public:
	void DrawRect(void);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind);
public:
	const RECT& GetRect(void);
public:
	BOOL PtCollCheck(const CPoint& _pt);
protected:
	D3DXMATRIX		m_matWorld;
	D3DXVECTOR2		m_vPos;
	RECT			m_collRc;
	CRect			tets;
	const TCHAR*	m_TexName;
	OBJ_POS_KIND	m_ePosKind; /*static일 경우 행렬을 한번만 초기화 하려고*/
	D3DXVECTOR2		m_vRectPoint[5];
public:
	CObj(void);
	virtual ~CObj(void);
};
