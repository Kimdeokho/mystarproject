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
