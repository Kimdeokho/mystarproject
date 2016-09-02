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
	const TEXINFO* GetTexture(wstring wstrObjKey
		, wstring wstrStateKey = L""
		, const int& iCnt = 0);

public:
	HRESULT	InsertTexture(wstring wstrFilePath
		, wstring wstrObjKey
		, TEXTYPE eTextype
		, wstring wstrStateKey = L""
		, const int& iCnt = 0);

	void Release(void);

public:
	CTextureMgr(void);
	~CTextureMgr(void);
};
