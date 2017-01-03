#pragma once

#include "Enum.h"

class CObj
{
public:
	virtual void Initialize(void);
	virtual void Render(void)PURE;
	virtual void Restore_TIleOption(void){};
public:
	void CObj::InitCollRC(void);
public:
	void DrawRect(void);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind = STATIC_OBJ);
public:
	const RECT& GetRect(void);
public:
	void	SaveInfo(HANDLE h);
public:
	BOOL PtCollCheck(const CPoint& _pt);
protected:
	D3DXMATRIX		m_matWorld;
	RECT			m_collRc;
	RECT			m_vertex;
	
	OBJ_POS_KIND	m_ePosKind; /*static일 경우 행렬을 한번만 초기화 하려고*/
	D3DXVECTOR2		m_vRectPoint[5];

	//저장해야할 목록
	const TCHAR*	m_ObjName;
	D3DXVECTOR2		m_vPos; 
public:
	CObj(void);
	virtual ~CObj(void);
};
