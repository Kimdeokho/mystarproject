#pragma once

#include "Define.h"

const int MAX_KEY = 255;

class CObj;
class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	bool	m_TurboKeyPress[MAX_KEY];
	bool	m_KeyPress[MAX_KEY];
	bool	m_DbClick_ready[MAX_KEY];
	bool	m_bKeyUp_ready[MAX_KEY];
	float   m_dbClick_Timer[MAX_KEY];

	bool	m_bTurboKeyDown_complete[MAX_KEY];
	bool	m_bOnceKeyDown_complete[MAX_KEY];
	bool	m_bKeyUp_complete[MAX_KEY];
	bool	m_dbclick_complete[MAX_KEY];
	bool	m_combinekey[MAX_KEY];

	int		m_clickCnt[MAX_KEY];
	bool	m_bCombine;

	bool	m_bDbClick_ready;

	int		m_combine_ready[2];
	int		m_combineidx;
	bool	m_bcombinefirst[MAX_KEY];
	bool	m_bcombinesecond[MAX_KEY];


	D3DXVECTOR2	m_downpt;
	D3DXVECTOR2 m_curpt;

	int objcnt;
	int m_nkey;

	bool	m_clickwating[MAX_KEY];
	bool	m_bwork; //미니맵 클릭시 곧바로 이동방지
	bool	m_select; // 단축키+ 클릭시 유닛선택되는거 방지
	bool	m_bdraging;
public:
	void TurboKeyDown(const int& nkey);
	void OnceKeyDown(const int& nkey);
	void CombineKey(const int& firstkey ,const int& secondkey);
	void MouseOnceKeyDown(const int& nkey);
	void MouseKeyUp(const int& nkey);
	void DbClick(const int& nkey);
	void Update(void);
	bool GetLbdraging(void);
public:
	void Intput_turbokey_reaction(void);
	void Intput_oncekey_reaction(void);
	void Intput_keyup_reaction(void);
	void Intput_dbclick_reaction(void);
	void Intput_combine_reaction(void);
public:
	CKeyMgr(void);
	~CKeyMgr(void);
};
