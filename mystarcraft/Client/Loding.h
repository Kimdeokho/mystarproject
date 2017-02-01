#pragma once


unsigned int APIENTRY	LodingStart(void* parg);


class CTextureMgr;
class CLoding
{
public:
	enum	LODINGFLAG{LODING_BASIC};
private:
	LODINGFLAG			m_eLodingFlag;
	CRITICAL_SECTION	m_CriticalSection;
	HANDLE				m_Thread;
	TCHAR				m_szPath[MAX_PATH];
	CTextureMgr*		m_pTextureMgr;
private:
	bool				m_bLodingComplete;
public:
	void Initialize(void);
	void Logo_LodingRender(void);
	void Release(void);
public:
	LODINGFLAG			GetLodingFlag(void);
	CRITICAL_SECTION	GetKey(void);
	bool				GetLoadingComplete(void);
public:
	void Load_BaiscTexture(void);
public:
	CLoding(void);
	CLoding(LODINGFLAG eFlag);
	~CLoding(void);
};
