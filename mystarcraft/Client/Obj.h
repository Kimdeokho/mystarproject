#pragma once

#include "Include.h"

class CObj
{
protected:
	D3DXMATRIX		m_matWorld;
	OBJ_POS_KIND	m_ePosKind; /*static�� ��� ����� �ѹ��� �ʱ�ȭ �Ϸ���*/
	D3DXVECTOR2		m_vRectLine[5];

	MYRECT<float>	m_rect;	
	D3DXVECTOR2		m_vPos;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind = STATIC_OBJ);
public:
	CObj(void);
	~CObj(void);
};
