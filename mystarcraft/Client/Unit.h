#pragma once
#include "obj.h"

class CUnit :
	public CObj
{
protected:
	//map<wstring , vector<TEXINFO*>>*	m_unitmultiTex;
	UNIT_TYPE			m_eType;
	LPDIRECT3DTEXTURE9	m_curtex;
	list<int>			m_Sightoff_List;
protected:
	bool				m_sightoffsw;
	bool				m_InitSight;
	bool				m_fogsearch[SQ_TILECNTY*SQ_TILECNTX];

	int					m_iSightrange;
	int					m_diridx;
	float				m_fogtime;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void FogInitialize(void);
	void FogUpdate(void);
	void FogRelease(void);
public:
	void Dir_calculation(void);
public:
	CUnit(void);
	~CUnit(void);
};
