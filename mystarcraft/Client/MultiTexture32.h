#pragma once
#include "texture.h"

class CMultiTexture32 :
	public CTexture
{
	//방향이 32방향인 텍스쳐
private:
	map<wstring , vector<TEXINFO*>*>	m_multiTex;
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0);
	virtual void Release(void);
public:
	const vector<TEXINFO*>* GetMultiTex32(const wstring& wstrstatekey);
public:
	CMultiTexture32(void);
	~CMultiTexture32(void);
};
