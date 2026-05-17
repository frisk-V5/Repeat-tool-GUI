#include <windows.h>
#include <tchar.h>
#include <stdio.h>

// コントロールのID定義
#define ID_EDIT_MS 101
#define ID_BTN_APPLY 102
#define ID_STATIC_STATUS 103

// タイマーIDとキー入力判定用のマクロ
#define TIMER_ID 1
#define IS_KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

// グローバル変数
int g_ms = 100;              // クリックの間隔 (ミリ秒)
BOOL g_is_clicking = FALSE;  // 現在連打中かどうかのフラグ

// ウィンドウプロシージャの宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const TCHAR szClassName[] = _T("AutoClickerWindowClass");

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = szClassName;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("ウィンドウクラスの登録に失敗しました。"), _T("エラー"), MB_ICONERROR);
        return 1;
    }

    // ウィンドウの作成 (サイズ: 300x180)
    HWND hwnd = CreateWindowEx(
        0, szClassName, _T("オートクリッカー"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 180,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, _T("ウィンドウの作成に失敗しました。"), _T("エラー"), MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // メッセージループ
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// ステータステキストを更新する関数
void UpdateStatusText(HWND hwndStatic) {
    TCHAR buf[128];
    if (g_is_clicking) {
        _stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _T("ステータス: 連打中... (F7で停止)"));
    } else {
        _stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _T("ステータス: 停止中 (F6で開始)\n現在の間隔: %d ms"), g_ms);
    }
    SetWindowText(hwndStatic, buf);
}

// ウィンドウメッセージの処理
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hEditMs, hBtnApply, hStaticStatus;

    switch (msg) {
    case WM_CREATE:
        // ラベルの作成
        CreateWindow(_T("STATIC"), _T("クリック間隔 (ms):"),
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 20, 130, 20, hwnd, NULL, NULL, NULL);

        // テキストボックスの作成（デフォルト100ms、数値のみ入力可）
        hEditMs = CreateWindow(_T("EDIT"), _T("100"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_AUTOHSCROLL,
            150, 18, 100, 22, hwnd, (HMENU)ID_EDIT_MS, NULL, NULL);

        // 設定反映ボタンの作成
        hBtnApply = CreateWindow(_T("BUTTON"), _T("設定変更"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            150, 48, 100, 25, hwnd, (HMENU)ID_BTN_APPLY, NULL, NULL);

        // ステータス表示ラベルの作成
        hStaticStatus = CreateWindow(_T("STATIC"), _T(""),
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 90, 240, 40, hwnd, (HMENU)ID_STATIC_STATUS, NULL, NULL);

        UpdateStatusText(hStaticStatus);

        // キー状態を10ミリ秒ごとに監視するためのタイマーをセット
        SetTimer(hwnd, TIMER_ID, 10, NULL);
        break;

    case WM_COMMAND:
        // ボタンが押されたときの処理
        if (LOWORD(wParam) == ID_BTN_APPLY) {
            TCHAR buf[16];
            GetWindowText(hEditMs, buf, 16);
            int val = _ttoi(buf);
            if (val >= 1) {
                g_ms = val;
                UpdateStatusText(hStaticStatus);
            } else {
                MessageBox(hwnd, _T("1以上の数値を入力してください。"), _T("入力エラー"), MB_ICONWARNING);
                SetWindowText(hEditMs, _T("1"));
            }
        }
        break;

    case WM_TIMER:
        if (wParam == TIMER_ID) {
            static DWORD lastClickTime = 0;

            // F6が押されたら連打を開始
            if (!g_is_clicking && IS_KEY_DOWN(VK_F6)) {
                g_is_clicking = TRUE;
                UpdateStatusText(hStaticStatus);
                lastClickTime = GetTickCount(); // タイマーリセット
            }
            // F7が押されたら連打を停止
            else if (g_is_clicking && IS_KEY_DOWN(VK_F7)) {
                g_is_clicking = FALSE;
                UpdateStatusText(hStaticStatus);
            }

            // 連打中のクリックシミュレーション処理（途切れていた部分の補完）
            if (g_is_clicking) {
                DWORD currentTime = GetTickCount();
                if (currentTime - lastClickTime >= (DWORD)g_ms) {
                    // マウスの左ボタン押し下げと解放をシミュレート
                    INPUT inputs[2] = { 0 };

                    inputs[0].type = INPUT_MOUSE;
                    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

                    inputs[1].type = INPUT_MOUSE;
                    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

                    SendInput(2, inputs, sizeof(INPUT));
                    lastClickTime = currentTime;
                }
            }
        }
        break;

    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
