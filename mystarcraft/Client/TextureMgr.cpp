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

const TEXINFO* CTextureMgr::GetTexture( const wstring& wstrObjKey 
									   , const wstring& wstrStateKey /*= L"" */
									   , const int& iCnt )
{
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return iter->second->GetTexture(wstrStateKey, iCnt);
}
const vector<TEXINFO*>* CTextureMgr::GetStateTexture(const wstring& wstrObjKey , 
										 const wstring& wstrStateKey)
{
	map<wstring , CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return ((CMultiTexture*)(iter->second))->GetStateTexture(wstrStateKey);
}
int CTextureMgr::GetTexCnt( const wstring& wstrObjKey , const wstring& wstrStateKey /*= L""*/ )
{
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return ((CMultiTexture*)iter->second)->GetTextureSize(wstrStateKey);
}


HRESULT CTextureMgr::InsertTexture( const wstring& wstrFilePath 
								   , const wstring& wstrObjKey 
								   , TEXTYPE eTextype 
								   , const wstring& wstrStateKey /*= L"" */
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
	else
	{
		if(TEXTYPE_MULTI == eTextype)
			iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);
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

HRESULT CTextureMgr::ReadImagePath( const wstring& wstrFilePath )
{
	wifstream	LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"../Data/ImgPath.txt");
		return E_FAIL;
	}

	TCHAR	szKind[MIN_STR] = L""; //종족인지 타일인지
	TCHAR	szSystem[MIN_STR] = L""; //유닛인지 건물인지 텍스쳐 STL 따로 관리하려고
	TCHAR	szObjKey[MIN_STR] = L"";
	TCHAR	szStateKey[MIN_STR] = L"";
	TCHAR	szCount[MIN_STR] = L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, L'|');
		LoadFile.getline(szStateKey, MIN_STR, L'|');
		LoadFile.getline(szCount, MIN_STR, L'|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int		iCount = _ttoi(szCount);

		InsertTexture(szImgPath, szObjKey, TEXTYPE_MULTI
			, szStateKey, iCount);

	}
	LoadFile.close();

	return S_OK;
}



