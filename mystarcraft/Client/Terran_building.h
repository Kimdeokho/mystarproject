#pragma once
#include "obj.h"

class CUI;
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
public:
	UPG_INFO*		m_upg_info;
public:
	CObj*			m_mainbuilding;
	CObj*			m_partbuilding;

	CUI*			m_main_preview;
	CUI*			m_sub_preview;

	CObj*			m_pfire_eff1;
	CObj*			m_pfire_eff2;
	CObj*			m_pfire_eff3;
protected:
	vector<int>		m_areaidx_vec;
	vector<int>		m_old_areaidx_vec;
protected:
	list<PRODUCTION_INFO>	m_production_list;
	bool			m_is_rally;
	D3DXVECTOR2		m_rallypoint;
	vector<D3DXVECTOR2>	m_rallypath;
protected:
	D3DXMATRIX		m_matshadow;
	D3DXVECTOR2		m_vgroundpos;
	D3DXVECTOR2		m_vairpos;
	D3DXVECTOR2		m_weight;//���콺���� ��ŭ �������ִ��� ����ġ
	//�ǹ������並 ���� ���콺��ġ�� 0��,0�� �� �»�ܿ��ִ� 
	//pos�� �߽����� �ű������ �������̴�

	TERRAN_BUILD_TECH m_ebuild_tech;

	int				m_icol;
	int				m_irow;

	bool			m_is_take_off;//�̷����³� �������³�
	bool			m_is_preview;//�̸����⸦ ��� ���� �Ҳ���, �ƴϳ�
	bool			m_is_partinstall; //������ �μӰǹ� ��ġ�Ҳ���?
	bool			m_is_autoinstall;
	bool			m_is_fire_render;




protected:
	float			m_fbuild_tick;
	float			m_build_hp;
protected:

public:
	//void unit_collocate(CObj* const pobj);
	//void rallypoint_pathfinding(void);
	//void add_production_info(const float& maxtime , PRODUCTION_ID eid , const TCHAR* texkey);
	//void update_production(void);
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
	void Build_Complete(void);

	void	upginfo_update(const UPGRADE& eupg);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);

	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Setlink(bool blink , CObj* pobj);

	//virtual void create_unit(PRODUCTION_ID eid);
public:
	CTerran_building(void);
	virtual ~CTerran_building(void);
};
