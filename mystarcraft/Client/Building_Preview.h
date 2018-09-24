#pragma once
#include "ui.h"

class CObj;
class CBuilding_Preview :
	public CUI
{
private:
	bool					m_active;
	//int						m_icol;
	//int						m_irow;
	
	const TEXINFO*			m_Debugtexinfo;
	const vector<TEXINFO*>*	m_preview_tex;

	CObj*					m_pobj;
	D3DXVECTOR2				m_weight;
	D3DXMATRIX				m_tempmat;

	//const TCHAR*			m_obj_key;
	//D3DXVECTOR2				m_vpos;
	PREVIEW_INFO			m_preview_info;
public:
	void SetPos(const D3DXVECTOR2& vpos);
	void SetActive(bool bactive);
	void SetPreviewInfo(const TCHAR* objkey , TERRAN_BUILD_TECH ebuild , const int& icol , const int& irow, MYRECT<float> vtx);
	void SetPreviewInfo(const PREVIEW_INFO& _info);
	bool GetActive(void);
	bool Install_check(void);
	bool Install_check(const PREVIEW_INFO& cur_info);
	const PREVIEW_INFO GetPreviewInfo(void);
	
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CBuilding_Preview(CObj* pobj);
	~CBuilding_Preview(void);
};
