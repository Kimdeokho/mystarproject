#pragma once

class CKeyMgr;
class CMyCommand;
class CInput_Interface
{
protected:
	CKeyMgr*	m_inst;
public:
	virtual void Initialize(void) = 0;
	virtual void Update(void) = 0;
	virtual void Reelase(void) = 0;
public:
	virtual void PushCommand(CMyCommand*	pcmd){};
public:
	CInput_Interface(void);
	virtual ~CInput_Interface(void);
};
