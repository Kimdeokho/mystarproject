#include "StdAfx.h"
#include "GeneralTexture.h"
#include "Device.h"

CGeneralTexture::CGeneralTexture(void)
{
}

CGeneralTexture::~CGeneralTexture(void)
{
	Release();
}
HRESULT CGeneralTexture::InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey /*= L"" */, const int& iCnt /*= 0*/)
{
	TCHAR		szFullPath[MAX_PATH] = L"";
	m_vecTex.reserve(iCnt);

	for(int i = 0; i < iCnt; ++i)
	{
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
			, D3DCOLOR_XRGB(0,255,0)/*투명이 되는 D3DCOLOR 의 값 png 비트여야함 */
			, &pTexInfo->ImgInfo
			, NULL
			, &pTexInfo->pTexture)))
		{
			return E_FAIL;
		}
		m_vecTex.push_back(pTexInfo);
	}

	return S_OK;
}

void CGeneralTexture::Release(void)
{
	for(size_t i = 0; i < m_vecTex.size(); ++i)
	{
		m_vecTex[i]->pTexture->Release();
		Safe_Delete(m_vecTex[i]);
	}
	m_vecTex.clear();
}

const vector<TEXINFO*>* CGeneralTexture::GetGeneralTexture(void)
{
	return &m_vecTex;
}
