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
	/*������ƮŰ�� ����Ű, iCnt�� �ش��ϴ� �ؽ��������� �����´�*/
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return ((CSingleTexture*)iter->second)->GetSingleTexture(wstrTexKey);
}
const vector<TEXINFO*>* CTextureMgr::GetStateTexture(const wstring& wstrObjKey , 
													 const wstring& wstrStateKey)
{
	/*������ƮŰ�� ����Ű�� �ش��ϴ� �ؽ��� ���� �����´�
	
	��ǻ� �̷��Ա��� �� �ʿ�� ������ʹ�.*/
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
	/*���⿡�� m_MapTexture�� �� �����ڴ°Ծƴ϶�
	������ ������ �����̳ʸ� �� ������ �־���ҵ�

	����_���� �����̳�
	����_�ǹ� �����̳�
	������Ʈ_����Ʈ �����̳�
	������Ʈ_�ڿ� �����̳�
	
	�̷���*/

	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end()) /*Ű�� ��ã������*/
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

	TCHAR	szKind[MIN_STR] = L""; //����,������Ʈ,Ÿ�� ���� �����Ѵ�
	TCHAR	szSystem[MIN_STR] = L""; //�������� �ǹ�,����Ʈ,�ڿ� �������ΰ� �����Ѵ�
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
	/* ������ �������� �ǹ����� �̷��� ������ �Ű������� �־����*/

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



