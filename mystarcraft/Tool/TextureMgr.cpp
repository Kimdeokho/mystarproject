#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr(void)
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

const TEXINFO* CTextureMgr::GetTexture( wstring wstrObjKey 
									   , wstring wstrStateKey /*= L"" */
									   , const int& iCnt )
{
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return iter->second->GetTexture(wstrStateKey, iCnt);
}



HRESULT CTextureMgr::InsertTexture( wstring wstrFilePath 
								   , wstring wstrObjKey 
								   , TEXTYPE eTextype 
								   , wstring wstrStateKey /*= L"" */
								   , const int& iCnt /*= 0*/ )
{
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
	{
		CTexture*	pTexture = NULL;

		switch(eTextype)
		{
		case TEXTYPE_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEXTYPE_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED(pTexture->InsertTexture(wstrFilePath
			, wstrStateKey, iCnt)))
		{
			return E_FAIL;
		}

		m_MapTexture.insert(make_pair(wstrObjKey, pTexture));
	}

	return S_OK;
}

void CTextureMgr::Release( void )
{
	for(map<wstring, CTexture*>::iterator iter = m_MapTexture.begin();
		iter != m_MapTexture.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_MapTexture.clear();
}

