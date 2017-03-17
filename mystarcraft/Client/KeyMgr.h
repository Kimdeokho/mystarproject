#pragma once

#include "Define.h"

const int MAX_KEY = 255;

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	bool	m_KeyPress[MAX_KEY];
	bool	m_bCombine;
public:
	void TurboKeyDown(const int& nkey);
	void OnceKeyDown(const int& nkey);
	void CombineKey(const int& firstkey ,const int& secondkey);
	void Update(void);
public:
	CKeyMgr(void);
	~CKeyMgr(void);
};
