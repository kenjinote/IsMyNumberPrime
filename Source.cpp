#pragma comment(linker,"/opt:nowin98")
#include<windows.h>
#include<math.h>

#define WM_EXITTHREAD (WM_APP+100)

TCHAR szClassName[] = TEXT("MyNumberPrime");

BOOL IsPrime(__int64 nNumber)
{
	if (nNumber % 2 == 0) return FALSE;
	for (__int64 n = 3; n*n <= nNumber; n += 2)
	{
		if (nNumber % n == 0) return FALSE;
	}
	return TRUE;
}

int GetCheckDigit(__int64 nNumber)
{
	__int64 sum = 0;
	for (int n = 1; n <= 11; ++n)
	{
		const __int64 q = (n <= 6) ? n + 1 : n - 5;
		sum += (nNumber % 10) * q;
		nNumber /= 10;
	}
	const __int64 mod11 = sum % 11;
	if (mod11 <= 1) return 0;
	return (int)(11 - mod11);
}

DWORD WINAPI ThreadFunc(LPVOID p)
{
	TCHAR szText[1024];
	__int64 primecount = 0;
	int i = 0;
	for (__int64 nNumber = 0; nNumber <= 99999999999i64; ++nNumber)
	{
		const int nCheck = GetCheckDigit(nNumber);
		if (IsPrime(nNumber * 10 + nCheck)) ++primecount;
		if (nNumber % 1000000000i64 == 0i64)
		{
			wsprintf(szText, TEXT("%d%%"), i);
			SetWindowText((HWND)p, szText);
			++i;
		}			          
	}
	wsprintf(szText, TEXT("‘f”‚Ì”‚Í%I64d"), primecount);
	MessageBox((HWND)p, szText, 0, 0);
	PostMessage((HWND)p, WM_EXITTHREAD, 0, 0);
	ExitThread(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThread = 0;
	static DWORD dwParam;
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(TEXT("BUTTON"), TEXT("ŒvŽZŠJŽn"), WS_CHILD | WS_VISIBLE, 10, 10, 256, 32, hWnd, (HMENU)100, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 100)
		{
			hThread = CreateThread(0, 0, ThreadFunc, (LPVOID)hWnd, 0, &dwParam);
			EnableWindow(GetDlgItem(hWnd, 100), FALSE);
		}
		break;
	case WM_EXITTHREAD:
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		hThread = 0;
		EnableWindow(GetDlgItem(hWnd, 100), TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("MyNumberPrime"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
		);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
