#pragma once

#include "Include.h"


class CInput_Interface;
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

	CInput_Interface*	m_input;
public:
	void TurboKeyDown(const int& nkey);
	void OnceKeyDown(const int& nkey);
	void CombineKey(const int& firstkey ,const int& secondkey);
	void MouseOnceKeyDown(const int& nkey);
	void MouseKeyUp(const int& nkey);
	void DbClick(const int& nkey);
	void Update(void);
	//bool GetLbdraging(void);
public:
	CInput_Interface*	GetInputDevice(void){return m_input;}
	void	SetInput_Device(SCENEID escene);
public:
	bool GetOnceKeyDown_Check(const int& nkey );
	bool GetKeyUp_Check(const int& nkey );
	bool GetTurboKeyDown_Check(const int& nkey );	
	bool GetDbClick_Check(const int& nkey );	
	bool GetCombineKey_Check(const int& firstkey , const int& secondkey);
public:	
public:
	CKeyMgr(void);
	~CKeyMgr(void);
};
