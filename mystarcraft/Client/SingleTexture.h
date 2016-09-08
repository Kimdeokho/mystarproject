#pragma once
#include "texture.h"

class CSingleTexture :
	public CTexture
{
private:
	TEXINFO*		m_pTexInfo;

public:
	virtual TEXINFO* GetTexture(const wstring& wstrStateKey = L""
		, const int& iCnt = 0);

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath 
		, const wstring& wstrStateKey = L"" 
		, const int& iCnt = 0 );

	virtual void Release( void );

public:
	CSingleTexture(void);
	virtual ~CSingleTexture(void);
};
