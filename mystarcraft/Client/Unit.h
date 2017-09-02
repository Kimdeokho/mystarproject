#pragma once
#include "obj.h"

class CAstar;
class CUnit :
	public CObj
{
protected:
	LPDIRECT3DTEXTURE9	m_curtex;
protected:
protected:
	const vector<TEXINFO*>* 	m_unittexture[DIR_CNT];/*[]�� ����(����)�� ���Ѵ�*/
protected:
	int					m_diridx;
protected:
	//transform ����
	D3DXVECTOR2			m_vcurdir; //���� ����Ű�� �ִ� ����
	float				m_fspeed;
protected:
	//collision
	CObj*				m_collision_target;
	D3DXVECTOR2 vtargetpos;
	D3DXVECTOR2 m_collision_vnormal;
	float		m_search_time;
protected:
	//��ã�� ����
	CAstar*		m_Astar;
	short		m_flowfieldpath[SQ_TILECNTX*SQ_TILECNTY];
	float		m_movetime;
	int			m_igoalidx;
	vector<D3DXVECTOR2>		m_terrainpath;
	int						m_curterrain_pathidx;
	
	D3DXVECTOR2				m_vgoal_clickpos;
	int						m_realpathidx;

	vector<D3DXVECTOR2>		m_realpath;

	D3DXVECTOR2				m_curpos;
	D3DXVECTOR2				m_goalpos;

	bool					m_multithread;
	int						m_stepsize;
	
	float					m_collisionmove_time;
	bool					m_collision_move;

	int						m_arrivalrange;

	D3DXVECTOR2				m_vprepos;
	D3DXVECTOR2				m_voripos;
	
	MYRECT<float>			m_orirect;
	MYRECT<float>			m_prerect;

	int						m_target_oldidx;
	bool					m_pathfind_pause;
protected:
	CObj*					m_attack_target;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Set_texture_statename(const TCHAR* statekey);
public:
	void SetState(STATE estate);
public:

public:
	//�浹����
	void Collision_update(void);
public:
	//�ٸ� Ŭ������ �� ����
	//void FogInitialize(void);
	void FogUpdate(void); 
	void FogRelease(void);
public:
	void Animation_update(void);
public:

public:
	void Search_Update(void);
public:
	void Dir_calculation(D3DXVECTOR2& vnomal);
public:
	//��ã�� ����
	//�ٸ� Ŭ������ ������
	void	Pathfind_start(void);
	short*	getflowfield(void);
	void	PathFinder_Update(void);
	void	make_flowfieldpath(void);
	void	UnitMoving_update(void);
public:
public:
	CUnit(void);
	virtual ~CUnit(void);
};
