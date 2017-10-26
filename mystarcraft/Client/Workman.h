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
	CComponent*		m_com_collision;
protected:
protected:

	CUI*			m_miniunit_display;

	CObj*			m_pgas;
	CObj*			m_pgas_fragment;

	CObj*			m_pmineral;
	CObj*			m_pmineral_fragment;

	CObj*			m_charge_building;//짓고있는 건물

	bool			m_is_preview;
	CUI*			m_main_preview;
protected:
	TEXINFO*						m_curtex;
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
	CWorkman(void);
	virtual ~CWorkman(void);
};
