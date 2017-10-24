#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture(void)
{
}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey /*= L"" */, const int& iCnt /*= 0*/)
{
	TCHAR		szFullPath[MAX_PATH] = L"";
	vector<TEXINFO*>		vecTexture;
	vecTexture.reserve(iCnt);

	for(int i = 0; i < iCnt; ++i)
	{
		/*L"../Texture/Tile/Tile%d.png"*/
		wsprintf(szFullPath, wstrFilePath.c_str(), i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if(FAILED(D3DXGetImageInfoFromFile(szFullPath
			, &pTexInfo->ImgInfo)))
		{
			ERR_MSG(szFullPath);
			return E_FAIL;
		}

		if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice()
			, szFullPath
			, pTexInfo->ImgInfo.Width
			, pTexInfo->ImgInfo.Height
			, pTexInfo->ImgInfo.MipLevels
			, 0
			, pTexInfo->ImgInfo.Format
			, D3DPOOL_MANAGED
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, D3DCOLOR_XRGB(0,0,0)/*투명이 되는 D3DCOLOR 의 값*/
			, &pTexInfo->ImgInfo
			, NULL
			, &pTexInfo->pTexture)))
		{
			return E_FAIL;
		}

		vecTexture.push_back(pTexInfo);
	}

	m_multiTex.insert(map<wstring , vector<TEXINFO*>>::value_type(wstrStateKey , vecTexture));
	return S_OK;
}

void CMultiTexture::Release(void)
{
	for(map<wstring, vector<TEXINFO*>>::iterator iter = m_multiTex.begin();
		iter != m_multiTex.end(); ++iter)
	{
		for(size_t i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->pTexture->Release();
			Safe_Delete(iter->second[i]);
		}
		iter->second.clear();
	}
	m_multiTex.clear();
}

const vector<TEXINFO*>* CMultiTexture::GetTextureSet(const wstring& wstrStateKey)
{
	map<wstring , vector<TEXINFO*>>::iterator iter = m_multiTex.find(wstrStateKey);

	if(m_multiTex.end() != iter)
		return &(iter->second);
	else
		return NULL;
}
