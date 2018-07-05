// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Include.h"
#include "Maingame.h"

#define MAX_LOADSTRING 100

// 전역 변수:

TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

HINSTANCE g_hInst;								// 현재 인스턴스입니다.
HWND	g_hWnd;
BYTE WriteBuffer[MAX_BUFFER_LENGTH];
BYTE ReadBuffer[MAX_BUFFER_LENGTH];
WNDPROC    g_OldEditProc1, g_OldEditProc2;
HWND       g_hEdit1, g_hEdit2;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);





void StartDebug() 
{ 
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	msg.message = WM_NULL;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	// 기본 메시지 루프입니다.

	StartDebug();

	CMaingame	maingame;

	maingame.Initialize();
	while (msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			maingame.Update();
			maingame.Render();
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   RECT	rc = {0,0,CLINETSIZE_X, CLINETSIZE_Y};

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

   //hWnd = CreateWindow(szWindowClass, szTitle, WS_EX_TOPMOST | WS_POPUP,
	  // 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

   g_hWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	//포커스가 서로다르더라도 메시지가 입력되면 호출된다.

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴의 선택 영역을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:		
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		//SetWindowLong(g_hEdit2,GWL_WNDPROC,(LONG)g_OldEditProc2);
		//SetWindowLong(g_hEdit1,GWL_WNDPROC,(LONG)g_OldEditProc1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// 입력된 내용을 저장하는 에디트 윈도우를 서브 클래싱한 프로시저

LRESULT CALLBACK EditSubProc1(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage)
	{
		// 키가 눌리면 발생되는 메시지들(WM_CHAR, WM_KEYUP, WM_KEYDOWN)과
		// 에디트 윈도우에서만 발생되는 메시지들(WM_PASTE, WM_CLEAR, WM_UNDO)을
		// 막습니다.

	case WM_CHAR:
	case WM_KEYUP:      
	case WM_KEYDOWN:
	case WM_PASTE:
	case WM_CLEAR:
	case WM_UNDO:
		return 0;
		// 잘라내기 메시지는 복사 메시지로 바꿔치기 합니다.
	case WM_CUT:
		iMessage=WM_COPY;
		break;
	}

	return CallWindowProc(g_OldEditProc1,hWnd,iMessage,wParam,lParam);

}

// 입력받는 에디트 윈도우를 서브 클래싱한 프로시저
LRESULT CALLBACK EditSubProc2(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int   nLen1, nLen2;
	TCHAR  szStr[1025];

	switch (iMessage)
	{
	case WM_CHAR:
		// 엔터를 누를때 나는 소리를 잠재우기 위해
		// 엔터가 눌릴때 발생되는 WM_CHAR메시지는 막습니다.
		if (wParam==VK_RETURN)  return 0;
		break;
	case WM_KEYDOWN:
		if (wParam==VK_RETURN)  // 엔터가 눌리면...
		{
			// 두 에디트 윈도우에 입력된 문자열의 갯수를 구해서
			// nLen1과 nLen2변수에 저장합니다.

			nLen1=GetWindowTextLength(g_hEdit1);
			nLen2=GetWindowTextLength(hWnd); // 이 함수내에서 hWnd는 hEdit2값과 같습니다.

			// 입력윈도우에 입력된 문자열을  szStr변수에 저장합니다.
			GetWindowText(hWnd, szStr, 1024);

			// 뒷부분에 개행문자를 붙여주고...
			lstrcat(szStr, L"\r\n");

			// 첫번째 에디트 윈도우에 커서를 맨 끝으로 가도록 만든뒤
			SendMessage(g_hEdit1, EM_SETSEL, nLen1, nLen1);

			// szStr 변수에 저장된 내용을 추가합니다.
			SendMessage(g_hEdit1, EM_REPLACESEL, FALSE, (LPARAM)szStr);

			// 두번째 에디트 윈도우의 내용을 "모두선택" 한 뒤
			SendMessage(hWnd, EM_SETSEL, 0, nLen2);

			// "삭제" 합니다.
			SendMessage(hWnd, WM_CLEAR, 0, 0);

			// 두번째 윈도우에 포커스를 줍니다.
			SetFocus(hWnd);

			return 0;
		}
		break;
	}

	return CallWindowProc(g_OldEditProc2,hWnd,iMessage,wParam,lParam);
}












// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
