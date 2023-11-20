#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
#define BUTTON_DISPLAYTEXT 69
#define BUTTON_COPYIMAGE   420
#define BUTTON_OPENIMAGE   582
 
#define DEBUG 0
 
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void GetScreenShot();

// Returns file path
char* display_opendialog();

int WindowWidth = 800;
int WindowHeight = 900;
 
int FontSize = 50;
 
int IsLeftClicked = 0;
int IsInsideTextRect1 = 0;
int IsInsideTextRect2 = 0;
int IsInsideTextRect3 = 0;
int IsInsideTextRect4 = 0;
 
int IsInsideImageRect1 = 0;
int IsInsideImageRect2 = 0;
 
int MouseXPos;
int MouseYPos;
 
char ToBeDisplayedText1[100];
char ToBeDisplayedText2[100];
char ToBeDisplayedText3[100];
char ToBeDisplayedText4[100];
 
HWND TextField1;
HWND TextField2;
HWND TextField3;
HWND TextField4;
 
HFONT WindowControlFont;
HFONT WindowDrawFont;
 
HBRUSH GreenBrush;
 
RECT TextRect1;
RECT TextRect2;
RECT TextRect3;
RECT TextRect4;
 
RECT ImageRect1;
RECT ImageRect1TopRight;
 
RECT ImageRect2;
RECT ImageRect2TopRight;
 
SIZE SizeOfText1;
SIZE SizeOfText2;
SIZE SizeOfText3;
SIZE SizeOfText4;
 
HDC DeviceContextHandleImage;
HBITMAP ImageHandle;
BITMAP ImageHandleInfo;
 
HDC DeviceContextHandleImage2;
HBITMAP ImageHandle2;
BITMAP ImageHandleInfo2;
 
COLORREF GreenColor = 0x0000FF00;
COLORREF BlackColor = 0x00000000;

HWND WindowHandle;
HINSTANCE instance_handle;
 
int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, LPSTR CommandLine, int CommandShow) {
    WNDCLASSEXA WindowClass = {0};
 
    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = LoadIconA(0, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WindowClass.lpszClassName = "MainWindowClass";
    WindowClass.hIconSm = LoadIconA(0, IDI_APPLICATION);
    
    RegisterClassExA(&WindowClass);
    
    WindowHandle = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_COMPOSITED, WindowClass.lpszClassName, "Meme Generator", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, 0, 0, Instance, 0);
 
    TextField1 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField2 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 50, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField3 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 90, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField4 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 130, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    
    HWND DisplayTextButton = CreateWindowA("BUTTON", "Display Text", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 170, 100, 30, WindowHandle, (HMENU)BUTTON_DISPLAYTEXT, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    HWND CopyImageButton   = CreateWindowA("BUTTON", "Copy Image",   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 205, 100, 30, WindowHandle, (HMENU)BUTTON_COPYIMAGE, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    HWND open_image_button = CreateWindowA("BUTTON", "Open Image",   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 240, 100, 30, WindowHandle, (HMENU)BUTTON_OPENIMAGE, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
 
    NONCLIENTMETRICSA NonClientMetrics;
    NonClientMetrics.cbSize = sizeof(NonClientMetrics);
    
    SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(NonClientMetrics), &NonClientMetrics, 0);
    
    WindowControlFont = CreateFontIndirectA(&NonClientMetrics.lfMessageFont);
    
    NonClientMetrics.lfMessageFont.lfHeight = FontSize;
    
    WindowDrawFont = CreateFontIndirectA(&NonClientMetrics.lfMessageFont);
    
    ImageRect1.left = 400;
    ImageRect1.top = 50;
 
    ImageRect1TopRight.left = 0;
    ImageRect1TopRight.top = 0;
    ImageRect1TopRight.right = 0;
    ImageRect1TopRight.bottom = 0;
 
    ImageRect2.left = 400;
    ImageRect2.top = 50;
 
    ImageRect2TopRight.left = 0;
    ImageRect2TopRight.top = 0;
    ImageRect2TopRight.right = 0;
    ImageRect2TopRight.bottom = 0;
 
    SendMessage(TextField1, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField2, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField3, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField4, WM_SETFONT, (WPARAM)WindowControlFont, 1);
 
    SendMessage(DisplayTextButton, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(CopyImageButton,   WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(open_image_button, WM_SETFONT, (WPARAM)WindowControlFont, 1);
 
    ImageHandle = (HBITMAP)LoadImageA(0, "../data/sample.bmp", 0, 0, 0, LR_LOADFROMFILE);
    GetObject(ImageHandle, sizeof(BITMAP), &ImageHandleInfo);
    
    DeviceContextHandleImage = CreateCompatibleDC(NULL);
    SelectObject(DeviceContextHandleImage, ImageHandle);
 
    ImageHandle2 = (HBITMAP)LoadImageA(0, "../data/sample2.bmp", 0, 0, 0, LR_LOADFROMFILE);
    GetObject(ImageHandle2, sizeof(BITMAP), &ImageHandleInfo2);
    
    DeviceContextHandleImage2 = CreateCompatibleDC(NULL);
    SelectObject(DeviceContextHandleImage2, ImageHandle2);
    
    GreenBrush = CreateSolidBrush(GreenColor);
 
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
 
LRESULT CALLBACK WindowProc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam) {
    
    switch(Message) {
        case WM_CLOSE:
            DestroyWindow(WindowHandle);
            break;
 
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
 
        case WM_COMMAND:
        
            InvalidateRect(WindowHandle, 0, 1);

            if(LOWORD(WParam) == BUTTON_OPENIMAGE) {

                char* file_path = display_opendialog();

                for(int i = 0; file_path[i] != '\0'; i++) {

                    // 92 = '\' and 47 = '/'

                    if(file_path[i] == 92) {

                        file_path[i] = 47;
                    
                    }

                }

                OutputDebugStringA(file_path);

                ImageHandle2 = (HBITMAP)LoadImageA(0, file_path, 0, 0, 0, LR_LOADFROMFILE);

                GetObject(ImageHandle2, sizeof(BITMAP), &ImageHandleInfo2);

                SelectObject(DeviceContextHandleImage2, ImageHandle2);

                UpdateWindow(WindowHandle);

            }
 
            if(LOWORD(WParam) == BUTTON_COPYIMAGE) {
                BITMAPINFO OutputImageBitmapInfo = {0};
                OutputImageBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 
                HDC TemporaryDC = GetDC(WindowHandle);
 
                GetDIBits(TemporaryDC, GetCurrentObject(TemporaryDC, OBJ_BITMAP), 0, WindowHeight, 0, &OutputImageBitmapInfo, DIB_RGB_COLORS);

                LPVOID PixelDataBuffer = malloc(OutputImageBitmapInfo.bmiHeader.biSizeImage);
 
                GetDIBits(TemporaryDC, GetCurrentObject(TemporaryDC, OBJ_BITMAP), 0, WindowHeight, PixelDataBuffer, &OutputImageBitmapInfo, DIB_RGB_COLORS);
 
                ReleaseDC(WindowHandle, TemporaryDC);
 
                BITMAPFILEHEADER BitmapFileHeader = {0};
 
                BitmapFileHeader.bfType = 'MB';
                BitmapFileHeader.bfSize = OutputImageBitmapInfo.bmiHeader.biSizeImage + sizeof(BITMAPINFO) + sizeof(BITMAPFILEHEADER);
                BitmapFileHeader.bfOffBits = sizeof(BITMAPINFO) + sizeof(BITMAPFILEHEADER);
 
                // ------------ Exporting the image --------------

                GetScreenShot();

                MessageBoxA(0, "Output Image copied!", "Info", MB_OK | MB_ICONINFORMATION);
            }
 
            if(LOWORD(WParam) == BUTTON_DISPLAYTEXT) {
                GetWindowTextA(TextField1, ToBeDisplayedText1, 100);
                GetWindowTextA(TextField2, ToBeDisplayedText2, 100);
                GetWindowTextA(TextField3, ToBeDisplayedText3, 100);
                GetWindowTextA(TextField4, ToBeDisplayedText4, 100);
 
                HDC DeviceContextHandle = GetDC(WindowHandle);
                SelectObject(DeviceContextHandle, WindowDrawFont);
 
                GetTextExtentPoint32A(DeviceContextHandle, ToBeDisplayedText1, strlen(ToBeDisplayedText1), &SizeOfText1);
                GetTextExtentPoint32A(DeviceContextHandle, ToBeDisplayedText2, strlen(ToBeDisplayedText2), &SizeOfText2);
                GetTextExtentPoint32A(DeviceContextHandle, ToBeDisplayedText3, strlen(ToBeDisplayedText3), &SizeOfText3);
                GetTextExtentPoint32A(DeviceContextHandle, ToBeDisplayedText4, strlen(ToBeDisplayedText4), &SizeOfText4);
 
                ReleaseDC(WindowHandle, DeviceContextHandle);
 
                TextRect1.left = 350;
                TextRect1.top = 50;
                TextRect1.right = TextRect1.left + SizeOfText1.cx;
                TextRect1.bottom = TextRect1.top + SizeOfText1.cy;
                
                TextRect2.left = 600;
                TextRect2.top = 50;
                TextRect2.right = TextRect2.left + SizeOfText2.cx;
                TextRect2.bottom = TextRect2.top + SizeOfText2.cy;
 
                TextRect3.left = 350;
                TextRect3.top = 200;
                TextRect3.right = TextRect3.left + SizeOfText3.cx;
                TextRect3.bottom = TextRect3.top + SizeOfText3.cy;
 
                TextRect4.left = 600;
                TextRect4.top = 200;
                TextRect4.right = TextRect4.left + SizeOfText4.cx;
                TextRect4.bottom = TextRect4.top + SizeOfText4.cy;
            }
 
            break;
 
        case WM_PAINT:
            PAINTSTRUCT PaintStruct;
 
            HDC DeviceContextHandle = BeginPaint(WindowHandle, &PaintStruct);
 
            int DestWidth = 200;
 
            ImageRect1.right = ImageRect1.left + DestWidth;
            ImageRect1.bottom = ImageRect1.top + ((DestWidth * ImageHandleInfo.bmHeight) / ImageHandleInfo.bmWidth);
 
            ImageRect1TopRight.left = ImageRect1.right - 10;
            ImageRect1TopRight.top = ImageRect1.top - 10;
            ImageRect1TopRight.right = ImageRect1.right + 10;
            ImageRect1TopRight.bottom = ImageRect1.top + 10;
 
            SetStretchBltMode(DeviceContextHandle, HALFTONE);
            StretchBlt(DeviceContextHandle, ImageRect1.left, ImageRect1.top, ImageRect1.right - ImageRect1.left, ImageRect1.bottom - ImageRect1.top, DeviceContextHandleImage, 0, 0, ImageHandleInfo.bmWidth, ImageHandleInfo.bmHeight, SRCCOPY);
            
            ImageRect2.right = ImageRect2.left + DestWidth;
            ImageRect2.bottom = ImageRect2.top + ((DestWidth * ImageHandleInfo2.bmHeight) / ImageHandleInfo2.bmWidth);
 
            ImageRect2TopRight.left = ImageRect2.right - 10;
            ImageRect2TopRight.top = ImageRect2.top - 10;
            ImageRect2TopRight.right = ImageRect2.right + 10;
            ImageRect2TopRight.bottom = ImageRect2.top + 10;
 
            SetStretchBltMode(DeviceContextHandle, HALFTONE);
            StretchBlt(DeviceContextHandle, ImageRect2.left, ImageRect2.top, ImageRect2.right - ImageRect2.left, ImageRect2.bottom - ImageRect2.top, DeviceContextHandleImage2, 0, 0, ImageHandleInfo2.bmWidth, ImageHandleInfo2.bmHeight, SRCCOPY);
 
            // ------ Final Image export area ------
 
            // north line
            for(int i = 20; i < WindowWidth - 40; i++) {
                SetPixel(DeviceContextHandle, i, 300, BlackColor);
            }
 
            // south line
            for(int i = 20; i < WindowWidth - 40; i++) {
                SetPixel(DeviceContextHandle, i, WindowHeight - 60, BlackColor);
            }
 
            // west line
            for(int i = 300; i < WindowHeight - 60; i++) {
                SetPixel(DeviceContextHandle, 20, i, BlackColor);
            }
 
            // east line
            for(int i = 300; i < WindowHeight - 60; i++) {
                SetPixel(DeviceContextHandle, WindowWidth - 40, i, BlackColor);
            }
 
            // -------------------------------------
 
            SelectObject(DeviceContextHandle, WindowDrawFont);
 
            #if DEBUG
            FillRect(DeviceContextHandle, &TextRect1, GreenBrush);
            FillRect(DeviceContextHandle, &TextRect2, GreenBrush);
            FillRect(DeviceContextHandle, &TextRect3, GreenBrush);
            FillRect(DeviceContextHandle, &TextRect4, GreenBrush);
            #endif
 
            SetBkMode(DeviceContextHandle, TRANSPARENT);
 
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
            IsInsideTextRect2 = PtInRect(&TextRect2, MouseLocation);
            IsInsideTextRect3 = PtInRect(&TextRect3, MouseLocation);
            IsInsideTextRect4 = PtInRect(&TextRect4, MouseLocation);
 
            IsInsideImageRect1 = PtInRect(&ImageRect1, MouseLocation);
 
            IsInsideImageRect2 = PtInRect(&ImageRect2, MouseLocation);
            
            break;
 
        case WM_LBUTTONUP:
            IsLeftClicked = 0;
            IsInsideTextRect1 = 0;
            IsInsideTextRect2 = 0;
            IsInsideTextRect3 = 0;
            IsInsideTextRect4 = 0;
 
            IsInsideImageRect1 = 0;
            IsInsideImageRect2 = 0;
 
            break;
 
        case WM_MOUSEMOVE:        
            POINT MouseLocation2 = {GET_X_LPARAM(LParam), GET_Y_LPARAM(LParam)};
            int IsInsideImageRect1ForDisplayAllArrow = PtInRect(&ImageRect1, MouseLocation2);
            int IsInsideImageRect1TopRightNESWArrow = PtInRect(&ImageRect1TopRight, MouseLocation2);
 
            int IsInsideImageRect2ForDisplayAllArrow = PtInRect(&ImageRect2, MouseLocation2);
            int IsInsideImageRect2TopRightNESWArrow = PtInRect(&ImageRect2TopRight, MouseLocation2);
 
            if(PtInRect(&TextRect1, MouseLocation2) || PtInRect(&TextRect2, MouseLocation2) || PtInRect(&TextRect3, MouseLocation2) || PtInRect(&TextRect4, MouseLocation2)) {
                SetCursor(LoadCursorA(0, IDC_SIZEALL));
            }
 
            if(IsInsideImageRect1ForDisplayAllArrow) {
                SetCursor(LoadCursorA(0, IDC_SIZEALL));
            }
 
            if(IsInsideImageRect1TopRightNESWArrow) {
                SetCursor(LoadCursorA(0, IDC_SIZENESW));
            }
 
            if(IsInsideImageRect2ForDisplayAllArrow) {
                SetCursor(LoadCursorA(0, IDC_SIZEALL));
            }
 
            if(IsInsideImageRect2TopRightNESWArrow) {
                SetCursor(LoadCursorA(0, IDC_SIZENESW));
            }
 
            if(IsLeftClicked && IsInsideTextRect1) {
                int NewMouseXPos = GET_X_LPARAM(LParam);
                int NewMouseYPos = GET_Y_LPARAM(LParam);
                RECT UpdateRect = TextRect1;
 
                OffsetRect(&TextRect1, NewMouseXPos - MouseXPos, NewMouseYPos - MouseYPos);
                UnionRect(&UpdateRect, &UpdateRect, &TextRect1);
                InvalidateRect(WindowHandle, &UpdateRect, 1);
 
                MouseXPos = NewMouseXPos;
                MouseYPos = NewMouseYPos;
            } else if(IsLeftClicked && IsInsideTextRect2) {
                int NewMouseXPos = GET_X_LPARAM(LParam);
                int NewMouseYPos = GET_Y_LPARAM(LParam);
 
                OffsetRect(&TextRect2, NewMouseXPos - MouseXPos, NewMouseYPos - MouseYPos);
                InvalidateRect(WindowHandle, 0, 1);
 
                MouseXPos = NewMouseXPos;
                MouseYPos = NewMouseYPos;
            } else if(IsLeftClicked && IsInsideTextRect3) {
                int NewMouseXPos = GET_X_LPARAM(LParam);
                int NewMouseYPos = GET_Y_LPARAM(LParam);
 
                OffsetRect(&TextRect3, NewMouseXPos - MouseXPos, NewMouseYPos - MouseYPos);
                InvalidateRect(WindowHandle, 0, 1);
 
                MouseXPos = NewMouseXPos;
                MouseYPos = NewMouseYPos;
            } else if(IsLeftClicked && IsInsideTextRect4) {
                int NewMouseXPos = GET_X_LPARAM(LParam);
                int NewMouseYPos = GET_Y_LPARAM(LParam);
 
                OffsetRect(&TextRect4, NewMouseXPos - MouseXPos, NewMouseYPos - MouseYPos);
                InvalidateRect(WindowHandle, 0, 1);
 
                MouseXPos = NewMouseXPos;
                MouseYPos = NewMouseYPos;
            } else if(IsLeftClicked && IsInsideImageRect1) {
                int NewMouseXPos = GET_X_LPARAM(LParam);
                int NewMouseYPos = GET_Y_LPARAM(LParam);
                RECT UpdateRect = ImageRect1;
 
                OffsetRect(&ImageRect1, NewMouseXPos - MouseXPos, NewMouseYPos - MouseYPos);
                UnionRect(&UpdateRect, &UpdateRect, &ImageRect1);
                InvalidateRect(WindowHandle, &UpdateRect, 1);
                InvalidateRect(WindowHandle, &ImageRect1TopRight, 1);
 
                MouseXPos = NewMouseXPos;
                MouseYPos = NewMouseYPos;
            } else if(IsLeftClicked && IsInsideImageRect2) {
                int NewMouseXPos = GET_X_LPARAM(LParam);
                int NewMouseYPos = GET_Y_LPARAM(LParam);
                RECT UpdateRect = ImageRect2;
 
                OffsetRect(&ImageRect2, NewMouseXPos - MouseXPos, NewMouseYPos - MouseYPos);
                UnionRect(&UpdateRect, &UpdateRect, &ImageRect2);
                InvalidateRect(WindowHandle, &UpdateRect, 1);
                InvalidateRect(WindowHandle, &ImageRect2TopRight, 1);
 
                MouseXPos = NewMouseXPos;
                MouseYPos = NewMouseYPos;
            }
 
            break;
        
        default:
            return DefWindowProcA(WindowHandle, Message, WParam, LParam);
    }
    
    return 0;
}

///
///
///
/// COPIED CODE FROM STACK OVERFLOW
///
///
///
BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
    HDC hDC;
    int iBits;
    WORD wBitCount;
    DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
    BITMAP Bitmap0;
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    HANDLE fh, hDib, hPal, hOldPal2 = NULL;
    hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else
        wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap0.bmWidth;
    bi.biHeight = -Bitmap0.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 256;
    dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
        * Bitmap0.bmHeight;
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }


    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        + dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

    if (hOldPal2)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }

    fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

void GetScreenShot() {
    int x1, y1, x2, y2, w, h;

    x1  = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y1  = GetSystemMetrics(SM_YVIRTUALSCREEN) + 280;
    x2  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    y2  = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    w   = x2 - x1 - 1140;
    h   = y2 - y1 - 225;

    HDC     hScreen = GetDC(WindowHandle);
    HDC     hDC     = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL    bRet    = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);

    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();

    SaveHBITMAPToFile(hBitmap, "output.bmp");

    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);
}

char* display_opendialog() {

    /// FILE OPEN DIALOG BOX ///

    OPENFILENAME ofn;
    char szFile[260];
    HWND hwnd = WindowHandle;
    
    // handle to the file idk
    HANDLE hf;

    // similar to ofn = {0};
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize             = sizeof(ofn);
    ofn.hwndOwner               = hwnd;
    ofn.lpstrFile               = szFile;
    ofn.lpstrFile[0]            = '\0';
    ofn.nMaxFile                = sizeof(szFile);
    ofn.lpstrFilter             = ".BMP";
    ofn.nFilterIndex            = 1;
    ofn.nMaxFileTitle           = 0;
    ofn.lpstrInitialDir         = 0;
    ofn.lpstrInitialDir         = 0;
    ofn.Flags                   = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    GetOpenFileNameA(&ofn);

    // COPIED FROM CHATGPT

    char* result = 0;

    result = (char*)malloc(strlen(szFile) + 1);
    strcpy(result, szFile);

    return result;
}