#pragma once
#include "texture.h"

class CMultiTexture :
	public CTexture
{
	//상태는 있으나 방향성은 없는것들(스파이더 마인 등등)
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
