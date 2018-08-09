#pragma once

#define	NO_COPY(CLASSNAME)							\
		private:									\
		CLASSNAME(const CLASSNAME&);				\
		CLASSNAME& operator = (const CLASSNAME&);	\

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)							\
		private:									\
		static CLASSNAME* pInstance;			\
		public:										\
		static CLASSNAME* GetInstance(void);	\
		static void DestroyInstance(void);		\

#define	IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME* CLASSNAME::pInstance = NULL;		\
	CLASSNAME* CLASSNAME::GetInstance(void){	\
	if(pInstance == NULL)					\
	pInstance = new CLASSNAME;			\
	return pInstance;						\
		}											\
		void CLASSNAME::DestroyInstance(void){		\
		if(pInstance != NULL) {					\
		delete pInstance;					\
		pInstance = NULL;					\
		}										\
		}	


#define ERR_MSG(message)	MessageBox(g_hWnd, message, L"System Error", NULL)

#define GETTIME		CTimeMgr::GetInstance()->GetTime()

#define READ_TCP_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(m_Read_TCPBuf, Data);
#define READ_UDP_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(m_Read_UDPBuf, Data);

#define WRITE_TCP_PACKET(PROTOCOL , PACKET , WRITE)\
	CSession_Mgr::GetInstance()->Write_TCP_Packet(PROTOCOL , PACKET , WRITE)
#define WRITEALL_UDP_PACKET(PROTOCOL , PACKET , WRITE)\
	CRoomSession_Mgr::GetInstance()->WriteAll(PROTOCOL , PACKET , WRITE)