#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Constants
 
#define BUTTON_DISPLAYTEXT        69
#define BUTTON_SAVEIMAGE          420
#define BUTTON_OPENIMAGE          582
#define BUTTON_EXPORTFOLDER       555
#define IMAGE_RECTS_ARRAY_SIZE    5
#define DEBUG                     0
 
// Functions

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void             export_image();
char*            display_opendialog();

// Global Variables

char*     file_path = 0;
int       WindowWidth = 800;
int       WindowHeight = 900;
int       FontSize = 50;
int       image_handle_index = 0;
int       IsLeftClicked = 0;
int       IsInsideTextRect1 = 0;
int       IsInsideTextRect2 = 0;
int       IsInsideTextRect3 = 0;
int       IsInsideTextRect4 = 0;
int       IsInsideImageRect1 = 0;
int       IsInsideImageRect2 = 0;
int       is_inside_image_rect[IMAGE_RECTS_ARRAY_SIZE];
int       MouseXPos;
int       MouseYPos;
char      ToBeDisplayedText1[100];
char      ToBeDisplayedText2[100];
char      ToBeDisplayedText3[100];
char      ToBeDisplayedText4[100];
HWND      TextField1;
HWND      TextField2;
HWND      TextField3;
HWND      TextField4;
HFONT     WindowControlFont;
HFONT     WindowDrawFont;
HBRUSH    GreenBrush;
RECT      TextRect1;
RECT      TextRect2;
RECT      TextRect3;
RECT      TextRect4;
RECT      image_rects[IMAGE_RECTS_ARRAY_SIZE];
RECT      image_rects_top_right[IMAGE_RECTS_ARRAY_SIZE];
SIZE      SizeOfText1;
SIZE      SizeOfText2;
SIZE      SizeOfText3;
SIZE      SizeOfText4;
HDC       DeviceContextHandleImage;
HBITMAP   ImageHandle;
BITMAP    ImageHandleInfo;
HDC       DeviceContextHandleImage2;
HBITMAP   ImageHandle2;
BITMAP    ImageHandleInfo2;
HDC       device_context_image_handles[IMAGE_RECTS_ARRAY_SIZE];
HBITMAP   image_handles[IMAGE_RECTS_ARRAY_SIZE];
BITMAP    image_info_handles[IMAGE_RECTS_ARRAY_SIZE];
COLORREF  GreenColor = 0x0000FF00;
COLORREF  BlackColor = 0x00000000;
HWND      WindowHandle;
HINSTANCE instance_handle;