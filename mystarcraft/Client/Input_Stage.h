#pragma once

#include "value.h"
#include "Input_Interface.h"

class CInput_Stage : public CInput_Interface
{
private:
	D3DXVECTOR2	m_downpt;
	D3DXVECTOR2 m_curpt;

	bool	m_clickwating[MAX_KEY];
	bool	m_bwork; //�̴ϸ� Ŭ���� ��ٷ� �̵�����
	bool	m_select; // ����Ű+ Ŭ���� ���ּ��õǴ°� ����	

	int			objcnt;
public:
	void Intput_turbokey_reaction(void);
	void Intput_oncekey_reaction(void);
	void Intput_keyup_reaction(void);
	void Intput_dbclick_reaction(void);
	void Intput_combine_reaction(void);
public:
	void Initialize(void);
	void Update(void);
	void Reelase(void);
public:
	CInput_Stage(void);
	~CInput_Stage(void);
};
