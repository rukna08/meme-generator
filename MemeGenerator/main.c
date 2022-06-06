#include <windows.h>
#include <windowsx.h>
#include <string.h>

#define BUTTON_DISPLAYTEXT 69

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int IsLeftClicked = 0;
int IsInsideTextRect1 = 0;

int MouseXPos;
int MouseYPos;

char* ToBeDisplayedText1;
char* ToBeDisplayedText2;
char* ToBeDisplayedText3;
char* ToBeDisplayedText4;

HWND TextField1;
HWND TextField2;
HWND TextField3;
HWND TextField4;

HFONT WindowControlFont;
HFONT WindowDrawFont;

RECT TextRect1;
RECT TextRect2;
RECT TextRect3;
RECT TextRect4;

SIZE SizeOfText1;

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, LPSTR CommandLine, int CommandShow) {
    WNDCLASSEXA WindowClass = {0};

    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.lpfnWndProc = WndProc;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = LoadIconA(0, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WindowClass.lpszClassName = "MainWindowClass";
    WindowClass.hIconSm = LoadIconA(0, IDI_APPLICATION);
    
    RegisterClassExA(&WindowClass);
    
    HWND WindowHandle = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_COMPOSITED, WindowClass.lpszClassName, "Meme Generator", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX) , CW_USEDEFAULT, CW_USEDEFAULT, 800, 400, 0, 0, Instance, 0);

    TextField1 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField2 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 50, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField3 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 90, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField4 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 130, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    
    HWND DisplayTextButton = CreateWindowA("BUTTON", "Display Text", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 170, 100, 30, WindowHandle, (HMENU)BUTTON_DISPLAYTEXT, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);

    NONCLIENTMETRICSA NonClientMetrics;
	NonClientMetrics.cbSize = sizeof(NonClientMetrics);
    
	SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(NonClientMetrics), &NonClientMetrics, 0);
    
    WindowControlFont = CreateFontIndirectA(&NonClientMetrics.lfMessageFont);
    
    NonClientMetrics.lfMessageFont.lfHeight = 100;
    
    WindowDrawFont = CreateFontIndirectA(&NonClientMetrics.lfMessageFont);

    SendMessage(TextField1, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField2, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField3, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField4, WM_SETFONT, (WPARAM)WindowControlFont, 1);

    SendMessage(DisplayTextButton, WM_SETFONT, (WPARAM)WindowControlFont, 1);

    ShowWindow(WindowHandle, CommandShow);
    UpdateWindow(WindowHandle);

    MSG Message = {0};

    while(GetMessage(&Message, 0, 0, 0) > 0) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    DeleteObject(WindowControlFont);
    DeleteObject(WindowDrawFont);

    return 0;
}

LRESULT CALLBACK WndProc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam) {
    
    switch(Message) {
        case WM_CLOSE:
            DestroyWindow(WindowHandle);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_COMMAND:
            InvalidateRect(WindowHandle, 0, 1);

            if(LOWORD(WParam) == BUTTON_DISPLAYTEXT) {
                ToBeDisplayedText1 = malloc(100);
                ToBeDisplayedText2 = malloc(100);
                ToBeDisplayedText3 = malloc(100);
                ToBeDisplayedText4 = malloc(100);

                GetWindowTextA(TextField1, ToBeDisplayedText1, 100);
                GetWindowTextA(TextField2, ToBeDisplayedText2, 100);
                GetWindowTextA(TextField3, ToBeDisplayedText3, 100);
                GetWindowTextA(TextField4, ToBeDisplayedText4, 100);

                HDC DeviceContextHandle = GetDC(WindowHandle);
                SelectObject(DeviceContextHandle, WindowDrawFont);

                GetTextExtentPoint32A(DeviceContextHandle, ToBeDisplayedText1, strlen(ToBeDisplayedText1), &SizeOfText1);
                ReleaseDC(WindowHandle, DeviceContextHandle);

                TextRect1.left = 350;
                TextRect1.top = 50;
                TextRect1.right = TextRect1.left + SizeOfText1.cx;
                TextRect1.bottom = TextRect1.top + SizeOfText1.cy;
                // TextRect1.right = 800;
                // TextRect1.bottom = 450;

                TextRect2.left = 600;
                TextRect2.top = 50;
                TextRect2.right = 800;
                TextRect2.bottom = 450;

                TextRect3.left = 350;
                TextRect3.top = 200;
                TextRect3.right = 800;
                TextRect3.bottom = 450;

                TextRect4.left = 600;
                TextRect4.top = 200;
                TextRect4.right = 800;
                TextRect4.bottom = 450;
            }

            break;

        case WM_PAINT:
            PAINTSTRUCT PaintStruct;

            HDC DeviceContextHandle = BeginPaint(WindowHandle, &PaintStruct);

            SelectObject(DeviceContextHandle, WindowDrawFont);

            DrawTextA(DeviceContextHandle, ToBeDisplayedText1, -1, &TextRect1, DT_LEFT | DT_NOPREFIX);
            DrawTextA(DeviceContextHandle, ToBeDisplayedText2, -1, &TextRect2, DT_LEFT | DT_NOPREFIX);
            DrawTextA(DeviceContextHandle, ToBeDisplayedText3, -1, &TextRect3, DT_LEFT | DT_NOPREFIX);
            DrawTextA(DeviceContextHandle, ToBeDisplayedText4, -1, &TextRect4, DT_LEFT | DT_NOPREFIX);

            EndPaint(WindowHandle, &PaintStruct);

            break;

        case WM_LBUTTONDOWN:
            IsLeftClicked = 1;
            
            MouseXPos = GET_X_LPARAM(LParam);
            MouseYPos = GET_Y_LPARAM(LParam);

            POINT MouseLocation = {MouseXPos, MouseYPos};

            IsInsideTextRect1 = PtInRect(&TextRect1, MouseLocation);
            
            break;

        case WM_LBUTTONUP:
            IsLeftClicked = 0;
            IsInsideTextRect1 = 0;
            
            break;

        case WM_MOUSEMOVE:
            // dragging code
            
            if(IsLeftClicked && IsInsideTextRect1) {
                int NewMouseXPos = GET_X_LPARAM(LParam);
                int NewMouseYPos = GET_Y_LPARAM(LParam);

                OffsetRect(&TextRect1, NewMouseXPos - MouseXPos, NewMouseYPos - MouseYPos);
                InvalidateRect(WindowHandle, 0, 1);

                MouseXPos = NewMouseXPos;
                MouseYPos = NewMouseYPos;
            }

            break;
        
        default:
            return DefWindowProc(WindowHandle, Message, WParam, LParam);
    }
    
    return 0;
}