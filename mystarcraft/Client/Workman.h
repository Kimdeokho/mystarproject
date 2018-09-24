#pragma once
#include "obj.h"

class CUI;
class CBuilding_Preview;
class CCom_Animation;
class CWorkman :
	public CObj
{
protected:
	//CComponent*					m_panimation;
	CCom_Animation*		m_com_anim;
	CComponent*			m_com_pathfind;
	CComponent*			m_com_cc;
	CComponent*			m_com_collision;
	CComponent*			m_com_worksearch;
	CComponent*			m_com_targetsearch;
	CComponent*			m_com_weapon;
protected:
	CMD_STATE		m_ecmd_state;
protected:

	CUI*						m_miniunit_display;
	CBuilding_Preview*			m_main_preview;

	CObj*			m_pgas;
	CObj*			m_pgas_fragment;

	CObj*			m_pmineral;
	CObj*			m_pmineral_fragment;

	CObj*			m_charge_building;//짓고있는 건물

	bool			m_is_preview;
	

	UPG_INFO*		m_upg_info;

	MYRECT<float>	m_preview_vtx;

	PREVIEW_INFO	m_preview_info;
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
	void SetPreview_info(const TCHAR* objkey , TERRAN_BUILD_TECH ebuild , const int& icol , const int& irow ,MYRECT<float> vtx);	
	void SetPreview_info(PREVIEW_INFO _info);
public:
		bool GetMineral_mark(void);
public:
	CWorkman(void);
	virtual ~CWorkman(void);
};
