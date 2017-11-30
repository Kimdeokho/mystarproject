#pragma once
#include "texture.h"

class CMultiEffTexture :
	public CTexture
{
	//방향성은 있으나 상태가 없는 텍스쳐들
public:
	vector<TEXINFO*>	m_multiefftex[DIR_CNT17];
	//map<wstring , vector<TEXINFO*>*>	m_multiTex;
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const int& iCnt = 0);
	virtual void Release(void);
public:
	const vector<TEXINFO*>* GetMultiEffTex( const int& diridx);
public:
	CMultiEffTexture(void);
	virtual ~CMultiEffTexture(void);
};
