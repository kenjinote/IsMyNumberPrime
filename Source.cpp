#include<windows.h>
#include<math.h>

#define WM_EXITTHREAD (WM_APP+100)

TCHAR szClassName[] = TEXT("CountPrimeNumber");

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
	wsprintf(szText, TEXT("マイナンバーに含まれる素数の数は%I64dです。"), primecount);
	MessageBox((HWND)p, szText, 0, 0);
	PostMessage((HWND)p, WM_EXITTHREAD, 0, 0);
	ExitThread(0);
}

BOOL getprime(unsigned char *prime_table, __int64 number)
{
	const __int64 index = (number / 6i64) * 2i64 - ((number % 6i64 == 1) ? 1i64 : 0i64);
	return (prime_table[index / 8i64] >> (index % 8)) & 1;
}

VOID setprime(unsigned char *prime_table, __int64 number)
{
	const __int64 index = (number / 6i64) * 2i64 - ((number % 6i64 == 1i64) ? 1i64 : 0i64);
	prime_table[index / 8i64] |= 1 << (index % 8);
}

DWORD WINAPI ThreadFunc2(LPVOID p)
{
	__int64 i, k, m, n, t;
	unsigned char * prime_table = (unsigned char *)GlobalAlloc(GPTR, 41666666667i64);
	if (prime_table == 0) { MessageBox((HWND)p, TEXT("メモリが足りません"),0,0); goto EXIT0; }
	TCHAR szText[1024];
	for (i = 6i64; i <= 999999i64; i += 6i64)
	{
		m = i - 1;
		n = i + 1;
		if (!getprime(prime_table, m))
		{
			for (k = m * m; ; k += 6 * m)
			{
				if (k > 999999999999i64) break;
				setprime(prime_table, k);
				t = k + 2 * m;
				if (t > 999999999999i64) break;
				setprime(prime_table, t);
			}
		}
		if (!getprime(prime_table, n))
		{
			for (k = n * n; k <= 999999999999i64; k += 6 * n)
			{
				if (k > 999999999999i64) break;
				setprime(prime_table, k);
				t = k + 4 * n;
				if (t > 999999999999i64) break;
				setprime(prime_table, t);
			}
		}
	}
	__int64 primecount = 0; // 2, 3もマイナンバーではない
	for (i = 6; i <= 999999999999i64; i += 6)
	{
		m = i - 1;
		n = i + 1;
		if (!getprime(prime_table, m) && GetCheckDigit(m / 10i64) == m % 10i64) {
			++primecount;
		}
		if (!getprime(prime_table, n) && GetCheckDigit(n / 10i64) == n % 10i64) {
			++primecount;
		}
	}
	GlobalFree(prime_table);
	wsprintf(szText, TEXT("マイナンバーに含まれる素数の数は%I64dです。"), primecount);
	MessageBox((HWND)p, szText, 0, 0);
EXIT0:
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
		CreateWindow(TEXT("BUTTON"), TEXT("計算開始1"), WS_CHILD | WS_VISIBLE, 10, 10, 256, 32, hWnd, (HMENU)100, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		CreateWindow(TEXT("BUTTON"), TEXT("計算開始2"), WS_CHILD | WS_VISIBLE, 10, 50, 256, 32, hWnd, (HMENU)101, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 100)
		{
			hThread = CreateThread(0, 0, ThreadFunc, (LPVOID)hWnd, 0, &dwParam);
			EnableWindow(GetDlgItem(hWnd, 100), FALSE);
			EnableWindow(GetDlgItem(hWnd, 101), FALSE);
		}
		else if (LOWORD(wParam) == 101)
		{
			hThread = CreateThread(0, 0, ThreadFunc2, (LPVOID)hWnd, 0, &dwParam);
			EnableWindow(GetDlgItem(hWnd, 100), FALSE);
			EnableWindow(GetDlgItem(hWnd, 101), FALSE);
		}
		break;
	case WM_EXITTHREAD:
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		hThread = 0;
		EnableWindow(GetDlgItem(hWnd, 100), TRUE);
		EnableWindow(GetDlgItem(hWnd, 101), TRUE);
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
		TEXT("マイナンバーに含まれる素数を数える"),
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
	return (int)msg.wParam;
}
