#include "StdAfx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture(void)
: m_pTexInfo(NULL)
{
}

CSingleTexture::~CSingleTexture(void)
{
	Release();
}

HRESULT CSingleTexture::InsertTexture( const wstring& wstrFilePath 
								   , const wstring& wstrStateKey /*= L"" */
								   , const int& iCnt /*= 0 */ )
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if(FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str()
		, &m_pTexInfo->ImgInfo)))
	{
		ERR_MSG(wstrFilePath.c_str());
		return E_FAIL;
	}

	//D3DXCreateTextureFromFile();

	if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice()
		, wstrFilePath.c_str()
		, m_pTexInfo->ImgInfo.Width
		, m_pTexInfo->ImgInfo.Height
		, m_pTexInfo->ImgInfo.MipLevels
		, 0
		, m_pTexInfo->ImgInfo.Format
		, D3DPOOL_MANAGED
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, NULL
		, &m_pTexInfo->ImgInfo
		, NULL
		, &m_pTexInfo->pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release( void )
{
	m_pTexInfo->pTexture->Release();
	Safe_Delete(m_pTexInfo);
}

TEXINFO* CSingleTexture::GetTexture( const wstring& wstrStateKey /*= L"" */, const int& iCnt /*= 0*/ )
{
	return m_pTexInfo;
}
