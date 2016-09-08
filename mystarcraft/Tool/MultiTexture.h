#pragma once
#include "texture.h"

class CMultiTexture :
	public CTexture
{
public:
	map<wstring, vector<TEXINFO*>>		m_MapMulti;

public:
	virtual TEXINFO* GetTexture( const wstring& wstrStateKey = L"" 
		, const int& iCnt = 0 );
	int GetTextureSize( const wstring& wstrStateKey );
	const vector<TEXINFO*>*	GetStateTexture(const wstring& wstrstatekey);
public:
	virtual HRESULT InsertTexture( const wstring& wstrFilePath 
		, const wstring& wstrStateKey = L"" 
		, const int& iCnt = 0 );
	virtual void Release( void );

public:
	CMultiTexture(void);
	virtual ~CMultiTexture(void);
};