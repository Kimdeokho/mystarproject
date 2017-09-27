#pragma once

#include "Include.h"

class CComponent;

typedef boost::unordered_map<COMPONENT_LIST, CComponent*> COMPONENT_PAIR;
//typedef map<COMPONENT_LIST, CComponent*> COMPONENT_PAIR;

class CObj
{
protected:
	boost::unordered_map<COMPONENT_LIST , CComponent*>	m_componentlist;
	//map<COMPONENT_LIST , CComponent*>	m_componentlist;
protected:
	//transform
	D3DXVECTOR2		m_vPos; //��ũ���� ������ �� ���� 0~4096
	D3DXMATRIX		m_matWorld;	//.41 , 42������ �������� 0~800, 0~600
	D3DXVECTOR2		m_vcurdir;
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

	int				m_oldidx256;
	int				m_curidx256;

	int				m_curidx512;
	int				m_oldidx512;
	
	int				m_obj_id;

private:
	static int		m_globalobj_id;
protected:
	/*ENUM*/

	//��� ������Ʈ ����
	SORT_ID				m_sortID;
	OBJ_POS_KIND		m_ePosKind; /*static�� ��� ����� �ѹ��� �ʱ�ȭ �Ϸ���*/
	TEAM_NUMBER			m_eteamnumber; /*�Ʊ����� �������� �߸�����*/
	CATEGORY			m_ecategory; //�������� �ǹ����� �ڿ����� ���..

	// ����/////////////////////////////////////////
	//MOVE_TYPE			m_eType;           /*������ ��������*/
	//ATTACK_SEARCH_TYPE	m_eAttackType;
	//DAMAGE_TYPE			m_eDamageType;
	//ARMOR_TYPE			m_eArmorType;
	//STATE				m_estate; // ����
	//
	//ORDER				m_eorder; //���� ���
	//float				m_fspeed;
	//int					m_damage;
	//int					m_hp;
	//int					m_mp;
	//���� Ÿ��, �Ƹ�Ÿ�� ��..�̷��͵�
	/*���Ÿ���� �������� �з�
	*/

	UNITINFO			m_unitinfo;
	//�ǹ�
	/*
	hp , ����ð�.. �̷��͵�..
	*/
protected:
	/*struct*/
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
	virtual void Release(void);
public:
	void SetPos(const float x , const float y, OBJ_POS_KIND ekind = STATIC_OBJ);
	void SetPos(const D3DXVECTOR2& vpos);
	virtual void SetDestroy(bool bdestroy);
	void SetSelect(bool bselect);
	void SetSpeed(const float& fspeed);
	void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	void Setdir(D3DXVECTOR2& vdir);
	void Setdiridx(const int& diridx);

	void SetState(STATE estate);
	void SetOrder(ORDER eorder);
	void SetObjID(const int& id);
	void SetTeamNumber(TEAM_NUMBER eteamnum);
	void SetMagicBox(bool bmagicbox);
public:
	const D3DXMATRIX&		GetMat(void);
	int						GetObjID(void);
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
	
	D3DXVECTOR2&			GetReferencePos(void);
	
	const D3DXVECTOR2&		GetcurDir(void);
	int						Getcuridx(const int& tilesize);
	

	//int						GetHP(void);	
	//MOVE_TYPE				GetType(void);
	//ORDER					GetOrder(void);
	//STATE					GetState(void);
	//float*					GetSpeed(void);


	CComponent*				GetComponent(COMPONENT_LIST ecom_name);
	const	UNITINFO&		GetUnitinfo();
public:
	void idx_update(void);
public:
	virtual bool Be_in_camera(void);
public:
	CObj(void);
	virtual ~CObj(void);
};
