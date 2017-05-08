#pragma once

#include "Define.h"

const int MAX_KEY = 255;

class CObj;
class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	bool	m_KeyPress[MAX_KEY];
	bool	m_DbClick_ready[MAX_KEY];
	bool	m_bCombine;

	bool	m_bDbClick_ready;
	bool	m_bKeyUp[MAX_KEY];
	float   m_dbClick_Timer[MAX_KEY];

	int		m_clickCnt[MAX_KEY];

	D3DXVECTOR2	m_downpt;
	D3DXVECTOR2 m_curpt;

	CObj* pObj;
public:
	void TurboKeyDown(const int& nkey);
	void OnceKeyDown(const int& nkey);
	void CombineKey(const int& firstkey ,const int& secondkey);
	void MouseKeyDown(const int& nkey);
	void MouseKeyUp(const int& nkey);
	void DbClick(const int& nkey);
	void Update(void);
public:
	CKeyMgr(void);
	~CKeyMgr(void);
};
