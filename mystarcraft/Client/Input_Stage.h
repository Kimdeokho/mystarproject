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
	CMyCommandList*		m_cmdlist; //������ �ƴ϶� �����ؾ��Ѵ�..

private:
	D3DXVECTOR2	m_downpt;
	D3DXVECTOR2 m_curpt;

	bool	m_clickwating[MAX_KEY];
	bool	m_bwork; //�̴ϸ� Ŭ���� ��ٷ� �̵�����
	bool	m_select; // ����Ű+ Ŭ���� ���ּ��õǴ°� ����	

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
