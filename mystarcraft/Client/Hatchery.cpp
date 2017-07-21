#include "StdAfx.h"
#include "Hatchery.h"

#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "FontMgr.h"
CHatchery::CHatchery(void)
{
}

CHatchery::~CHatchery(void)
{
}

void CHatchery::Initialize(void)
{
	CObj::Initialize();

	m_irange = 13;
	m_offset = m_irange >> 1;

	CZerg_Building::Creep_Initialize(false);

	m_sortID = SORT_GROUND;
}

void CHatchery::Update(void)
{

	CObj::idx_update();

	CZerg_Building::Creep_expansion();
	CZerg_Building::Creep_decrase();


	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , 111 , 400 , 300);
}
void CHatchery::Render(void)
{

}

void CHatchery::Release(void)
{

}