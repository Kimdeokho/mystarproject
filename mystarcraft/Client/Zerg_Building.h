#pragma once
#include "obj.h"

class CCom_ZBuildingAnim;
class CCom_Creep;
class CUI;
class CZerg_building :
	public CObj
{
protected:
	CCom_ZBuildingAnim*			m_com_anim;
	CCom_Creep*					m_com_creep;
	CUI*						m_miniunit_display;
protected:
	UPG_INFO*		m_upg_info;
protected:
	float			m_fbuild_tick;
	float			m_build_hp;
	int				m_build_maxhp;
protected:
	int				m_icol;
	int				m_irow;

protected:
	bool			m_is_rally;
	D3DXVECTOR2		m_rallypoint;
	vector<D3DXVECTOR2>	m_rallypath;

protected:
	vector<int>		m_areaidx_vec;
	vector<int>		m_old_areaidx_vec;
	D3DXVECTOR2		m_weight;//마우스에서 얼만큼 떨어져있는지 가중치
	//건물프리뷰를 볼때 마우스위치는 0행,0열 즉 좌상단에있다 
	//pos를 중심으로 옮기기위한 보정값이다
public:
	bool					Get_Is_rally(void){return m_is_rally;}
	D3DXVECTOR2				GetRallyPoint(void){ return m_rallypoint;}
	vector<D3DXVECTOR2>&	GetRally_Path(void) {return m_rallypath;}
public:
	void building_area_Initialize(const int& col , const int& row);
	void building_pos_Initialize(const int& col , const int& row);
public:
	//void SetDestroy(void);
	virtual void Build_Complete(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CZerg_building(void);
	virtual ~CZerg_building(void);
};
