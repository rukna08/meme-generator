#include "meme_generator.h"
 
int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, LPSTR CommandLine, int CommandShow) {

    WNDCLASSEXA WindowClass = {0};
 
    WindowClass.cbSize =            sizeof(WNDCLASSEXA);
    WindowClass.lpfnWndProc =       WindowProc;
    WindowClass.hInstance =         Instance;
    WindowClass.hIcon =             LoadIconA(0, IDI_APPLICATION);
    WindowClass.hCursor =           LoadCursorA(0, IDC_ARROW);
    WindowClass.hbrBackground =     (HBRUSH)(COLOR_WINDOW+1);
    WindowClass.lpszClassName =     "MainWindowClass";
    WindowClass.hIconSm =           LoadIconA(0, IDI_APPLICATION);
    
    RegisterClassExA(&WindowClass);
    
    WindowHandle = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_COMPOSITED, WindowClass.lpszClassName, "Meme Generator", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, 0, 0, Instance, 0);
 
    TextField1 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10,  300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField2 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 50,  300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField3 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 90,  300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    TextField4 = CreateWindowA("EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 130, 300, 30, WindowHandle, 0, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    
    HWND DisplayTextButton =    CreateWindowA("BUTTON", "Display Text",     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,  170, 100, 30, WindowHandle, (HMENU)BUTTON_DISPLAYTEXT,  (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    HWND CopyImageButton   =    CreateWindowA("BUTTON", "Save Image",       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,  205, 100, 30, WindowHandle, (HMENU)BUTTON_SAVEIMAGE,    (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    HWND save_image_button =    CreateWindowA("BUTTON", "Open Image",       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,  240, 100, 30, WindowHandle, (HMENU)BUTTON_OPENIMAGE,    (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0);
    HWND export_folder_button = CreateWindowA("BUTTON", "Export Folder",    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 170, 100, 30, WindowHandle, (HMENU)BUTTON_EXPORTFOLDER, (HINSTANCE)GetWindowLongPtrA(WindowHandle, GWLP_HINSTANCE), 0); 

    NONCLIENTMETRICSA NonClientMetrics;
    NonClientMetrics.cbSize = sizeof(NonClientMetrics);
    
    SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(NonClientMetrics), &NonClientMetrics, 0);
    
    WindowControlFont = CreateFontIndirectA(&NonClientMetrics.lfMessageFont);
    
    NonClientMetrics.lfMessageFont.lfHeight = FontSize;
    
    WindowDrawFont = CreateFontIndirectA(&NonClientMetrics.lfMessageFont);
    
    for(int i = 0; i < IMAGE_RECTS_ARRAY_SIZE; i++) {
        image_rects[i].left = 400;
        image_rects[i].top  = 50;

        image_rects_top_right[i].left    = 0;
        image_rects_top_right[i].top     = 0;
        image_rects_top_right[i].right   = 0;
        image_rects_top_right[i].bottom  = 0;
    }
 
    SendMessage(TextField1, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField2, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField3, WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(TextField4, WM_SETFONT, (WPARAM)WindowControlFont, 1);
 
    SendMessage(DisplayTextButton,    WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(CopyImageButton,      WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(save_image_button,    WM_SETFONT, (WPARAM)WindowControlFont, 1);
    SendMessage(export_folder_button, WM_SETFONT, (WPARAM)WindowControlFont, 1);

    for(int i = 0; i < IMAGE_RECTS_ARRAY_SIZE; i++) {
        device_context_image_handles[i] = CreateCompatibleDC(0);
        SelectObject(device_context_image_handles[i], image_handles[i]);
    }
    
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

            if(LOWORD(WParam) == BUTTON_EXPORTFOLDER) {
                
                int end_index = strlen(file_path) - 1;
                while(1) {
                    if(file_path[end_index] == 47) break;
                    end_index--;
                }

                char export_path[MAX_PATH];
                for(int i = 0; i <= end_index + 1; i++) {
                    if(i == end_index + 1) {
                        export_path[i] = '\0';
                    } else {
                        export_path[i] = file_path[i];
                    }
                }

                ShellExecuteA(WindowHandle, "open", export_path, 0, 0, SW_SHOWDEFAULT);
            }

            if(LOWORD(WParam) == BUTTON_OPENIMAGE) {
                file_path = display_opendialog();
                for(int i = 0; file_path[i] != '\0'; i++) {
                    if(file_path[i] == 92) {
                        file_path[i] = 47;
                    }
                }

                image_handles[image_handle_index] = (HBITMAP)LoadImageA(0, file_path, 0, 0, 0, LR_LOADFROMFILE);

                GetObject(image_handles[image_handle_index], sizeof(BITMAP), &image_info_handles[image_handle_index]);
                SelectObject(device_context_image_handles[image_handle_index], image_handles[image_handle_index]);

                image_handle_index++;
                
                UpdateWindow(WindowHandle);
            }
 
            if(LOWORD(WParam) == BUTTON_SAVEIMAGE) {
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
 
                export_image();

                MessageBoxA(0, "Image exported.", "Info", MB_OK | MB_ICONINFORMATION);
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
 
            for(int i = 0; i < image_handle_index; i++) {
                image_rects[i].right  = image_rects[i].left + DestWidth;
                image_rects[i].bottom = image_rects[i].top  + ((DestWidth * image_info_handles[i].bmHeight) / image_info_handles[i].bmWidth);

                image_rects_top_right[i].left    = image_rects[i].right - 10;
                image_rects_top_right[i].top     = image_rects[i].top   - 10;
                image_rects_top_right[i].right   = image_rects[i].right + 10;
                image_rects_top_right[i].bottom  = image_rects[i].top   + 10;

                SetStretchBltMode(DeviceContextHandle, HALFTONE);
                StretchBlt(DeviceContextHandle, image_rects[i].left, image_rects[i].top, image_rects[i].right - image_rects[i].left, image_rects[i].bottom - image_rects[i].top, device_context_image_handles[i], 0, 0, image_info_handles[i].bmWidth, image_info_handles[i].bmHeight, SRCCOPY);
            }

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

            for(int i = 0; i < IMAGE_RECTS_ARRAY_SIZE; i++) {
                is_inside_image_rect[i] = PtInRect(&image_rects[i], MouseLocation);
            }
            
            break;
 
        case WM_LBUTTONUP:
            IsLeftClicked = 0;
            IsInsideTextRect1 = 0;
            IsInsideTextRect2 = 0;
            IsInsideTextRect3 = 0;
            IsInsideTextRect4 = 0;
 
            IsInsideImageRect1 = 0;
            IsInsideImageRect2 = 0;

            for(int i = 0; i < IMAGE_RECTS_ARRAY_SIZE; i++) {
                is_inside_image_rect[i] = 0;
            }
 
            break;
 
        case WM_MOUSEMOVE:        
            POINT MouseLocation2 = {GET_X_LPARAM(LParam), GET_Y_LPARAM(LParam)};

            for(int i = 0; i < image_handle_index; i++) {
                if(PtInRect(&image_rects[i], MouseLocation2)) {
                    SetCursor(LoadCursorA(0, IDC_SIZEALL));
                }
            }
 
            if(PtInRect(&TextRect1, MouseLocation2) || PtInRect(&TextRect2, MouseLocation2) || PtInRect(&TextRect3, MouseLocation2) || PtInRect(&TextRect4, MouseLocation2)) {
                SetCursor(LoadCursorA(0, IDC_SIZEALL));
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
            }

            for(int i = 0; i < IMAGE_RECTS_ARRAY_SIZE; i++) {
                if(IsLeftClicked && is_inside_image_rect[i]) {
                    int new_mousex_pos = GET_X_LPARAM(LParam);
                    int new_mousey_pos = GET_Y_LPARAM(LParam);
                    RECT updated_rect  = image_rects[i];
                    
                    OffsetRect(&image_rects[i], new_mousex_pos - MouseXPos, new_mousey_pos - MouseYPos);
                    UnionRect(&updated_rect, &updated_rect, &image_rects[i]);
                    InvalidateRect(WindowHandle, &updated_rect, 1);
                    InvalidateRect(WindowHandle, &image_rects_top_right[i], 1);
                    
                    MouseXPos = new_mousex_pos;
                    MouseYPos = new_mousey_pos;
                }
            }
 
            break;
        
        default:
            return DefWindowProcA(WindowHandle, Message, WParam, LParam);
    }
    
    return 0;
}

BOOL save_hbitmap_to_file(HBITMAP hBitmap, LPCTSTR lpszFileName)
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

void export_image() {

    int     x1               = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int     y1               = GetSystemMetrics(SM_YVIRTUALSCREEN) + 280;
    int     x2               = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int     y2               = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int     w                = x2 - x1 - 1140;
    int     h                = y2 - y1 - 225;
    HDC     hScreen          = GetDC(WindowHandle);
    HDC     hDC              = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap          = CreateCompatibleBitmap(hScreen, w, h);
    HGDIOBJ old_obj          = SelectObject(hDC, hBitmap);
    BOOL    bRet             = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);

    save_hbitmap_to_file(hBitmap, "output.bmp");

    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);

}

char* display_opendialog() {

    OPENFILENAME ofn;
    char         szFile[260];
    HWND         hwnd = WindowHandle;
    HANDLE       hf;

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

    char* result = 0;
    result = (char*)malloc(strlen(szFile) + 1);
    strcpy(result, szFile);
    return result;

}