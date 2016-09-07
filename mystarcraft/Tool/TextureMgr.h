#pragma once

#include "Include.h"

class CTexture;
class CTextureMgr
{
public:
	DECLARE_SINGLETON(CTextureMgr)

private:
	map<wstring, CTexture*>		m_MapTexture;

public:
	const TEXINFO* GetTexture(const wstring& wstrObjKey
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0);
	int GetTexCnt(const wstring& wstrObjKey
		, const wstring& wstrStateKey = L"");

public:
	HRESULT	ReadImagePath(const wstring& wstrFilePath);
	HRESULT	InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, TEXTYPE eTextype
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0);

	void Release(void);

public:
	CTextureMgr(void);
	~CTextureMgr(void);
};
