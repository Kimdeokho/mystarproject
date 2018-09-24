#pragma once
#include "input_interface.h"

#include "value.h"

class CMyCommandList;
class CMyCommand;
class CInput_Stage :
	public CInput_Interface
{
	//t
private:
	CMyCommandList*		m_cmdlist; //공유가 아니라 복사해야한다..

private:
	D3DXVECTOR2	m_downpt;
	D3DXVECTOR2 m_curpt;

	bool	m_clickwating[MAX_KEY];
	bool	m_bwork; //미니맵 클릭시 곧바로 이동방지
	bool	m_select; // 단축키+ 클릭시 유닛선택되는거 방지	

	int			objcnt;
public:
	CMyCommandList*		GetCmdList(void){ return m_cmdlist; }
	void				ClearCmdList(void);	
public:
	void Intput_oncekey_reaction(void);
	void Intput_turbokey_reaction(void);	
	void Intput_keyup_reaction(void);
	void Intput_dbclick_reaction(void);
	void Intput_combine_reaction(void);
public:
	void Initialize(void);
	void Update(void);
	void Reelase(void);
	virtual void		PushCommand(CMyCommand*	pcmd);
public:
	CInput_Stage(void);
	~CInput_Stage(void);
};
