// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "Include.h"
#include "Maingame.h"

#define MAX_LOADSTRING 100

// ���� ����:

TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

HINSTANCE g_hInst;								// ���� �ν��Ͻ��Դϴ�.
HWND	g_hWnd;
BYTE WriteBuffer[MAX_BUFFER_LENGTH];
BYTE ReadBuffer[MAX_BUFFER_LENGTH];
WNDPROC    g_OldEditProc1, g_OldEditProc2;
HWND       g_hEdit1, g_hEdit2;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	msg.message = WM_NULL;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	// �⺻ �޽��� �����Դϴ�.

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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   RECT	rc = {0,0,CLINETSIZE_X, CLINETSIZE_Y};

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	//��Ŀ���� ���δٸ����� �޽����� �ԷµǸ� ȣ��ȴ�.

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴��� ���� ������ ���� �м��մϴ�.
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
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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


// �Էµ� ������ �����ϴ� ����Ʈ �����츦 ���� Ŭ������ ���ν���

LRESULT CALLBACK EditSubProc1(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage)
	{
		// Ű�� ������ �߻��Ǵ� �޽�����(WM_CHAR, WM_KEYUP, WM_KEYDOWN)��
		// ����Ʈ �����쿡���� �߻��Ǵ� �޽�����(WM_PASTE, WM_CLEAR, WM_UNDO)��
		// �����ϴ�.

	case WM_CHAR:
	case WM_KEYUP:      
	case WM_KEYDOWN:
	case WM_PASTE:
	case WM_CLEAR:
	case WM_UNDO:
		return 0;
		// �߶󳻱� �޽����� ���� �޽����� �ٲ�ġ�� �մϴ�.
	case WM_CUT:
		iMessage=WM_COPY;
		break;
	}

	return CallWindowProc(g_OldEditProc1,hWnd,iMessage,wParam,lParam);

}

// �Է¹޴� ����Ʈ �����츦 ���� Ŭ������ ���ν���
LRESULT CALLBACK EditSubProc2(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int   nLen1, nLen2;
	TCHAR  szStr[1025];

	switch (iMessage)
	{
	case WM_CHAR:
		// ���͸� ������ ���� �Ҹ��� ������ ����
		// ���Ͱ� ������ �߻��Ǵ� WM_CHAR�޽����� �����ϴ�.
		if (wParam==VK_RETURN)  return 0;
		break;
	case WM_KEYDOWN:
		if (wParam==VK_RETURN)  // ���Ͱ� ������...
		{
			// �� ����Ʈ �����쿡 �Էµ� ���ڿ��� ������ ���ؼ�
			// nLen1�� nLen2������ �����մϴ�.

			nLen1=GetWindowTextLength(g_hEdit1);
			nLen2=GetWindowTextLength(hWnd); // �� �Լ������� hWnd�� hEdit2���� �����ϴ�.

			// �Է������쿡 �Էµ� ���ڿ���  szStr������ �����մϴ�.
			GetWindowText(hWnd, szStr, 1024);

			// �޺κп� ���๮�ڸ� �ٿ��ְ�...
			lstrcat(szStr, L"\r\n");

			// ù��° ����Ʈ �����쿡 Ŀ���� �� ������ ������ �����
			SendMessage(g_hEdit1, EM_SETSEL, nLen1, nLen1);

			// szStr ������ ����� ������ �߰��մϴ�.
			SendMessage(g_hEdit1, EM_REPLACESEL, FALSE, (LPARAM)szStr);

			// �ι�° ����Ʈ �������� ������ "��μ���" �� ��
			SendMessage(hWnd, EM_SETSEL, 0, nLen2);

			// "����" �մϴ�.
			SendMessage(hWnd, WM_CLEAR, 0, 0);

			// �ι�° �����쿡 ��Ŀ���� �ݴϴ�.
			SetFocus(hWnd);

			return 0;
		}
		break;
	}

	return CallWindowProc(g_OldEditProc2,hWnd,iMessage,wParam,lParam);
}












// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
