#include "StdAfx.h"
#include "TextureMgr.h"
#include "Texture.h"
#include "MultiTexture.h"
#include "GeneralTexture.h"
#include "SingleTexture.h"
#include "UnitMultiTexture.h"
#include "MultiEffTexture.h"
#include "MultiTexture32.h"
IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr(void)
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}
HRESULT CTextureMgr::Insert_TileMultiTex( const wstring& wstrFilePath 
										 , const wstring& wstrObjKey 
										 , const wstring& wstrStateKey /*= L"" */
										 , const int& iCnt /*= 0*/ )
{
	map<wstring, CTexture*>::iterator	iter = m_TileTexture.find(wstrObjKey);

	if(iter == m_TileTexture.end()) //키값을 못찾았다면
	{
		CTexture*	pTexture = NULL;
		pTexture = new CMultiTexture;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey , iCnt);
		
		m_TileTexture.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);

	return S_OK;
}
HRESULT CTextureMgr::Insert_StateMultiTex( const wstring& wstrFilePath 
										 , const wstring& wstrObjKey 
										 , const wstring& wstrStateKey /*= L"" */
										 , const int& iCnt /*= 0*/ )
{
	map<wstring, CTexture*>::iterator	iter = m_StateTexture.find(wstrObjKey);

	if(iter == m_StateTexture.end()) //키값을 못찾았다면
	{
		CTexture*	pTexture = NULL;
		pTexture = new CMultiTexture;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey , iCnt);

		m_StateTexture.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);

	return S_OK;
}

HRESULT CTextureMgr::Insert_ZUnitMultiTex( const wstring& wstrFilePath 
										 , const wstring& wstrObjKey 
										 , const wstring& wstrStateKey /*= L"" */
										 , const int& iCnt /*= 0*/ )
{
	map<wstring, CTexture*>::iterator	iter = m_ZergTex.find(wstrObjKey);

	if(iter == m_ZergTex.end()) //키값을 못찾았다면
	{
		CTexture*	pTexture = NULL;
		pTexture = new CUnitMultiTexture;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey , iCnt);

		m_ZergTex.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);

	return S_OK;
}
HRESULT CTextureMgr::Insert_TUnitMultiTex( const wstring& wstrFilePath 
										  , const wstring& wstrObjKey 
										  , const wstring& wstrStateKey /*= L"" */
										  , const int& iCnt /*= 0*/ )
{
	map<wstring, CTexture*>::iterator	iter = m_TerranTex.find(wstrObjKey);

	if(iter == m_TerranTex.end()) //키값을 못찾았다면
	{
		CTexture*	pTexture = NULL;
		pTexture = new CUnitMultiTexture;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey , iCnt);

		m_TerranTex.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);

	return S_OK;
}
HRESULT CTextureMgr::Insert_MultiTex(const wstring& wstrFilePath , const wstring& wstrObjKey , const wstring& wstrStateKey /*= L"" */ , const int& iCnt /*= 0*/)
{
	map<wstring, CTexture*>::iterator	iter = m_MultiTex.find(wstrObjKey);

	if(iter == m_MultiTex.end()) //키값을 못찾았다면
	{
		CTexture*	pTexture = NULL;
		pTexture = new CUnitMultiTexture;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey , iCnt);

		m_MultiTex.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);

	return S_OK;
}

HRESULT CTextureMgr::Insert_MultiTex32(const wstring& wstrFilePath , const wstring& wstrObjKey , const wstring& wstrStateKey /*= L"" */ , const int& iCnt /*= 0*/)
{
	map<wstring, CTexture*>::iterator	iter = m_MultiTex32.find(wstrObjKey);

	if(iter == m_MultiTex32.end()) //키값을 못찾았다면
	{
		CTexture*	pTexture = NULL;
		pTexture = new CMultiTexture32;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey , iCnt);

		m_MultiTex32.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);

	return S_OK;
}
HRESULT CTextureMgr::Insert_MultiEffTex(const wstring& wstrFilePath , const wstring& wstrObjKey , const int& iCnt)
{
	map<wstring, CTexture*>::iterator	iter = m_MultilEffTex.find(wstrObjKey);

	if(iter == m_MultilEffTex.end()) //키값을 못찾았다면
	{
		CTexture*	pTexture = NULL;
		pTexture = new CMultiEffTexture;

		pTexture->InsertTexture(wstrFilePath , iCnt);

		m_MultilEffTex.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, iCnt);

	return S_OK;
}
HRESULT CTextureMgr::Insert_GeneralTex(const wstring& wstrFilePath , const wstring& wstrObjKey , const wstring& wstrStateKey , const int& iCnt)
{
	map<wstring, CTexture*>::iterator	iter = m_GeneralTex.find(wstrObjKey);

	if(iter == m_GeneralTex.end())
	{
		CTexture*	pTexture = NULL;
		pTexture = new CGeneralTexture;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey , iCnt);

		m_GeneralTex.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCnt);

	return S_OK;
}

HRESULT CTextureMgr::Insert_SingleTex(const wstring& wstrFilePath , const wstring& wstrObjKey , const wstring& wstrStateKey)
{
	map<wstring, CTexture*>::iterator	iter = m_SinglelTex.find(wstrObjKey);

	if(iter == m_SinglelTex.end())
	{
		CTexture*	pTexture = NULL;
		pTexture = new CSingleTexture;

		pTexture->InsertTexture(wstrFilePath , wstrStateKey);

		m_SinglelTex.insert(map<wstring , CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
		iter->second->InsertTexture(wstrFilePath, wstrStateKey);

	return S_OK;
}



HRESULT CTextureMgr::Read_directional_ImgPath(const wstring& wstrFilePath ,TCHAR*	szPath)
{
	wifstream	LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_MultiImgPath ERROR");
		return E_FAIL;
	}

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

		//if(!_tcscmp(szKind , L"Tile"))
		//	Insert_TileMultiTex(szImgPath, szObjKey, szStateKey, iCount);
		//else if(!_tcscmp(szKind , L"ZERG"))
		//	Insert_ZUnitMultiTex(szImgPath, szObjKey, szStateKey, iCount);

		if(!_tcscmp(szKind , L"ZERG"))
			Insert_ZUnitMultiTex(szImgPath, szObjKey, szStateKey, iCount);
		else if(!_tcscmp(szKind , L"TERRAN"))
			Insert_TUnitMultiTex(szImgPath, szObjKey, szStateKey, iCount);
		else
			Insert_MultiTex(szImgPath, szObjKey, szStateKey, iCount);

		lstrcpy(szPath , szImgPath);
	}

	LoadFile.close();
	return S_OK;
}
HRESULT CTextureMgr::Read_directional32_ImgPath(const wstring& wstrFilePath ,TCHAR* szPath)
{
	wifstream	LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_MultiImgPath32 ERROR");
		return E_FAIL;
	}

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


		Insert_MultiTex32(szImgPath, szObjKey, szStateKey, iCount);

		lstrcpy(szPath , szImgPath);
	}

	LoadFile.close();
	return S_OK;
}
HRESULT CTextureMgr::Read_MultiEff_ImgPath(const wstring& wstrFilePath ,TCHAR* szPath)
{
	wifstream	LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_MultiImgPath ERROR");
		return E_FAIL;
	}

	TCHAR	szKind[MIN_STR] = L""; //종족,오브젝트,타일 등을 구분한다
	TCHAR	szSystem[MIN_STR] = L""; //유닛인지 건물,이펙트,자원 세부적인걸 구분한다
	TCHAR	szObjKey[MIN_STR] = L"";
	TCHAR	szCount[MIN_STR] = L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, L'|');
		LoadFile.getline(szCount, MIN_STR, L'|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int		iCount = _ttoi(szCount);

		//if(!_tcscmp(szKind , L"Tile"))
		//	Insert_TileMultiTex(szImgPath, szObjKey, szStateKey, iCount);
		//else if(!_tcscmp(szKind , L"ZERG"))
		//	Insert_ZUnitMultiTex(szImgPath, szObjKey, szStateKey, iCount);
		Insert_MultiEffTex(szImgPath , szObjKey , iCount);


		lstrcpy(szPath , szImgPath);
	}

	return S_OK;
}
HRESULT CTextureMgr::Read_GeneralImgPath(const wstring& wstrFilePath , TCHAR*	szPath)
{
	wifstream LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_GeneralImgPath ERROR");
		return E_FAIL;
	}

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


		Insert_GeneralTex(szImgPath, szTexKey
			, szTexKey, iCount);

		lstrcpy(szPath , szImgPath);

	}

	LoadFile.close();
	return S_OK;
}

HRESULT CTextureMgr::Read_SingleImagePath(const wstring& wstrFilePath , TCHAR*	szPath)
{
	wifstream LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_SingleImagePath ERROR");
		return E_FAIL;
	}

	TCHAR	szObjKey[MIN_STR] = L"";
	TCHAR	szTextureKey[MIN_STR] = L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, L'|');
		LoadFile.getline(szTextureKey, MIN_STR, L'|');
		LoadFile.getline(szImgPath, MAX_PATH);


		Insert_SingleTex(szImgPath, szObjKey
			, szTextureKey);

		lstrcpy(szPath , szImgPath);

	}

	LoadFile.close();
	return S_OK;
}
HRESULT CTextureMgr::Read_LodingImgPath(const wstring& wstrFilePath)
{
	wifstream LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_SingleImagePath ERROR");
		return E_FAIL;
	}

	TCHAR	szObjKey[MIN_STR] = L"";
	TCHAR	szTextureKey[MIN_STR] = L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, L'|');
		LoadFile.getline(szTextureKey, MIN_STR, L'|');
		LoadFile.getline(szImgPath, MAX_PATH);


		Insert_SingleTex(szImgPath, szObjKey
			, szTextureKey);
	}

	LoadFile.close();
	return S_OK;
}
HRESULT CTextureMgr::Read_StateImgPath(const wstring& wstrFilePath ,TCHAR* szPath)
{
	wifstream	LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_MultiImgPath ERROR");
		return E_FAIL;
	}

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

		if(!_tcscmp(szKind , L"Tile"))
			Insert_TileMultiTex(szImgPath, szObjKey, szStateKey, iCount);
		else
			Insert_StateMultiTex(szImgPath, szObjKey, szStateKey, iCount);



		lstrcpy(szPath , szImgPath);
	}

	LoadFile.close();

	return S_OK;
}

const vector<TEXINFO*>* CTextureMgr::GetTileTexture_vecset(const wstring& wstrObjey, const wstring& wstrStatekey)
{
	/*키값과 상태값은 있으나 방향이 없다*/
	map<wstring , CTexture*>::iterator iter = m_TileTexture.find(wstrObjey);
	if(m_TileTexture.end() != iter)
	{
		/*키 값을 찾았다.*/
		CTexture* pTexture = iter->second;

		return ((CMultiTexture*)pTexture)->GetTextureSet(wstrStatekey);
	}
	else
		return NULL;
}
const vector<TEXINFO*>* CTextureMgr::GetStateTexture_vecset(const wstring& wstrObjey, const wstring& wstrStatekey)
{
	/*키값과 상태값은 있으나 방향이 없다*/
	map<wstring , CTexture*>::iterator iter = m_StateTexture.find(wstrObjey);
	if(m_StateTexture.end() != iter)
	{
		/*키 값을 찾았다.*/
		CTexture* pTexture = iter->second;

		return ((CMultiTexture*)pTexture)->GetTextureSet(wstrStatekey);
	}
	else
		return NULL;
}
const vector<TEXINFO*>* CTextureMgr::GetZUnitTexture(const wstring& wstrobjkey , const wstring& wstrstatekey)
{
	map<wstring , CTexture*>::iterator iter = m_ZergTex.find(wstrobjkey);

	if(m_ZergTex.end() != iter)
	{
		/*키 값을 찾았다.*/
		CTexture* pTexture = iter->second;

		return ((CUnitMultiTexture*)pTexture)->GetUnitMultiTex(wstrstatekey);
	}
	else
		return NULL;
}
const vector<TEXINFO*>* CTextureMgr::GetTUnitTexture(const wstring& wstrobjkey , const wstring& wstrstatekey )
{
	map<wstring , CTexture*>::iterator iter = m_TerranTex.find(wstrobjkey);

	if(m_TerranTex.end() != iter)
	{
		/*키 값을 찾았다.*/
		CTexture* pTexture = iter->second;

		return ((CUnitMultiTexture*)pTexture)->GetUnitMultiTex(wstrstatekey);
	}
	else
		return NULL;
}
const vector<TEXINFO*>* CTextureMgr::GetMultiTexture(const wstring& wstrobjkey , const wstring& wstrstatekey )
{
	map<wstring , CTexture*>::iterator iter = m_MultiTex.find(wstrobjkey);

	if(m_MultiTex.end() != iter)
	{
		/*키 값을 찾았다.*/
		CTexture* pTexture = iter->second;

		return ((CUnitMultiTexture*)pTexture)->GetUnitMultiTex(wstrstatekey );
	}
	else
		return NULL;
}

const vector<TEXINFO*>* CTextureMgr::GetMultiTexture32(const wstring& wstrobjkey , const wstring& wstrstatekey )
{
	map<wstring , CTexture*>::iterator iter = m_MultiTex32.find(wstrobjkey);

	if(m_MultiTex32.end() != iter)
	{
		/*키 값을 찾았다.*/
		CTexture* pTexture = iter->second;

		return ((CMultiTexture32*)pTexture)->GetMultiTex32(wstrstatekey);
	}
	else
		return NULL;
}
const TEXINFO* CTextureMgr::GetSingleTexture(const wstring& wstrObjKey , const wstring& wstrStateKey)
{
	map<wstring , CTexture*>::iterator iter = m_SinglelTex.find(wstrObjKey);
	if(iter != m_SinglelTex.end())
	{
		CTexture* pTexture = iter->second;

		return ((CSingleTexture*)pTexture)->GetSingleTexture(wstrStateKey);
	}
	else
		return NULL;
}
const vector<TEXINFO*>* CTextureMgr::GetGeneralTexture(const wstring& wstrObjKey)
{
	map<wstring , CTexture*>::iterator iter = m_GeneralTex.find(wstrObjKey);
	if(iter != m_GeneralTex.end())
	{
		CTexture* pTexture = iter->second;

		return ((CGeneralTexture*)pTexture)->GetGeneralTexture();
	}
	else
		return NULL;
}
const vector<TEXINFO*>* CTextureMgr::GetMultiEffTexture(const wstring& wstrObjKey , const int& dirdix)
{
	map<wstring , CTexture*>::iterator iter = m_MultilEffTex.find(wstrObjKey);
	if(iter != m_MultilEffTex.end())
	{
		CTexture* pTexture = iter->second;

		return ((CMultiEffTexture*)pTexture)->GetMultiEffTex(dirdix);
	}
	else
		return NULL;
}

bool CTextureMgr::Read_Texture(TCHAR*	szPath)
{
	if(Read_SingleImagePath(L"../Data/imgpath/SingleImgPath.txt" , szPath) )
		ERR_MSG(L"싱글텍스쳐 불러오기 실패");

	if(Read_GeneralImgPath(L"../Data/imgpath/GeneralImgPath.txt" , szPath) )
		ERR_MSG(L"일반텍스쳐 불러오기 실패");

	if(Read_StateImgPath(L"../Data/imgpath/StateImgPath.txt" , szPath) )
		ERR_MSG(L"방향성은 없지만 상태가 있는 텍스쳐 불러오기 실패");

	if(Read_directional_ImgPath(L"../Data/imgpath/directionalImgPath.txt" , szPath) )
		ERR_MSG(L"유닛텍스쳐 불러오기 실패");

	if(Read_directional32_ImgPath(L"../Data/imgpath/directional32ImgPath.txt" , szPath) )
		ERR_MSG(L"32방향텍스쳐 불러오기 실패");

	if(Read_MultiEff_ImgPath(L"../Data/imgpath/MultiEff_ImgPath.txt" , szPath) )
		ERR_MSG(L"멀티이펙텍스쳐 불러오기 실패");

	lstrcpy(szPath , L"로딩완료");

	return true;
	//종족별 멀티텍스쳐는 게임 시작전에 따로 부르기

}


void CTextureMgr::Release( void )
{
	for(map<wstring, CTexture*>::iterator iter = m_TileTexture.begin();
		iter != m_TileTexture.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_TileTexture.clear();

	for(map<wstring, CTexture*>::iterator iter = m_StateTexture.begin();
		iter != m_StateTexture.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_StateTexture.clear();


	for(map<wstring, CTexture*>::iterator iter = m_SinglelTex.begin();
		iter != m_SinglelTex.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_SinglelTex.clear();

	for(map<wstring, CTexture*>::iterator iter = m_GeneralTex.begin();
		iter != m_GeneralTex.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_GeneralTex.clear();

	for(map<wstring, CTexture*>::iterator iter = m_ZergTex.begin();
		iter != m_ZergTex.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_ZergTex.clear();

	for(map<wstring, CTexture*>::iterator iter = m_TerranTex.begin();
		iter != m_TerranTex.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_TerranTex.clear();

	if(!m_MultiTex.empty())
	{
		for(map<wstring, CTexture*>::iterator iter = m_MultiTex.begin();
			iter != m_TerranTex.end(); ++iter)
		{
			::Safe_Delete(iter->second);
		}
		m_MultiTex.clear();
	}

	for(map<wstring, CTexture*>::iterator iter = m_MultiTex32.begin();
		iter != m_MultiTex32.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_MultiTex32.clear();

	for(map<wstring, CTexture*>::iterator iter = m_MultilEffTex.begin();
		iter != m_MultilEffTex.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_MultilEffTex.clear();
}



