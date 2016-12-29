#pragma once
#include "texture.h"

class CSingleTexture :
	public CTexture
{
private:
	map<wstring, TEXINFO*>		m_mapTexInfo;

public:
	virtual TEXINFO* GetSingleTexture(const wstring& wstrStateKey);

public:
	virtual HRESULT InsertTexture( const wstring& wstrFilePath 
		, const wstring& wstrStateKey = L"" 
		, const int& iCnt = 0 );

	virtual void Release( void );

public:
	CSingleTexture(void);
	virtual ~CSingleTexture(void);
};
