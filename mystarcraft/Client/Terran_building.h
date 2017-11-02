#pragma once
#include "obj.h"

class CUI;
class CTerran_building :
	public CObj
{
protected:
	TEXINFO*						m_curtex;
	TEXINFO*						m_linktex;//부속건물 연결고리
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

	bool			m_is_take_off;//이륙상태냐 착지상태냐
	bool			m_is_preview;//미리보기를 계속 보게 할꺼냐, 아니냐
	bool			m_is_partinstall; //착지시 부속건물 설치할꺼임?
	bool			m_is_autoinstall;
	bool			m_is_fire_render;

	//PREVIEW_INFO	m_preview_info;
	//PREVIEW_INFO	m_subpreview_info;

	CObj*			m_mainbuilding;
	CObj*			m_partbuilding;

	CUI*			m_main_preview;
	CUI*			m_sub_preview;

	CObj*			m_pfire_eff1;
	CObj*			m_pfire_eff2;
	CObj*			m_pfire_eff3;
public:
	void building_area_Initialize(const int& col , const int& row);
	void building_pos_Initialize(const int& col , const int& row);
	void building_area_update(void);
	void area_release(void);

	void fire_eff_initialize(void);
	void fire_eff_update(void);
	void fire_eff_render(void);

	
	void TakeOff(void);
	void Landing_move(D3DXVECTOR2 vpos);
	void SetPartBuilding(CObj* pobj);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);

	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Setlink(bool blink , CObj* pobj);
public:
	CTerran_building(void);
	virtual ~CTerran_building(void);
};
