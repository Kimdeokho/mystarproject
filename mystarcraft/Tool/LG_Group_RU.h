#pragma once
#include "Terrain_Group.h"

class CLG_Group_RU :
	public CTerrain_Group
{
public:
	virtual void	MakeTerrain_Group(const int istartidx);
public:
	void	Group_RU_Algorithm(void);
public:
	void	OverlapGroup_L(void);
	void	OverlapGroup_LU(void);
	void	OverlapGroup_RU(void);
	void	OverlapGroup_R(void);
	void	OverlapGroup_RD(void);
	void	OverlapGroup_LD(void);

	void	OverlapSequence_L_0(void);
	void	OverlapSequence_L_1(void);
	void	OverlapSequence_L_2(void);
	void	OverlapSequence_L_3(void);
	void	OverlapSequence_L_4(void);
	void	OverlapSequence_L_5(void);

	void	OverlapSequence_LU_0(void);
	void	OverlapSequence_LU_1(void);
	void	OverlapSequence_LU_2(void);
	void	OverlapSequence_LU_3(void);

	void	OverlapSequence_RU_0(void);
	void	OverlapSequence_RU_1(void);
	void	OverlapSequence_RU_2(void);
	void	OverlapSequence_RU_3(void);

	void	OverlapSequence_RD_0(void);
	void	OverlapSequence_RD_1(void);
	void	OverlapSequence_RD_2(void);
	void	OverlapSequence_RD_3(void);
	void	OverlapSequence_RD_4(void);
	void	OverlapSequence_RD_5(void);

	void	OverlapSequence_LD_0(void);
	void	OverlapSequence_LD_1(void);
	void	OverlapSequence_LD_2(void);
	void	OverlapSequence_LD_3(void);
	void	OverlapSequence_LD_4(void);
	void	OverlapSequence_LD_5(void);

	void	OverlapSequence_R_0(void);
	void	OverlapSequence_R_1(void);
	void	OverlapSequence_R_2(void);
	void	OverlapSequence_R_3(void);
	void	OverlapSequence_R_4(void);
	void	OverlapSequence_R_5(void);
public:
	CLG_Group_RU(void);
	virtual ~CLG_Group_RU(void);
};
