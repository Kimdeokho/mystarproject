#include "StdAfx.h"
#include "MultiTexture32.h"

#include "Device.h"
CMultiTexture32::CMultiTexture32(void)
{
}

CMultiTexture32::~CMultiTexture32(void)
{
	Release();
}

HRESULT CMultiTexture32::InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey /*= L"" */, const int& iCnt /*= 0*/)
{
	TCHAR		szFullPath[MAX_PATH] = L"";

	vector<TEXINFO*>		vecTexture;

	int reservecnt = int((iCnt)/DIR_CNT17);
	vecTexture.reserve(reservecnt);

	for(int i = 0; i < DIR_CNT17; ++i)
	{
		m_multiTex[i].insert(make_pair(wstrStateKey , vecTexture));
	}

	map<wstring , vector<TEXINFO*>>::iterator iter;

	for(int i = 0; i < iCnt; ++i)
	{
		iter = m_multiTex[i%DIR_CNT17].find(wstrStateKey);

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

		iter->second.push_back(pTexInfo);
		//vecTexture.push_back(pTexInfo);

	}

	//m_multiTex.insert(map<wstring , vector<TEXINFO*>>::value_type(wstrStateKey , vecTexture));
	return S_OK;
}

void CMultiTexture32::Release(void)
{
	for(int i = 0; i < DIR_CNT17; ++i)
	{
		map<wstring,vector<TEXINFO*>>::iterator iter = m_multiTex[i].begin();
		map<wstring,vector<TEXINFO*>>::iterator iter_end = m_multiTex[i].end();

		for( ; iter != iter_end; ++iter)
		{
			for(size_t j = 0; j < iter->second.size(); ++j)
			{
				iter->second[j]->pTexture->Release();
				Safe_Delete(iter->second[j]);
			}
			iter->second.clear();
		}
		m_multiTex[i].clear();
	}
}

const vector<TEXINFO*>* CMultiTexture32::GetMultiTex32(const wstring& wstrstatekey , const int& diridx)
{
	map<wstring , vector<TEXINFO*>>::iterator iter = m_multiTex[diridx].find(wstrstatekey);

	if(iter == m_multiTex[diridx].end())
		return NULL;
	else
		return &(iter->second);
}
