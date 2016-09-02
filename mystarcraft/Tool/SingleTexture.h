#pragma once
#include "texture.h"

class CSingleTexture :
	public CTexture
{
private:
	TEXINFO*		m_pTexInfo;

public:
	virtual TEXINFO* GetTexture(wstring wstrStateKey = L""
		, const int& iCnt = 0);

public:
	virtual HRESULT InsertTexture( wstring wstrFilePath 
		, wstring wstrStateKey = L"" 
		, const int& iCnt = 0 );

	virtual void Release( void );

public:
	CSingleTexture(void);
	virtual ~CSingleTexture(void);
};
