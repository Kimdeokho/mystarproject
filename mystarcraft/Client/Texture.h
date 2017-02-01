#pragma once

#include "Include.h"

class CTexture
{
public:

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0)PURE;

	virtual void Release(void)PURE;
public:
public:
	CTexture(void);
	virtual ~CTexture(void);
};
