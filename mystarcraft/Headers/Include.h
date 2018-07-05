#pragma once

#include "stdafx.h"

extern HWND		g_hWnd;
extern HINSTANCE g_hInst;
extern BYTE		WriteBuffer[];
extern BYTE		ReadBuffer[];

extern WNDPROC	g_OldEditProc1;
extern WNDPROC	g_OldEditProc2;
extern HWND     g_hEdit1;
extern HWND     g_hEdit2;

using namespace std;

#include "Enum.h"
#include "value.h"
#include "Define.h"
#include "Struct.h"
#include "Function.h"