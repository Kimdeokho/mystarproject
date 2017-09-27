#include "StdAfx.h"
#include "MultiEffTexture.h"

#include "Device.h"
CMultiEffTexture::CMultiEffTexture(void)
{
}

CMultiEffTexture::~CMultiEffTexture(void)
{
	Release();
}

HRESULT CMultiEffTexture::InsertTexture(const wstring& wstrFilePath , const int& iCnt /*= 0*/)
{
	TCHAR		szFullPath[MAX_PATH] = L"";


	int reservecnt = int((iCnt)/DIR_CNT);

	for(int i = 0; i < DIR_CNT; ++i)
	{
		m_multiefftex[i].reserve(reservecnt);
	}

	map<wstring , vector<TEXINFO*>>::iterator iter;

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

		m_multiefftex[i%DIR_CNT].push_back(pTexInfo);
		//vecTexture.push_back(pTexInfo);

	}

	//m_multiTex.insert(map<wstring , vector<TEXINFO*>>::value_type(wstrStateKey , vecTexture));
	return S_OK;
}

void CMultiEffTexture::Release(void)
{
	for(int i = 0; i < DIR_CNT; ++i)
	{
		for( size_t j = 0; j < m_multiefftex[i].size(); ++j)
		{
			m_multiefftex[i][j]->pTexture->Release();
			Safe_Delete(m_multiefftex[i][j]);
		}
		m_multiefftex[i].clear();
	}
}

const vector<TEXINFO*>* CMultiEffTexture::GetMultiEffTex(const int& diridx)
{
	return &(m_multiefftex[diridx]);
}
