#pragma once

#include "Include.h"

class CComponent;

typedef boost::unordered_map<COMPONENT_LIST, CComponent*> COMPONENT_PAIR;
//typedef map<COMPONENT_LIST, CComponent*> COMPONENT_PAIR;

class CUI;
class CObj
{
protected:
	boost::unordered_map<COMPONENT_LIST , CComponent*>	m_componentlist;

	CUI*			m_select_ui;
	CUI*			m_energybar_ui;
protected:
	//transform
	D3DXVECTOR2		m_vPos; //스크롤이 더해진 값 범위 0~4096
	D3DXVECTOR2		m_vcurdir;
	D3DXMATRIX		m_matWorld;	//.41 , 42성분은 렌더범위 0~800, 0~600
	
	int				m_curdiridx;
	bool			m_bmagicbox;
protected:
	//collision
	D3DXVECTOR2		m_vRectLine[5];
	MYRECT<float>	m_rect;	
	MYRECT<float>	m_vertex;
protected:
	LPD3DXSPRITE			m_pSprite;
protected:
	
	int				m_curidx32;
	int				m_oldidx32;

	int				m_oldidx64;
	int				m_curidx64;

	int				m_obj_id;

private:
	static int		m_globalobj_cnt;
	int				m_objcnt;
protected:
	/*ENUM*/

	//모든 오브젝트 공용
	SORT_ID				m_sortID;
	OBJ_POS_KIND		m_ePosKind; /*static일 경우 행렬을 한번만 초기화 하려고*/
	TEAM_NUMBER			m_eteamnumber; /*아군인지 적군인지 중립인지*/
	CATEGORY			m_ecategory; //유닛인지 건물인지 자원인지 등등..
	OBJID				m_eOBJ_NAME;	

	UNITINFO			m_unitinfo;

protected:
	bool			m_bdestroy;
	bool			m_bSelect;
public:
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey , bool* waitkey){ return false; };
	virtual bool Input_cmd(const int& firstkey , const int& secondkey){ return false; };
	
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);

	virtual void Dead(void);
	virtual void Release(void);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind = STATIC_OBJ);
	void SetPos(const D3DXVECTOR2& vpos);
	
	void SetSelect(SELECT_FLAG eflag , D3DCOLOR ecolor = D3DCOLOR_ARGB(255,0,255,255));
	void SetSpeed(const float& fspeed);
	void SetMP(const int& imp);
	
	void Setdir(D3DXVECTOR2& vdir);
	void Setdiridx(const int& diridx);
	void SetActive(bool is_active);

	void SetState(STATE state);
	void SetOrder(ORDER order);
	void SetObjID(const int id);
	void SetObjName(OBJID eobjname);
	void SetTeamNumber(TEAM_NUMBER eteamnum);
	void SetMagicBox(bool bmagicbox);
	void SetDestroy(bool bdestroy);

	void SetHP(const int& ihp);
	void SetMyrect(MYRECT<float> temprc);
	void SetVertex(MYRECT<float> tempvtx);

	void AddComponent(const COMPONENT_LIST ecomname, CComponent*	pcom);
public:
	const D3DXMATRIX&		GetMat(void);
	int						GetObjNumber(void);
	int						GetObjCountNumber(void);
	CATEGORY				GetCategory(void);
	float					GetX(void);
	float					GetY(void);
	SORT_ID					GetsortID(void);	
	TEAM_NUMBER				GetTeamNumber(void);
	bool					GetDestroy(void);
	const MYRECT<float>&	GetMyRect(void);
	const MYRECT<float>&	GetVertex(void);
	const D3DXVECTOR2&		GetPos(void);
	const D3DXVECTOR2*		GetPosAdress(void);
	OBJID					GetOBJNAME(void);
	
	D3DXVECTOR2&			GetReferencePos(void);
	
	const D3DXVECTOR2&		GetcurDir(void);
	const int&				Getcuridx(const int& tilesize);

	virtual CComponent*		GetComponent(COMPONENT_LIST ecom_name);
	const	UNITINFO&		GetUnitinfo();
	UNITINFO&				Get_ref_Unitinfo(){return m_unitinfo;}
public:
	void unit_area_Initialize(void);
	void area_update(void);
	void area_release(void);


public:
	virtual bool Be_in_camera(void);
public:
	CObj(void);
	virtual ~CObj(void);
};
