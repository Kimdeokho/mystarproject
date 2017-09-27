#pragma once

#include "Include.h"

class CTexture
{
public:

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0){ return S_OK;}

	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const int& iCnt = 0){ return S_OK;}

	virtual void Release(void)PURE;
public:
public:
	CTexture(void);
	virtual ~CTexture(void);
};
