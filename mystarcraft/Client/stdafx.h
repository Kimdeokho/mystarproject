// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <process.h>
#include <map>
#include <vector>
#include <list>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <string>
#include <d3dx9.h>
#include <d3d9.h>
#include <shlwapi.h>
#include <fstream>

#include <crtdbg.h>

#ifdef _DEBUG 
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__) 
#endif