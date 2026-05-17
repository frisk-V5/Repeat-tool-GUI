#include <stdio.h>
#include <windows.h>

// キーが押されているか判定するマクロ
#define IS_KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

int main() {
    int ms;

    printf("クリック間隔をミリ秒で入力してください: ");
    if (scanf("%d", &ms) != 1 || ms < 1) {
        printf("無効な数値です。1以上の数値を入力してください。\n");
        return 1;
    }

    printf("F6で開始 / F7で停止\n");

    while (1) {
        // F6キーで連打開始
        if (IS_KEY_DOWN(VK_F6)) {
            printf("連打中...\n");

            // F7キーが押されるまでループ
            while (!IS_KEY_DOWN(VK_F7)) {
                // 左クリックの押し下げと解放
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                // 設定されたミリ秒待機（途中でF7が押されたらすぐ抜ける）
                int remaining = ms;
                while (remaining > 0) {
                    if (IS_KEY_DOWN(VK_F7)) break;
                    Sleep(remaining > 10 ? 10 : remaining);
                    remaining -= 10;
                }
            }

            printf("停止しました。再開するにはF6を押してください。\n");
            Sleep(300); // 誤作動防止のウェイト
        }
        Sleep(10); // CPU負荷を抑えるためのウェイト
    }

    return 0;
}
