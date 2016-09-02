#pragma once
#include "texture.h"

class CMultiTexture :
	public CTexture
{
public:
	map<wstring, vector<TEXINFO*>>		m_MapMulti;

public:
	virtual TEXINFO* GetTexture( wstring wstrStateKey = L"" 
		, const int& iCnt = 0 );

public:
	virtual HRESULT InsertTexture( wstring wstrFilePath 
		, wstring wstrStateKey = L"" 
		, const int& iCnt = 0 );
	virtual void Release( void );

public:
	CMultiTexture(void);
	virtual ~CMultiTexture(void);
};