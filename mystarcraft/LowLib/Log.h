#pragma once

class CLog
{
public:
	static BOOL	WriteLog(LPTSTR data, ...)
	{
		SYSTEMTIME	SystemTime;
		TCHAR		CurrentDate[32]					= {0,};
		TCHAR		CurrentFileName[MAX_PATH]		= {0,};
		FILE*		FilePtr							= NULL;
		TCHAR		DebugLog[MAX_BUFFER_LENGTH]		= {0,};

		va_list		ap;
		TCHAR		Log[MAX_BUFFER_LENGTH]	= {0,};

		va_start(ap, data);
		_vstprintf_s(Log,_countof(Log), data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);
		_sntprintf_s(CurrentDate, _countof(CurrentDate), _T("%d-%d-%d %d:%d:%d"),
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay, 
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond);

		_sntprintf_s(CurrentFileName, _countof(CurrentFileName), _T("LOG_%d-%d-%d %d.log"), 
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay,
			SystemTime.wHour);

		//FilePtr = _tfopen(CurrentFileName, _T("a"));
		FilePtr = _tfsopen(CurrentFileName ,_T("a") , _SH_DENYNO );
		if (!FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("[%s] %s\n"), CurrentDate, Log);
		_sntprintf_s(DebugLog, _countof(DebugLog), _T("[%s] %s\n"), CurrentDate, Log);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}

	static BOOL	WriteLogNoDate(LPTSTR data, ...)
	{
		SYSTEMTIME	SystemTime;
		TCHAR		CurrentDate[32]					= {0,};
		TCHAR		CurrentFileName[MAX_PATH]		= {0,};
		FILE*		FilePtr							= NULL;
		TCHAR		DebugLog[MAX_BUFFER_LENGTH]		= {0,};

		va_list		ap;
		TCHAR		Log[MAX_BUFFER_LENGTH]	= {0,};

		va_start(ap, data);
		_vstprintf_s(Log, _countof(Log) , data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);
		_sntprintf_s(CurrentDate, _countof(CurrentDate), _T("%d-%d-%d %d:%d:%d"),
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay, 
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond);

		_sntprintf_s(CurrentFileName, _countof(CurrentFileName), _T("LOG_%d-%d-%d %d.log"), 
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay,
			SystemTime.wHour);

		//FilePtr = _tfopen(CurrentFileName, _T("a"));
		FilePtr = _tfsopen(CurrentFileName, _T("a") , _SH_DENYNO);
		if (!FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("%s"), Log);
		_sntprintf_s(DebugLog, _countof(DebugLog), _T("%s"), Log);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}
};
