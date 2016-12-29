#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"
#include "GeneralTexture.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "MyProSheet.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr(void)
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

const TEXINFO* CTextureMgr::GetSingleTexture( const wstring& wstrObjKey 
									   , const wstring& wstrTexKey /*= L"" */)
{
	/*오브젝트키와 상태키, iCnt에 해당하는 텍스쳐한장을 가져온다*/
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return ((CSingleTexture*)iter->second)->GetSingleTexture(wstrTexKey);
}
const vector<TEXINFO*>* CTextureMgr::GetStateTexture(const wstring& wstrObjKey , 
													 const wstring& wstrStateKey)
{
	/*오브젝트키와 상태키에 해당하는 텍스쳐 셋을 가져온다
	
	사실상 이렇게까지 할 필요는 없을듯싶다.*/
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
	/*여기에서 m_MapTexture에 다 때려박는게아니라
	종류를 따져서 컨테이너를 좀 나눠서 넣어야할듯

	종족_유닛 컨테이너
	종족_건물 컨테이너
	오브젝트_이펙트 컨테이너
	오브젝트_자원 컨테이너
	
	이런식*/

	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end()) /*키값 못찾았을때*/
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
		case TEXTYPE_GENERAL:
			pTexture = new CGeneralTexture;
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
HRESULT CTextureMgr::Read_GeneralPath( const wstring& wstrFilePath )
{
	wifstream LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"../Data/GeneralPath.txt");
		return E_FAIL;
	}
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	MyForm* pMyForm = pMainFrm->m_pMyFormView;
	CMyProSheet* pProSheet = pMyForm->m_pProSheet;

	TCHAR	szKind[MIN_STR] = L"";
	TCHAR	szSystem[MIN_STR] = L"";
	TCHAR	szTexKey[MIN_STR] = L"";
	TCHAR	szCount[MIN_STR] = L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szKind, MIN_STR, L'|');
		LoadFile.getline(szSystem, MIN_STR, L'|');
		LoadFile.getline(szTexKey, MIN_STR, L'|');
		LoadFile.getline(szCount, MIN_STR, L'|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int		iCount = _ttoi(szCount);


		if(!_tcscmp(szSystem , _T("Resource")))
		{
			InsertTexture(szImgPath, szTexKey, TEXTYPE_GENERAL
				, szTexKey, iCount);

			if(!pProSheet->FindStr(szTexKey, 1))
				pProSheet->AddString(szTexKey,1);
		}
	}

	pProSheet->SetCursel(0,0);
	LoadFile.close();

	return S_OK;
}
HRESULT CTextureMgr::Read_SingleImagePath( const wstring& wstrFilePath )
{
	wifstream	LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"../Data/SingleImgPath.txt");
		return E_FAIL;
	}

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	MyForm* pMyForm = pMainFrm->m_pMyFormView;
	CMyProSheet* pProSheet = pMyForm->m_pProSheet;

	TCHAR	szObjKey[MIN_STR] = L"";
	TCHAR	szTextureKey[MIN_STR] = L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, L'|');
		LoadFile.getline(szTextureKey, MIN_STR, L'|');
		LoadFile.getline(szImgPath, MAX_PATH);

		InsertTexture(szImgPath, szObjKey, TEXTYPE_SINGLE
			, szTextureKey);

	}
	LoadFile.close();

	return S_OK;
}

HRESULT CTextureMgr::Read_MultiImagePath( const wstring& wstrFilePath )
{
	wifstream	LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"../Data/ImgPath.txt");
		return E_FAIL;
	}

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	MyForm* pMyForm = pMainFrm->m_pMyFormView;
	CMyProSheet* pProSheet = pMyForm->m_pProSheet;

	TCHAR	szKind[MIN_STR] = L""; //종족,오브젝트,타일 등을 구분한다
	TCHAR	szSystem[MIN_STR] = L""; //유닛인지 건물,이펙트,자원 세부적인걸 구분한다
	TCHAR	szObjKey[MIN_STR] = L"";
	TCHAR	szStateKey[MIN_STR] = L"";
	TCHAR	szCount[MIN_STR] = L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szKind, MIN_STR, L'|');
		LoadFile.getline(szSystem, MIN_STR, L'|');
		LoadFile.getline(szObjKey, MIN_STR, L'|');
		LoadFile.getline(szStateKey, MIN_STR, L'|');
		LoadFile.getline(szCount, MIN_STR, L'|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int		iCount = _ttoi(szCount);


		if(!_tcscmp(szKind , _T("Tile")))
		{
			InsertTexture(szImgPath, szObjKey, TEXTYPE_MULTI
				, szStateKey, iCount);

			if(!pProSheet->FindStr(szObjKey, 0))
				pProSheet->AddString(szObjKey,0);
		}
	}

	pProSheet->SetCursel(0,0);
	LoadFile.close();

	return S_OK;
}

const map<wstring , vector<TEXINFO*>>* CTextureMgr::GetMultiTextureSet(const wstring& wstrObjKey, const wstring& wstrKey)
{
	/* 종류와 유닛인지 건물인지 이런거 따지는 매개변수도 넣어야함*/

	map<wstring , CTexture*>::iterator iter = m_MapTexture.find(wstrObjKey);

	if(iter != m_MapTexture.end())
		return ((CMultiTexture*)iter->second)->GetMultiTextureSet(wstrKey);
	else
		return NULL;
}

const vector<TEXINFO*>* CTextureMgr::GetGeneralTexture(const wstring& wstrObjKey)
{
	map<wstring , CTexture*>::iterator iter = m_MapTexture.find(wstrObjKey);

	if(iter != m_MapTexture.end())
		return ((CGeneralTexture*)iter->second)->GetTexture();
	else
		return NULL;
}



