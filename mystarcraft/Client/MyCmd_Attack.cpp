#include "StdAfx.h"
#include "MyCmd_Attack.h"

#include "UnitMgr.h"
#include "Obj.h"
CMyCmd_Attack::CMyCmd_Attack(void)
{
}

CMyCmd_Attack::~CMyCmd_Attack(void)
{
}

void CMyCmd_Attack::Write(CStreamSP& writestream)
{

}

void CMyCmd_Attack::Read(CStreamSP& readstream)
{

}

void CMyCmd_Attack::Progress(void)
{

}

CMyCmd_Attack* CMyCmd_Attack::StaticCreate(const D3DXVECTOR2& vpt_arrive)
{
	CMyCmd_Attack*	pcmd = new CMyCmd_Attack;

	list<CObj*>* unitlist = CUnitMgr::GetInstance()->Getcur_unitlist();

	/*
	���õ� ���ּ�
	�� ���ֵ��� ��ȣ,
	Ÿ���õ� ���� ��ȣ
	��������
	*/

	return pcmd;
}
