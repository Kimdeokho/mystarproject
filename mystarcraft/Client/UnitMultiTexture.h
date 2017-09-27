#pragma once
#include "texture.h"

class CUnitMultiTexture :
	public CTexture
{
	//상태와 방향성이 있는것들
private:
	map<wstring , vector<TEXINFO*>>	m_multiTex[DIR_CNT];
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0);
	virtual void Release(void);
public:
	const vector<TEXINFO*>* GetUnitMultiTex(const wstring& wstrstatekey , const int& diridx);
public:
	CUnitMultiTexture(void);
	virtual ~CUnitMultiTexture(void);
};
