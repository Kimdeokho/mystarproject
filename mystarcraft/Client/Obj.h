#pragma once

#include "Include.h"

class CObj
{
protected:
	LPD3DXSPRITE			m_pSprite;
	TEXINFO**				m_pGeneralTexture;
protected:
	D3DXMATRIX		m_matWorld;
	OBJ_POS_KIND	m_ePosKind; /*static일 경우 행렬을 한번만 초기화 하려고*/
	D3DXVECTOR2		m_vRectLine[5];

	MYRECT<float>	m_rect;	
	D3DXVECTOR2		m_vPos; //스크롤이 더해진 값 범위 0~4096
	D3DXVECTOR3		m_vTextureCenter;
	
	int				m_curidx32;
	int				m_oldidx32;

	int				m_oldidx64;
	int				m_curidx64;
	
	int				m_oldidx512;
	int				m_curidx512;

	int				m_obj_id;
	enum SORT_ID	m_sortID;
public:
	bool			m_bdestroy;
public:
public:
	virtual void Initialize(void);
	virtual void InitialRect(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind = STATIC_OBJ);
	void SetObj_ID(const int& id);
	void SetDestroy(bool bdestroy);
public:
	float GetY(void);
	SORT_ID	GetsortID(void);
	bool  GetDestroy(void);
public:
	void idx_update(void);
public:
	bool Be_in_camera(void);
public:
	CObj(void);
	virtual ~CObj(void);
};
