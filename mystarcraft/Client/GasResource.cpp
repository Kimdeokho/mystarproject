#include "StdAfx.h"
#include "GasResource.h"

#include "TextureMgr.h"
CGasResource::CGasResource(void)
{
}

CGasResource::~CGasResource(void)
{
}

void CGasResource::Initialize(void)
{
	m_rect.left		= m_vPos.x - 64;
	m_rect.right	= m_vPos.x + 64;
	m_rect.top		= m_vPos.y - 32;
	m_rect.bottom	= m_vPos.y + 32;
}
void CGasResource::Update(void)
{

}
void CGasResource::Render(void)
{

}
void CGasResource::Release(void)
{

}
