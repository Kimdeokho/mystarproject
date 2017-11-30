#pragma once
#include "texture.h"

class CUnitMultiTexture :
	public CTexture
{
	//���¿� ���⼺�� �ִ°͵�
private:
	//map<wstring , vector<TEXINFO*>>	m_multiTex[DIR_CNT];
	map<wstring , vector<TEXINFO*>*>	m_multiTex;
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCnt = 0);
	virtual void Release(void);
public:
	const vector<TEXINFO*>* GetUnitMultiTex(const wstring& wstrstatekey);
public:
	CUnitMultiTexture(void);
	virtual ~CUnitMultiTexture(void);
};
