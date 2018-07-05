#pragma once

#include "value.h"
#include "Input_Interface.h"

class CInput_Stage : public CInput_Interface
{
private:
	D3DXVECTOR2	m_downpt;
	D3DXVECTOR2 m_curpt;

	bool	m_clickwating[MAX_KEY];
	bool	m_bwork; //미니맵 클릭시 곧바로 이동방지
	bool	m_select; // 단축키+ 클릭시 유닛선택되는거 방지	

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
