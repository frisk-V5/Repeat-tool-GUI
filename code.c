#include <stdio.h>
#include <windows.h>

#define IS_KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

int main(){
    int ms;

    printf("クリック間隔をミリ秒で入力してください: ");
    if (scanf("%d", &ms) != 1 || ms < 1) {
        printf("無効な数値です。1以上の数値を入力してください。\n");
        return 1;
    }

    printf("F6で開始 / F7で停止\n");

    while(1){
        if(IS_KEY_DOWN(VK_F6)){
            printf("連打中...\n");
            
            while(!IS_KEY_DOWN(VK_F7)){
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                int remaining = ms;
                while (remaining > 0) {
                    if (IS_KEY_DOWN(VK_F7)) break;
                    Sleep(remaining > 10 ? 10 : remaining);
                    remaining -= 10;
                }
            }
            printf("停止しました。再開するにはF6を押してください。\n");
            Sleep(300);
        }
        Sleep(10);
    }
    return 0;
}
