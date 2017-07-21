#pragma once

#include "Include.h"

class CObj
{
protected:
	LPD3DXSPRITE			m_pSprite;
	TEXINFO**				m_pGeneralTexture;
	const TCHAR*			m_objname;
	const TCHAR*			m_statename;
protected:
	D3DXMATRIX		m_matWorld;	
	D3DXVECTOR2		m_vRectLine[5];

	MYRECT<float>	m_rect;	
	MYRECT<float>	m_vertex;

	D3DXVECTOR2		m_vPos; //��ũ���� ������ �� ���� 0~4096
	D3DXVECTOR3		m_vTextureCenter;
	
	int				m_curidx32;
	int				m_oldidx32;

	int				m_oldidx64;
	int				m_curidx64;

	int				m_oldidx256;
	int				m_curidx256;

	int				m_curidx512;
	int				m_oldidx512;
	
	int				m_obj_id;
private:
	static int		m_globalobj_id;
protected:
	/*ENUM*/
	OBJ_POS_KIND		m_ePosKind; /*static�� ��� ����� �ѹ��� �ʱ�ȭ �Ϸ���*/
	SORT_ID				m_sortID;
	OBJ_TYPE			m_eType;           /*������ ��������*/
	DISTINCTION			m_ediscrimination; /*�Ʊ����� ��������*/
protected:
	/*struct*/
	FRAME				m_frame;
protected:
	bool			m_bdestroy;
	bool			m_bSelect;
public:
public:
	virtual void Initialize(void);
	virtual void InitialRect(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Release(void);
public:
	virtual void Setstate(const CHAR* statekey);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind = STATIC_OBJ);
	void SetDestroy(bool bdestroy);
	void SetSelect(bool bselect);
	void SetVertex(const float& _left, const float& _right, const float& _top , const float& _bottom);
public:
	float					GetY(void);
	SORT_ID					GetsortID(void);
	OBJ_TYPE				GetType(void);
	DISTINCTION			GetDiscirimination(void);
	bool					GetDestroy(void);
	const MYRECT<float>&	GetMyRect(void);
	const D3DXVECTOR2&		GetPos(void);
	D3DXVECTOR2&			GetReferencePos(void);
	int						GetObjID(void);
public:
	void idx_update(void);
	void idx_release(void);
public:
	bool Be_in_camera(void);
public:
	CObj(void);
	virtual ~CObj(void);
};
