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
	const map<wstring , vector<TEXINFO*>>* GetMultiTextureSet(const wstring& wstrObjKey, const wstring& wstrKey);
	const vector<TEXINFO*>* GetGeneralTexture(const wstring& wstrObjKey);
	const TEXINFO* GetSingleTexture(const wstring& wstrTexKey
		, const wstring& wstrStateKey = L"");
	int GetTexCnt(const wstring& wstrObjKey
		, const wstring& wstrStateKey = L"");
	const vector<TEXINFO*>* GetStateTexture(const wstring& wstrObjKey , 
		const wstring& wstrStateKey);
public:
	HRESULT	Read_MultiImagePath(const wstring& wstrFilePath);
	HRESULT Read_SingleImagePath( const wstring& wstrFilePath );
	HRESULT Read_GeneralPath( const wstring& wstrFilePath );
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
