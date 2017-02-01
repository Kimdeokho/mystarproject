#include "StdAfx.h"
#include "SingleTexture.h"
#include "Device.h"
CSingleTexture::CSingleTexture(void)
{
}

CSingleTexture::~CSingleTexture(void)
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey /*= L""*/, const int& iCnt /*= 0*/)
{
	TEXINFO* ptexture;
	ptexture = new TEXINFO;
	ZeroMemory(ptexture, sizeof(TEXINFO));

	if(FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str()
		, &ptexture->ImgInfo)))
	{
		ERR_MSG(wstrFilePath.c_str());
		return E_FAIL;
	}

	//D3DXCreateTextureFromFile();

	if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice()
		, wstrFilePath.c_str()
		, ptexture->ImgInfo.Width
		, ptexture->ImgInfo.Height
		, ptexture->ImgInfo.MipLevels
		, 0
		, ptexture->ImgInfo.Format
		, D3DPOOL_MANAGED
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DCOLOR_XRGB(0,255,0)/*투명이 되는 D3DCOLOR 의 값*/
		, &ptexture->ImgInfo
		, NULL
		, &ptexture->pTexture)))
	{
		return E_FAIL;
	}

	map<wstring , TEXINFO*>::iterator iter = m_mapTex.find(wstrStateKey);
	if(iter == m_mapTex.end())
		m_mapTex.insert(map<wstring , TEXINFO*>::value_type(wstrStateKey , ptexture));
	else
		return E_FAIL;

	return S_OK;
}

void CSingleTexture::Release(void)
{
	for(map<wstring , TEXINFO*>::iterator iter = m_mapTex.begin(); iter != m_mapTex.end(); ++iter)
	{
		iter->second->pTexture->Release();
		Safe_Delete(iter->second);
	}
	m_mapTex.clear();
}

const TEXINFO* CSingleTexture::GetSingleTexture(const wstring& wstrStateKey)
{
	map<wstring , TEXINFO*>::iterator iter = m_mapTex.find(wstrStateKey);

	if(iter != m_mapTex.end())
	{
		return iter->second;
	}
	else
		return NULL;
}
