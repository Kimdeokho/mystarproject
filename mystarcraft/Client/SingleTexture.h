#pragma once
#include "texture.h"

class CSingleTexture :
	public CTexture
{
private:
	map<wstring , TEXINFO*>	m_mapTex;
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey = L"", const int& iCnt = 0);

	virtual void Release(void);
public:
	const TEXINFO* GetSingleTexture(const wstring& wstrStateKey);
public:
	CSingleTexture(void);
	~CSingleTexture(void);
};
