#pragma once
#include "texture.h"

class CGeneralTexture :
	public CTexture
{
private:
	vector<TEXINFO*>	m_vecTex;
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey = L"", const int& iCnt = 0);

	virtual void Release(void);
public:
	CGeneralTexture(void);
	~CGeneralTexture(void);
};
