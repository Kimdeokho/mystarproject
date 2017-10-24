#pragma once
#include "obj.h"

class CTerran_building :
	public CObj
{
protected:
	TEXINFO*						m_curtex;
	TEXINFO*						m_linktex;//�μӰǹ� �����
protected:
	CComponent*		m_com_anim;
	CComponent*		m_com_targetsearch;
	CComponent*		m_com_pathfind;
	CComponent*		m_com_collision;
protected:
	D3DXMATRIX		m_matshadow;
	D3DXVECTOR2		m_vgroundpos;
	D3DXVECTOR2		m_vairpos;


	int				m_icol;
	int				m_irow;
	D3DXVECTOR2		m_weight;
	vector<int>		m_areaidx_vec;
	vector<int>		m_old_areaidx_vec;

	bool			m_is_take_off;//�̷����³� �������³�
	bool			m_is_preview;//�̸����⸦ ��� ���� �Ҳ���, �ƴϳ�
	bool			m_is_partinstall; //������ �μӰǹ� ��ġ�Ҳ���?
	bool			m_is_autoinstall;

	PREVIEW_INFO	m_preview_info;
	PREVIEW_INFO	m_subpreview_info;

	CObj*			m_mainbuilding;
	CObj*			m_partbuilding;
public:
	void building_area_Initialize(const int& col , const int& row);
	void building_pos_Initialize(const int& col , const int& row);
	void building_area_update(void);
	void area_release(void);

	void linktex_initialize(void);
	void linktex_update(void);

	
	void TakeOff(void);
	void Landing_move(D3DXVECTOR2& vpos);
	void SetPartBuilding(CObj* pobj);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);

	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Setlink(bool blink);
public:
	CTerran_building(void);
	virtual ~CTerran_building(void);
};
