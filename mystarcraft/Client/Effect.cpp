#include "StdAfx.h"
#include "Effect.h"

#include "Device.h"
CEffect::CEffect(void)
{
	//D3DXMatrixIdentity(&m_matWorld);
	m_texname = L"";
	m_statekey = L"";
	//m_pSprite = CDevice::GetInstance()->GetSprite();
	m_curtex = NULL;
}

CEffect::~CEffect(void)
{
}

void CEffect::Initialize(void)
{

}

void CEffect::Update(void)
{

}

void CEffect::Render(void)
{

}

void CEffect::Release(void)
{

}
