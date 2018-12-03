#include "StdAfx.h"
#include "UnitMultiTexture.h"

#include "Device.h"
CUnitMultiTexture::CUnitMultiTexture(void)
{
}

CUnitMultiTexture::~CUnitMultiTexture(void)
{
	Release();
}

HRESULT CUnitMultiTexture::InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey /*= L"" */, const int& iCnt /*= 0*/)
{
	TCHAR		szFullPath[MAX_PATH] = L"";

	//vector<TEXINFO*>		vecTexture;

	//int reservecnt = int((iCnt)/DIR_CNT);
	//vecTexture.reserve(reservecnt);


	vector<TEXINFO*>*	vecTexture = new vector<TEXINFO*>[DIR_CNT17];

	for(int i = 0; i < DIR_CNT17; ++i)
	{
		vecTexture[i].reserve(iCnt/DIR_CNT17);
	}

	map<wstring , vector<TEXINFO*>>::iterator iter;

	for(int i = 0; i < iCnt; ++i)
	{
		//iter = m_multiTex[i%DIR_CNT].find(wstrStateKey);

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

		vecTexture[i%DIR_CNT17].push_back(pTexInfo);
		//iter->second.push_back(pTexInfo);
		//vecTexture.push_back(pTexInfo);

	}

	m_multiTex.insert(map<wstring , vector<TEXINFO*>*>::value_type(wstrStateKey , vecTexture));
	return S_OK;
}

const vector<TEXINFO*>* CUnitMultiTexture::GetUnitMultiTex(const wstring& wstrstatekey)
{
	map<wstring , vector<TEXINFO*>*>::iterator iter = m_multiTex.find(wstrstatekey);

	if(iter == m_multiTex.end())
		return NULL;
	else
		return (iter->second);
}

void CUnitMultiTexture::Release(void)
{
	map<wstring,vector<TEXINFO*>*>::iterator iter = m_multiTex.begin();
	map<wstring,vector<TEXINFO*>*>::iterator iter_end = m_multiTex.end();
	vector<TEXINFO*> vectex;
	size_t vecsize;
	for( ; iter != iter_end; ++iter)
	{
		for(int i = 0; i < DIR_CNT17; ++i)
		{
			vectex = iter->second[i];
			vecsize = vectex.size();
			for(size_t j = 0; j < vecsize; ++j)
			{
				vectex[j]->pTexture->Release();
				Safe_Delete(vectex[j]);
			}
			vectex.clear();
		}
		delete[]iter->second;
		iter->second = NULL;
	}
	m_multiTex.clear();
}
