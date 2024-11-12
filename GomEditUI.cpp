// GomEditUI.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <windows.h>

#define ID_MYBUTTON          0x000102L
typedef int (WINAPI * ShowEditDlgFunc)(void*, int, int);
char* UI_buffer = NULL; 
long UI_len=0;
char* StateUI_buffer = NULL; 
long StateUI_len=0;
char* ConfigUI_buffer = NULL; 
long ConfigUI_len=0;
ShowEditDlgFunc ShowEditDlg = NULL;

int main1(HWND hWnd)
{
	HINSTANCE hDll = LoadLibraryA("GuiEditDll.dll");
	if (hDll == NULL) {
        MessageBoxA(hWnd, "无法加载dll!", "提示", MB_OK);
        return 1;
    }
	ShowEditDlgFunc ShowEditDlg = (ShowEditDlgFunc)GetProcAddress(hDll, "ShowEditDlg");
	if (ShowEditDlg == NULL) {
        MessageBoxA(hWnd, "无法获取函数地址!", "提示", MB_OK);
        return 1;
    }
	FILE *file;
	int fileSize;
	file = fopen("UI_0/UI.DB", "rb");
	if (file == NULL) {
        MessageBoxA(hWnd, "无法打开文件!", "提示", MB_OK);
        return 1;
    }
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = (char *)malloc(fileSize); 
	fread(buffer, fileSize, 1, file); // 读取文件内容到缓冲区
	ShowEditDlg(buffer, fileSize, 0);
	//printf("File size: %ld bytes\n", fileSize);
	//printf("File content:\n%s\n", buffer);
	fclose(file); // 关闭文件
	return 0;
}
int init_UI() {

    HINSTANCE hDll = LoadLibraryA("GuiEditDll.dll");
	if (hDll == NULL) {
        MessageBoxA(NULL, "无法加载dll!", "提示", MB_OK);
        return 1;
	}
    ShowEditDlg = (ShowEditDlgFunc)GetProcAddress(hDll, "ShowEditDlg");
	if (ShowEditDlg == NULL) {
        MessageBoxA(NULL, "无法获取函数地址!", "提示", MB_OK);
        return 1;
    }

    FILE * file = fopen("UI_0/UI.DB", "rb");
	if (file == NULL) {
        MessageBoxA(NULL, "无法打开文件!", "提示", MB_OK);
        return 1;
    }
    fseek(file, 0, SEEK_END);
	UI_len = ftell(file);
	fseek(file, 0, SEEK_SET);
	UI_buffer = (char *)malloc(UI_len); 
    fread(UI_buffer, UI_len, 1, file); 
    fclose(file);

    file = fopen("UI_0/StateUI.DB", "rb");
	if (file == NULL) {
        MessageBoxA(NULL, "无法打开文件!", "提示", MB_OK);
        return 1;
    }
    fseek(file, 0, SEEK_END);
	StateUI_len = ftell(file);
	fseek(file, 0, SEEK_SET);
	StateUI_buffer = (char *)malloc(StateUI_len); 
    fread(StateUI_buffer, StateUI_len, 1, file); 
    fclose(file);


    file = fopen("UI_0/ConfigUI.DB", "rb");
	if (file == NULL) {
        MessageBoxA(NULL, "无法打开文件!", "提示", MB_OK);
        return 1;
    }
    fseek(file, 0, SEEK_END);
	ConfigUI_len = ftell(file);
	fseek(file, 0, SEEK_SET);
	ConfigUI_buffer = (char *)malloc(ConfigUI_len); 
    fread(ConfigUI_buffer, ConfigUI_len, 1, file); 
    fclose(file);

    
    return 0;
}
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            // 处理按钮点击消息
            if (LOWORD(wParam) == ID_MYBUTTON) {
                ShowEditDlg(UI_buffer, UI_len, 0);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
 
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if(init_UI() == 1) {
        return 0;
    }



    WNDCLASSA wc;
    memset(&wc, 0, sizeof(WNDCLASSA));
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = "SimpleWindow";
    if (!RegisterClassA(&wc)) {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
 
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        TEXT("SimpleWindow"),
        TEXT("Simple Window"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL, NULL, hInstance, NULL
    );
 
    if (hWnd == NULL) {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    HWND hButton = CreateWindowA("BUTTON", "点击我", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        200, 100, 100, 30, hWnd, (HMENU)ID_MYBUTTON, hInstance, NULL);
 
    ShowWindow(hWnd, nCmdShow);
 
    MSG Msg ;
 
    while (GetMessage(&Msg, NULL, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
 
    return Msg.wParam;
}



//typedef void (WINBASEAPI FARPROC WINAPI ShowEditDlgFunc)(void*, int, int);


