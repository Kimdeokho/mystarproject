#pragma once

#include "Include.h"

class CTexture
{
public:
	virtual TEXINFO* GetTexture(const wstring& wstrStateKey = L""
		, const int& iCnt = 0) PURE;

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0) PURE;

	virtual void Release(void) PURE;

public:
	CTexture(void);
	virtual ~CTexture(void);
};
