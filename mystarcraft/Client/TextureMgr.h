#pragma once

#include "Include.h"

class CTexture;
class CTextureMgr
{
public:
	DECLARE_SINGLETON(CTextureMgr)

private:
	map<wstring, CTexture*>		m_TileTexture;

	map<wstring, CTexture*>		m_GeneralTex;
	map<wstring, CTexture*>		m_SinglelTex;

public:
public:
	HRESULT	Read_MultiImgPath(const wstring& wstrFilePath ,TCHAR*	szPath);
	HRESULT Read_GeneralImgPath(const wstring& wstrFilePath ,TCHAR*	szPath);
	HRESULT Read_SingleImagePath(const wstring& wstrFilePath,TCHAR*	szPath );
	HRESULT Read_LodingImgPath(const wstring& wstrFilePath);
	bool	Read_Texture(TCHAR*	szPath);
public:
	HRESULT	Insert_TileMultiTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const wstring& wstrStateKey
		, const int& iCnt);

	HRESULT	Insert_GeneralTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const wstring& wstrStateKey
		, const int& iCnt);

	HRESULT	Insert_SingleTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const wstring& wstrStateKey);
public:
	const vector<TEXINFO*>*	GetTileTexture_vecset(const wstring& wstrObjey,
		const wstring& wstrStatekey);
	const TEXINFO*			GetSingleTexture(const wstring& wstrObjKey , const wstring& wstrStateKey);
	const vector<TEXINFO*>*	GetGeneralTexture(const wstring& wstrObjKey);
public:
	void Release(void);

private:
	CTextureMgr(void);
public:
	~CTextureMgr(void);
};
