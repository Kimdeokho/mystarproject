#pragma once

#include "Include.h"

class CObj
{
protected:
	LPD3DXSPRITE			m_pSprite;
	//const vector<TEXINFO*>*	m_pGeneralTexture;
	TEXINFO**				m_pGeneralTexture;
protected:
	D3DXMATRIX		m_matWorld;
	OBJ_POS_KIND	m_ePosKind; /*static일 경우 행렬을 한번만 초기화 하려고*/
	D3DXVECTOR2		m_vRectLine[5];

	MYRECT<float>	m_rect;	
	D3DXVECTOR2		m_vPos;
	D3DXVECTOR3		m_vTextureCenter;
	
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind = STATIC_OBJ);
public:
	CObj(void);
	virtual ~CObj(void);
};
