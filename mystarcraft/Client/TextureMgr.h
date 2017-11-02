#pragma once

#include "Include.h"

class CTexture;
class CTextureMgr
{
public:
	DECLARE_SINGLETON(CTextureMgr)

private:
	map<wstring, CTexture*>		m_TileTexture;
	map<wstring, CTexture*>		m_StateTexture; // 방향은 없지만 상태가 있는 텍스쳐들

	map<wstring, CTexture*>		m_MultilEffTex; //방향은 있지만 상태가 없는 텍스쳐들
	map<wstring, CTexture*>		m_GeneralTex;
	map<wstring, CTexture*>		m_SinglelTex;

	map<wstring, CTexture*>		m_ZergTex;
	map<wstring, CTexture*>		m_TerranTex;
	map<wstring, CTexture*>		m_MultiTex;

	map<wstring, CTexture*>		m_MultiTex32; //32방향
public:
public:
	HRESULT	Read_directional_ImgPath(const wstring& wstrFilePath ,TCHAR*	szPath);
	HRESULT	Read_directional32_ImgPath(const wstring& wstrFilePath ,TCHAR*	szPath);
	HRESULT	Read_MultiEff_ImgPath(const wstring& wstrFilePath ,TCHAR*	szPath);
	HRESULT	Read_StateImgPath(const wstring& wstrFilePath ,TCHAR*	szPath);
	HRESULT Read_GeneralImgPath(const wstring& wstrFilePath ,TCHAR*	szPath);
	HRESULT Read_SingleImagePath(const wstring& wstrFilePath,TCHAR*	szPath );
	HRESULT Read_LodingImgPath(const wstring& wstrFilePath);

	bool	Read_Texture(TCHAR*	szPath);
public:
	HRESULT	Insert_TileMultiTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const wstring& wstrStateKey
		, const int& iCnt);

	HRESULT	Insert_StateMultiTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const wstring& wstrStateKey
		, const int& iCnt);

	HRESULT Insert_ZUnitMultiTex( const wstring& wstrFilePath 
		, const wstring& wstrObjKey 
		, const wstring& wstrStateKey /*= L"" */
		, const int& iCnt /*= 0*/ );

	HRESULT Insert_TUnitMultiTex( const wstring& wstrFilePath 
		, const wstring& wstrObjKey 
		, const wstring& wstrStateKey /*= L"" */
		, const int& iCnt /*= 0*/ );

	HRESULT Insert_MultiTex( const wstring& wstrFilePath 
		, const wstring& wstrObjKey 
		, const wstring& wstrStateKey /*= L"" */
		, const int& iCnt /*= 0*/ );

	HRESULT Insert_MultiTex32( const wstring& wstrFilePath 
		, const wstring& wstrObjKey 
		, const wstring& wstrStateKey /*= L"" */
		, const int& iCnt /*= 0*/ );

	HRESULT	Insert_GeneralTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const wstring& wstrStateKey
		, const int& iCnt);

	HRESULT	Insert_SingleTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const wstring& wstrStateKey);

	HRESULT	Insert_MultiEffTex(const wstring& wstrFilePath
		, const wstring& wstrObjKey
		, const int& iCnt);
public:
	const vector<TEXINFO*>*	GetTileTexture_vecset(const wstring& wstrObjkey,	const wstring& wstrStatekey);
	const vector<TEXINFO*>*	GetStateTexture_vecset(const wstring& wstrObjkey,	const wstring& wstrStatekey);
	const vector<TEXINFO*>* GetZUnitTexture(const wstring& wstrobjkey , const wstring& wstrstatekey , const int& diridx);
	const vector<TEXINFO*>* GetTUnitTexture(const wstring& wstrobjkey , const wstring& wstrstatekey , const int& diridx);
	const vector<TEXINFO*>* GetMultiTexture(const wstring& wstrobjkey , const wstring& wstrstatekey , const int& diridx);
	const vector<TEXINFO*>* GetMultiTexture32(const wstring& wstrobjkey , const wstring& wstrstatekey , const int& diridx);
	const TEXINFO*			GetSingleTexture(const wstring& wstrObjKey , const wstring& wstrStateKey);
	const vector<TEXINFO*>*	GetGeneralTexture(const wstring& wstrObjKey);
	const vector<TEXINFO*>* GetMultiEffTexture(const wstring& wstrObjKey , const int& dirdix);
public:
	void Release(void);

private:
	CTextureMgr(void);
public:
	~CTextureMgr(void);
};
