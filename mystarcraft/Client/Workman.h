#pragma once
#include "obj.h"

class CUI;
class CWorkman :
	public CObj
{
protected:
	//CComponent*					m_panimation;
	CComponent*		m_com_anim;
	CComponent*		m_com_targetsearch;
	CComponent*		m_com_pathfind;
	CComponent*		m_com_cc;
	CComponent*		m_com_collision;
protected:
	CMD_STATE		m_ecmd_state;
protected:

	CUI*			m_miniunit_display;
	CUI*			m_main_preview;

	CObj*			m_pgas;
	CObj*			m_pgas_fragment;

	CObj*			m_pmineral;
	CObj*			m_pmineral_fragment;

	CObj*			m_charge_building;//�����ִ� �ǹ�

	bool			m_is_preview;
	

	UPG_INFO*		m_upg_info;

	MYRECT<float>	m_preview_vtx;
protected:
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Create_Building(void);
	virtual void Create_Building(CObj* pgas_resorce);
	virtual void Dead(void);
public:
	void Fragment_Pos_update(void);
public:
	void SetMineral_mark(CObj* pmienral);
	void setmineral_fragment(CObj* pmienral_frag);
	bool ismineral_fragment(void);
	void setgas_fragment(CObj* pmienral_frag);
	void destroy_frag(void);

	void SetGas_mark(CObj* pgas);	
	
public:
		bool GetMineral_mark(void);
public:
	CWorkman(void);
	virtual ~CWorkman(void);
};
