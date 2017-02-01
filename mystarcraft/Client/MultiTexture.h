#pragma once
#include "texture.h"

class CMultiTexture :
	public CTexture
{
private:
	map<wstring , vector<TEXINFO*>>	m_multiTex;
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0);

	virtual void Release(void);
public:
	const vector<TEXINFO*>* GetTextureSet(const wstring& wstrStateKey);
public:
	CMultiTexture(void);
	~CMultiTexture(void);
};
