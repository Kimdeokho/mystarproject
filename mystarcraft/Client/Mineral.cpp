#include "StdAfx.h"
#include "Mineral.h"

#include "TextureMgr.h"
CMineral::CMineral(void)
{
}

CMineral::~CMineral(void)
{
}

void CMineral::Initialize(void)
{
	m_rect.left		= m_vPos.x - 32;
	m_rect.right	= m_vPos.x + 32;
	m_rect.top		= m_vPos.y - 16;
	m_rect.bottom	= m_vPos.y + 16;
}
void CMineral::Update(void)
{

}
void CMineral::Render(void)
{

}
void CMineral::Release(void)
{

}