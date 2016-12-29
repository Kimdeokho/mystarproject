#pragma once
#include "texture.h"

class CGeneralTexture :
	public CTexture
{
public:
	const vector<TEXINFO*>* GetTexture(const wstring& wstrStateKey = L"");
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0);

	virtual void Release(void);
private:
	vector<TEXINFO*>		m_vecTex;
public:
	CGeneralTexture(void);
	~CGeneralTexture(void);
};
