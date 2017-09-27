#pragma once
#include "obj.h"

class CAstar;


class CUI;
class CUnit :
	public CObj
{
protected:
	//CComponent*					m_panimation;
	CComponent*		m_com_anim;
protected:
	D3DXMATRIX	m_matshadow;
protected:
	//const vector<TEXINFO*>* 	m_unittexture[DIR_CNT];/*[]�� ����(����)�� ���Ѵ�*/
protected:
	//int					m_diridx;
protected:
	//CComponent*		m_com_multitex;
	//CComponent*		m_com_shadowtex;
	//CComponent*		m_com_generaltex;
	//CComponent*		m_com_singletex;
	CUI*			m_select_ui;
	CUI*			m_miniunit_display;

protected:
	TEXINFO*						m_curtex;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Set_texture_statename(const TCHAR* statekey);
public:
public:

public:
	//�浹����
	void Collision_update(void);
public:
public:
	void Animation_update(void);
public:

public:
	void Search_Update(void);
public:
	void Dir_calculation(D3DXVECTOR2& vnomal);
public:
	//��ã�� ����
	//�ٸ� Ŭ������ ������
	void	Pathfind_start(void);
	short*	getflowfield(void);
	void	PathFinder_Update(void);
	void	UnitMoving_update(void);
public:
public:
	CUnit(void);
	virtual ~CUnit(void);
};
