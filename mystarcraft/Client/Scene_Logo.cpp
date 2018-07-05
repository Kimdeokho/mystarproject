#include "StdAfx.h"
#include "Scene_Logo.h"

#include "Loding.h"
#include "TextureMgr.h"
#include "Device.h"
#include "SceneMgr.h"
CScene_Logo::CScene_Logo(void):m_pBackTexture(NULL)
{
	m_pLoding = NULL;
}

CScene_Logo::~CScene_Logo(void)
{
	Release();
}

HRESULT CScene_Logo::Initialize(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	if(CTextureMgr::GetInstance()->Read_LodingImgPath(L"../Data/imgpath/LodingImgPath.txt") )
		ERR_MSG(L"로딩텍스쳐 불러오기 실패");

	if( 800 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Scene" , L"Loding800")->pTexture;
	else if( 640 == BACKBUFFER_SIZEX)
		m_pBackTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Scene" , L"Loding640")->pTexture;

	m_pLoding = new CLoding(CLoding::LODING_BASIC);
	m_pLoding->Initialize();

	return S_OK;
}

void CScene_Logo::Update(void)
{
	//if(m_pLoding->GetLoadingComplete())
		//CSceneMgr::GetInstance()->SetScene(SCENE_STAGE);

	if(m_pLoding->GetLoadingComplete())
		CSceneMgr::GetInstance()->SetScene(SCENE_LOGIN);
}

void CScene_Logo::Render(void)
{
	m_matWorld._41 = 400;
	m_matWorld._42 = 300;
	m_pDevice->GetSprite()->SetTransform(&m_matWorld);
	m_pDevice->GetSprite()->Draw(m_pBackTexture , 
		NULL , &D3DXVECTOR3(400.f , 300.f, 0.f), NULL
		, D3DCOLOR_ARGB(255,255,255,255));

	m_pLoding->Logo_LodingRender();


}

void CScene_Logo::Release(void)
{
	Safe_Delete(m_pLoding);
}
