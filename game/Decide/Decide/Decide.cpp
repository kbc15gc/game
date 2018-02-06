// Decide.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include"stdafx.h"
#include"Decide.h"
#include <direct.h>

#define MAX_LOADSTRING 100
//最初のスクリーンサイズ
Vector2 g_StartWindowSize;

bool IS_CONTINUE;

// グローバル変数:
CHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
CHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void InitializeGame();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DECIDE, szWindowClass, MAX_LOADSTRING);

	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DECIDE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DECIDE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	g_MainWindow = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, WindowW, WindowH, nullptr, nullptr, hInstance, nullptr);

	/*auto hdc = GetDC(g_MainWindow);
	int R, G, B;
	auto color = GetSysColor(COLOR_ACTIVECAPTION);
	R = GetRValue(color);
	G = GetGValue(color);
	B = GetBValue(color);
	int elements[] = { COLOR_ACTIVECAPTION };
	COLORREF colors[] = { RGB(255,0,0) };
	SetSysColors(COLOR_ACTIVECAPTION, elements, colors);

	color = GetSysColor(COLOR_ACTIVECAPTION);
	R = GetRValue(color);
	G = GetGValue(color);
	B = GetBValue(color);*/

	/*MyRegisterClass(hInstance,
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DECIDE)),
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		MAKEINTRESOURCEW(IDC_DECIDE),
		szWindowClass,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL))
	);

	g_MainWindow = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, WindowW, WindowH, nullptr, nullptr, hInstance, nullptr);*/

	D3DXVECTOR2 WindowSize, ClientSize, Diff;
	RECT window, client;

	//ウィンドウエリアサイズ（外枠を含めたサイズ）
	GetWindowRect(g_MainWindow, &window);
	WindowSize.x = (float)(window.right - window.left);
	WindowSize.y = (float)(window.bottom - window.top);
	//クライアントエリアサイズ(描画している内側のサイズ)
	GetClientRect(g_MainWindow, &client);
	ClientSize.x = (float)(client.right - client.left);
	ClientSize.y = (float)(client.bottom - client.top);
	//差を求める
	Diff.x = WindowSize.x - ClientSize.x;
	Diff.y = WindowSize.y - ClientSize.y;
	//ゲームウィンドウの大きさ調整

	g_StartWindowSize = Vector2(WindowW, WindowH);
	g_WindowSize = Vector2(WindowW, WindowH);

#ifdef RELEASE_LOW
	g_FrameSize = g_WindowSize * 0.5f;
#else
	g_FrameSize = g_WindowSize * 1.0f;
#endif // RELEASE_LOW

	SetWindowPos(g_MainWindow, HWND_TOP, 0, 0, WindowW + Diff.x, WindowH + Diff.y, (SWP_NOZORDER));

	// アプリケーションの初期化を実行します:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//フォルダ作成.
	_mkdir("C:\\KBCGame");
	_mkdir("C:\\KBCGame\\ReHistory");
	_mkdir("C:\\KBCGame\\ReHistory\\SaveData");

	InitializeGame();

	return GameStart(hInstance, nCmdShow, IDC_DECIDE);
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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

#include "Scene\TitleScene.h"
#include "Scene\GameScene.h"
#include "Scene\LastBossTestScene.h"
#include "Scene\EndingScene.h"
#include "Scene\OpeningScene.h"

void InitializeGame()
{
	INSTANCE(SoundEngine)->Init();

	INSTANCE(SceneManager)->Add<TitleScene>();
	INSTANCE(SceneManager)->Add<GameScene>();
	INSTANCE(SceneManager)->Add<LastBossTestScene>();
	INSTANCE(SceneManager)->Add<EndingScene>();
	INSTANCE(SceneManager)->Add<OpeningScene>();

	INSTANCE(SceneManager)->ChangeScene("TitleScene");
}

//void OnNcPaint()
//{
//	int cyCaption = 4;//GetSystemMetrics(SM_CYCAPTION);//タイトルバーの高さを取得
//	int cxFrame = GetSystemMetrics(SM_CXFRAME);//　フレームの幅
//	int cyFrame = GetSystemMetrics(SM_CYFRAME);//　フレームの高さ
//	COLORREF crCaption = RGB(250, 250, 0);//　塗りつぶす色
//
//	RECT rcWnd;
//	GetWindowRect(g_MainWindow,&rcWnd);
//
//	//auto hdc = GetDC(g_MainWindow);
//	//hdc->FillSolidRect(0, 0, rcWnd.Width(), cyCaption, crCaption);//フレーム上部
//	//hdc->FillSolidRect(0, cyFrame, 4, rcWnd.Height(), crCaption);//左
//	//hdc->FillSolidRect(rcWnd.Width() - cxFrame, cyFrame - cxFrame, 4, rcWnd.Height(), crCaption);//右
//	//hdc->FillSolidRect(0, rcWnd.Height() - 4, rcWnd.Width(), 4, crCaption);//フレーム下部
//
//	//ReleaseDC(pDC);
//}