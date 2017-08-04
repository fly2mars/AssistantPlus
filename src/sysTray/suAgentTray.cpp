// QuickCodePage.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <Engines.h>
//#include "suAgentHelper.h"
#include "..\ServerSet.h "

#define _SUcountof(x) (sizeof(x) / sizeof((x)[0]))

const UINT WM_TRAY = WM_USER + 1;
HINSTANCE g_hInstance = NULL;
HICON g_hIcon = NULL;

//suAgentHelper agent;

void LoadStringSafe(UINT nStrID, LPTSTR szBuf, UINT nBufLen)
{
	UINT nLen = LoadString(g_hInstance, nStrID, szBuf, nBufLen);
	if (nLen >= nBufLen)
		nLen = nBufLen - 1;
	szBuf[nLen] = 0;
}

LRESULT CALLBACK HiddenWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			NOTIFYICONDATA stData;
			ZeroMemory(&stData, sizeof(stData));
			stData.cbSize = sizeof(stData);
			stData.hWnd = hWnd;
			stData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			stData.uCallbackMessage = WM_TRAY;
			stData.hIcon = g_hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_TRAYICON));
			LoadStringSafe(IDS_TIP, stData.szTip, _SUcountof(stData.szTip));

			//
			/*if (!agent.start())
			{
				return -1;
			}*/

			if (!Shell_NotifyIcon(NIM_ADD, &stData))
				return -1; // oops
		}
		return 0;

	case WM_DESTROY:
		{
			//
		    //agent.close();

			NOTIFYICONDATA stData;
			ZeroMemory(&stData, sizeof(stData));
			stData.cbSize = sizeof(stData);
			stData.hWnd = hWnd;
			Shell_NotifyIcon(NIM_DELETE, &stData);
		}
		return 0;

	case WM_TRAY:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			SendMessage(hWnd, WM_COMMAND, ID_SHOW, 0);
			break;

		case WM_RBUTTONDOWN:
			{
				HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_POPUP));
				if (hMenu)
				{
					HMENU hSubMenu = GetSubMenu(hMenu, 0);
					if (hSubMenu)
					{
						POINT stPoint;
						GetCursorPos(&stPoint);

						TrackPopupMenu(hSubMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, stPoint.x, stPoint.y, 0, hWnd, NULL);
					}

					DestroyMenu(hMenu);
				}
			}
			break;
		}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_SHOW:
		    {
			std::string strMonitorUrl = "http://" +
				CServerSet::gOnly().GetIP() + ":" +
				CServerSet::gOnly().GetOption().GetOption("Serverport");
			ShellExecute(0, 0, strMonitorUrl.c_str(), 0, 0, SW_SHOW);
			return 0;
		    }

		case ID_QUIT:
			PostQuitMessage(0);
			return 0;
		}
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	TCHAR szTxt[0x100];
	wsprintf(szTxt, _T("hInstance=%08x\n"), hInstance);
	OutputDebugString(szTxt);

	WNDCLASS stWC;
	ZeroMemory(&stWC, sizeof(stWC));
	stWC.lpszClassName = _T("Assystant+");

	HWND hHiddenWnd = FindWindow(stWC.lpszClassName, NULL);
	if (hHiddenWnd)
		PostMessage(hHiddenWnd, WM_TRAY, 0, WM_LBUTTONDBLCLK);
	else
	{
		stWC.hInstance = hInstance;
		stWC.lpfnWndProc = HiddenWndProc;

		ATOM aClass = RegisterClass(&stWC);
		if (aClass)
		{
			g_hInstance = hInstance;
			if (hHiddenWnd = CreateWindow((LPCTSTR) aClass, _T(""), 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL))
			{
				MSG stMsg;
				while (GetMessage(&stMsg, NULL, 0, 0) > 0)
				{
					TranslateMessage(&stMsg);
					DispatchMessage(&stMsg);
				}

				if (IsWindow(hHiddenWnd))
					DestroyWindow(hHiddenWnd);
			}
			UnregisterClass((LPCTSTR) aClass, g_hInstance);
		}
	}
	return 0;
}



