// Calculator-WINAPI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Calculator-WINAPI.h"
#include <iostream>
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
UINT WIDTH = 350;                               // the main window width
UINT HEIGHT = 560;                              // the main window height

HWND labelResult;

char OPERATION = ' '; 
bool OPERATION_SIDE = false; 
std::string FIRST_NUMBER_STRING;
std::string SECOND_NUMBER_STRING;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Info(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CALCULATORWINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CALCULATORWINAPI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALCULATORWINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CALCULATORWINAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_CALCULATORWINAPI));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
     CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // Calculate the screen width and height
   int screenWidth = GetSystemMetrics(SM_CXSCREEN);
   int screenHeight = GetSystemMetrics(SM_CYSCREEN);

   // Get the window dimensions
   RECT windowRect;
   GetWindowRect(hWnd, &windowRect);
   int windowWidth = windowRect.right - windowRect.left;
   int windowHeight = windowRect.bottom - windowRect.top;

   // Calculate the position for centering
   int xPos = (screenWidth - windowWidth) / 2;
   int yPos = (screenHeight - windowHeight) / 2;

   // Set the window position
   SetWindowPos(hWnd, nullptr, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);


   labelResult = CreateWindowW(L"STATIC", L"0", WS_VISIBLE | WS_CHILDWINDOW | SS_RIGHT | WS_BORDER,
     0, 0, 334, 50, hWnd, (HMENU)LABEL_RESULT, hInstance, nullptr);

   if (!labelResult)
   {
     return FALSE;
   }

   // First, create a new font
   HFONT hFont = CreateFont(48,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
   HFONT hFontButtonPlus = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
   HFONT hFontButton = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

   // Next, set the new font to the static control
   SendMessage(labelResult, WM_SETFONT, (WPARAM)hFont, TRUE);

   HWND buttonPercentage = CreateWindowW(L"Button", L"%", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     0, 50, 75, 75, hWnd, (HMENU)BUTTON_PERCENTAGE_CLICKED, hInstance, nullptr);

   if (!buttonPercentage)
   {
     return FALSE;
   }

   SendMessage(buttonPercentage, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonClearEverything = CreateWindowW(L"Button", L"CE", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77, 50, 75, 75, hWnd, (HMENU)BUTTON_CLEAR_EVERYTHING_CLICKED, hInstance, nullptr);

   if (!buttonClearEverything)
   {
     return FALSE;
   }

   SendMessage(buttonClearEverything, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonClear = CreateWindowW(L"Button", L"C", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77 * 2, 50, 75, 75, hWnd, (HMENU)BUTTON_CLEAR_CLICKED, hInstance, nullptr);

   if (!buttonClear)
   {
     return FALSE;
   }

   SendMessage(buttonClear, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonRemoveCharacter = CreateWindowW(L"Button", L"←", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     230, 50, 105, 75, hWnd, (HMENU)BUTTON_REMOVE_CHARACTER_CLICKED, hInstance, nullptr);

   if (!buttonRemoveCharacter)
   {
     return FALSE;
   }

   SendMessage(buttonRemoveCharacter, WM_SETFONT, (WPARAM)hFontButtonPlus, TRUE);

   HWND buttonOneOverX = CreateWindowW(L"Button", L"1/x", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     0, 125, 75, 75, hWnd, (HMENU)BUTTON_ONE_OVER_X_CLICKED, hInstance, nullptr);

   if (!buttonOneOverX)
   {
     return FALSE;
   }

   SendMessage(buttonOneOverX, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonPowerTwo = CreateWindowW(L"Button", L"x²", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77, 125, 75, 75, hWnd, (HMENU)BUTTON_X_POWER_TWO_CLICKED, hInstance, nullptr);

   if (!buttonPowerTwo)
   {
     return FALSE;
   }

   SendMessage(buttonPowerTwo, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonTwoSquaredX = CreateWindowW(L"Button", L"√", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77 * 2, 125, 75, 75, hWnd, (HMENU)BUTTON_SQUARE_ROOT_CLICKED, hInstance, nullptr);

   if (!buttonTwoSquaredX)
   {
     return FALSE;
   }

   SendMessage(buttonTwoSquaredX, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonDivide = CreateWindowW(L"Button", L"/", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     230, 125, 105, 75, hWnd, (HMENU)BUTTON_DIVIDE_CLICKED, hInstance, nullptr);

   if (!buttonDivide)
   {
     return FALSE;
   }

   SendMessage(buttonDivide, WM_SETFONT, (WPARAM)hFontButtonPlus, TRUE);

   HWND buttonSeven = CreateWindowW(L"Button", L"7", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     0, 200, 75, 75, hWnd, (HMENU)BUTTON_SEVEN_CLICKED, hInstance, nullptr);

   if (!buttonSeven)
   {
     return FALSE;
   }

   SendMessage(buttonSeven, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonEight = CreateWindowW(L"Button", L"8", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77, 200, 75, 75, hWnd, (HMENU)BUTTON_EIGHT_CLICKED, hInstance, nullptr);

   if (!buttonEight)
   {
     return FALSE;
   }

   SendMessage(buttonEight, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonNine = CreateWindowW(L"Button", L"9", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77*2, 200, 75, 75, hWnd, (HMENU)BUTTON_NINE_CLICKED, hInstance, nullptr);

   if (!buttonNine)
   {
     return FALSE;
   }

   SendMessage(buttonNine, WM_SETFONT, (WPARAM)hFontButton, TRUE);


   HWND buttonPlus = CreateWindowW(L"Button", L"+", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     230, 200, 105, 75, hWnd, (HMENU)BUTTON_PLUS_CLICKED, hInstance, nullptr);

   if (!buttonPlus)
   {
     return FALSE;
   }

   SendMessage(buttonPlus, WM_SETFONT, (WPARAM)hFontButtonPlus, TRUE);

   HWND buttonFour = CreateWindowW(L"Button", L"4", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     0, 275, 75, 75, hWnd, (HMENU)BUTTON_FOUR_CLICKED, hInstance, nullptr);

   if (!buttonFour)
   {
     return FALSE;
   }

   SendMessage(buttonFour, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonFive = CreateWindowW(L"Button", L"5", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77, 275, 75, 75, hWnd, (HMENU)BUTTON_FIVE_CLICKED, hInstance, nullptr);

   if (!buttonFive)
   {
     return FALSE;
   }

   SendMessage(buttonFive, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonSix = CreateWindowW(L"Button", L"6", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77 * 2, 275, 75, 75, hWnd, (HMENU)BUTTON_SIX_CLICKED, hInstance, nullptr);

   if (!buttonSix)
   {
     return FALSE;
   }

   SendMessage(buttonSix, WM_SETFONT, (WPARAM)hFontButton, TRUE);


   HWND buttonMinus = CreateWindowW(L"Button", L"-", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     230, 275, 105, 75, hWnd, (HMENU)BUTTON_MINUS_CLICKED, hInstance, nullptr);

   if (!buttonMinus)
   {
     return FALSE;
   }

   SendMessage(buttonMinus, WM_SETFONT, (WPARAM)hFontButtonPlus, TRUE);

   HWND buttonOne = CreateWindowW(L"Button", L"1", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     0, 350, 75, 75, hWnd, (HMENU)BUTTON_ONE_CLICKED, hInstance, nullptr);

   if (!buttonOne)
   {
     return FALSE;
   }

   SendMessage(buttonOne, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonTwo = CreateWindowW(L"Button", L"2", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77, 350, 75, 75, hWnd, (HMENU)BUTTON_TWO_CLICKED, hInstance, nullptr);

   if (!buttonTwo)
   {
     return FALSE;
   }

   SendMessage(buttonTwo, WM_SETFONT, (WPARAM)hFontButton, TRUE);


   HWND buttonThree = CreateWindowW(L"Button", L"3", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77 * 2, 350, 75, 75, hWnd, (HMENU)BUTTON_THREE_CLICKED, hInstance, nullptr);

   if (!buttonThree)
   {
     return FALSE;
   }

   SendMessage(buttonThree, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonMultiply = CreateWindowW(L"Button", L"×", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     230, 350, 105, 75, hWnd, (HMENU)BUTTON_MULTIPLY_CLICKED, hInstance, nullptr);

   if (!buttonMultiply)
   {
     return FALSE;
   }

   SendMessage(buttonMultiply, WM_SETFONT, (WPARAM)hFontButtonPlus, TRUE);

   HWND buttonChangeAbs = CreateWindowW(L"Button", L"+/-", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     0, 425, 75, 75, hWnd, (HMENU)BUTTON_ABS_CLICKED, hInstance, nullptr);

   if (!buttonChangeAbs)
   {
     return FALSE;
   }

   SendMessage(buttonChangeAbs, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonZero = CreateWindowW(L"Button", L"0", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77, 425, 75, 75, hWnd, (HMENU)BUTTON_ZERO_CLICKED, hInstance, nullptr);

   if (!buttonZero)
   {
     return FALSE;
   }

   SendMessage(buttonZero, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonComma = CreateWindowW(L"Button", L",", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     77 * 2, 425, 75, 75, hWnd, (HMENU)BUTTON_COMMA_CLICKED, hInstance, nullptr);

   if (!buttonComma)
   {
     return FALSE;
   }

   SendMessage(buttonComma, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   HWND buttonEquals = CreateWindowW(L"Button", L"=", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER | WS_BORDER,
     230, 425, 105, 75, hWnd, (HMENU)BUTTON_EQUALS_CLICKED, hInstance, nullptr);

   if (!buttonEquals)
   {
     return FALSE;
   }

   SendMessage(buttonEquals, WM_SETFONT, (WPARAM)hFontButton, TRUE);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN: {
      switch (wParam)
      {
        case VK_BACK: {
          if (!OPERATION_SIDE)
          {
            if (FIRST_NUMBER_STRING.length() != 0)
            {
              FIRST_NUMBER_STRING = FIRST_NUMBER_STRING.substr(0, FIRST_NUMBER_STRING.length() - 1);
              std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
              LPCWSTR sw = stemp.c_str();
              SetWindowTextW(labelResult, sw);
              return DefWindowProc(hWnd, message, wParam, lParam);
            }
            else
            {
              SetWindowTextW(labelResult, L"0");
              return DefWindowProc(hWnd, message, wParam, lParam);
            }
          }
          else
          {
            if (SECOND_NUMBER_STRING.length() != 0)
            {
              SECOND_NUMBER_STRING = SECOND_NUMBER_STRING.substr(0, SECOND_NUMBER_STRING.length() - 1);
              std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
              LPCWSTR sw = stemp.c_str();
              SetWindowTextW(labelResult, sw);
              return DefWindowProc(hWnd, message, wParam, lParam);
            }
            else
            {
              OPERATION = ' ';
              OPERATION_SIDE = false;
              SetWindowTextW(labelResult, L"0");
              return DefWindowProc(hWnd, message, wParam, lParam);
            }
          }
          return DefWindowProc(hWnd, message, wParam, lParam);
        }
        case VK_OEM_PLUS: {
          if (OPERATION_SIDE)
          {
            return DefWindowProc(hWnd, message, wParam, lParam);
          }
          OPERATION = '+';
          OPERATION_SIDE = true;
          SetWindowTextW(labelResult, L"+");
          ::SetFocus(hWnd);
          return DefWindowProc(hWnd, message, wParam, lParam);
          break;
        }
        case VK_OEM_MINUS: {
          if (OPERATION_SIDE)
          {
            return DefWindowProc(hWnd, message, wParam, lParam);
          }
          OPERATION = '-';
          OPERATION_SIDE = true;
          SetWindowTextW(labelResult, L"-");
          ::SetFocus(hWnd);
          return DefWindowProc(hWnd, message, wParam, lParam);
          break;
        }
        case VK_OEM_COMMA: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += '.';
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += '.';
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          return DefWindowProc(hWnd, message, wParam, lParam);
          break;
        }
        case VK_OEM_2: {
          if (OPERATION_SIDE)
          {
            return DefWindowProc(hWnd, message, wParam, lParam);
          }
          OPERATION = '/';
          OPERATION_SIDE = true;
          SetWindowTextW(labelResult, L"/");
          ::SetFocus(hWnd);
          return DefWindowProc(hWnd, message, wParam, lParam);
          break;
        }
        case VK_RETURN: {
          try
          {
            if (FIRST_NUMBER_STRING.length() != 0 && SECOND_NUMBER_STRING.length() != 0 && OPERATION != ' ')
            {
              switch (OPERATION)
              {
              case '+': {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                long double RESULT = FIRST_NUMBER + SECOND_NUMBER;
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                break;
              }
              case '-': {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                long double RESULT = FIRST_NUMBER - SECOND_NUMBER;
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                break;
              }
              case '/': {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                long double RESULT = FIRST_NUMBER / SECOND_NUMBER;
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                break;
              }
              case '*': {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                long double RESULT = FIRST_NUMBER * SECOND_NUMBER;
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                break;
              }
              default:
                break;
              }
            }
          }
          catch (const std::exception error)
          {
            std::string str = error.what();
            std::wstring stemp = std::wstring(str.begin(), str.end());
            LPCWSTR sw = stemp.c_str();
            MessageBox(hWnd, sw, L"ERROR!", MB_OK);
            SetWindowTextW(labelResult, L"");
            FIRST_NUMBER_STRING = "";
            SECOND_NUMBER_STRING = "";
            OPERATION = ' ';
            OPERATION_SIDE = false;
          }
          ::SetFocus(hWnd);
          return DefWindowProc(hWnd, message, wParam, lParam);
          break;
        }
        case VK_MULTIPLY: {
          if (OPERATION_SIDE)
          {
            return DefWindowProc(hWnd, message, wParam, lParam);
          }
          OPERATION = '*';
          OPERATION_SIDE = true;
          SetWindowTextW(labelResult, L"*");
          ::SetFocus(hWnd);
          return DefWindowProc(hWnd, message, wParam, lParam);
          break;
        }
        //0x30 -> VK_KEY 0 and so on...
        case 0x30: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(0);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(0);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x31: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(1);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(1);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x32: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(2);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(2);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x33: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(3);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(3);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x34: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(4);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(4);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x35: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(5);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(5);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x36: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(6);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(6);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          break;
        }
        case 0x37: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(7);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(7);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x38: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(8);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(8);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        case 0x39: {
          if (!OPERATION_SIDE) // first side
          {
            FIRST_NUMBER_STRING += std::to_string(9);
            std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          else
          {
            SECOND_NUMBER_STRING += std::to_string(9);
            std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
            LPCWSTR sw = stemp.c_str();
            SetWindowTextW(labelResult, sw);
          }
          ::SetFocus(hWnd);
          break;
        }
        default:
          break;
      }
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_HELP_INFO:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_HELPFUL_INFO), hWnd, Info);
              break;
            case BUTTON_ABS_CLICKED: {
              if (!OPERATION_SIDE)
              {
                if (!FIRST_NUMBER_STRING.empty() && FIRST_NUMBER_STRING[0] == '-') {
                  FIRST_NUMBER_STRING.erase(FIRST_NUMBER_STRING.begin()); // Remove the minus sign
                }
                else {
                  FIRST_NUMBER_STRING = '-' + FIRST_NUMBER_STRING; // Add the minus sign
                }
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                ::SetFocus(hWnd);
              }
              else 
              {
                if (!SECOND_NUMBER_STRING.empty() && SECOND_NUMBER_STRING[0] == '-') {
                  SECOND_NUMBER_STRING.erase(SECOND_NUMBER_STRING.begin()); // Remove the minus sign
                }
                else {
                  SECOND_NUMBER_STRING = '-' + SECOND_NUMBER_STRING; // Add the minus sign
                }
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                ::SetFocus(hWnd);
              }
              break;
            }
            case BUTTON_PERCENTAGE_CLICKED: {
              try
              {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                long double RESULT = FIRST_NUMBER / SECOND_NUMBER;
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              catch (const std::exception error)
              {
                std::string str = error.what();
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                MessageBox(hWnd, sw, L"ERROR!", MB_OK);
                SetWindowTextW(labelResult, L"0");
                FIRST_NUMBER_STRING = "";
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              break;
            }
            case BUTTON_CLEAR_EVERYTHING_CLICKED: {
              FIRST_NUMBER_STRING = "";
              SECOND_NUMBER_STRING = "";
              OPERATION = ' ';
              OPERATION_SIDE = false;
              SetWindowText(labelResult, L"0");
              break;
            }
            case BUTTON_CLEAR_CLICKED: {
              if (!OPERATION_SIDE)
              {
                FIRST_NUMBER_STRING = "";
                SetWindowText(labelResult, L"0");
              }
              else 
              {
                SECOND_NUMBER_STRING = "";
                SetWindowText(labelResult, L"0");
              }
              break;
            }
            case BUTTON_REMOVE_CHARACTER_CLICKED: {
              if (!OPERATION_SIDE)
              {
                if (FIRST_NUMBER_STRING.length() != 0)
                {
                  FIRST_NUMBER_STRING = FIRST_NUMBER_STRING.substr(0, FIRST_NUMBER_STRING.length() - 1);
                  std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                  LPCWSTR sw = stemp.c_str();
                  SetWindowTextW(labelResult, sw);
                  return DefWindowProc(hWnd, message, wParam, lParam);
                }
                else
                {
                  SetWindowTextW(labelResult, L"0");
                  return DefWindowProc(hWnd, message, wParam, lParam);
                }
              }
              else
              {
                if (SECOND_NUMBER_STRING.length() != 0)
                {
                  SECOND_NUMBER_STRING = SECOND_NUMBER_STRING.substr(0, SECOND_NUMBER_STRING.length() - 1);
                  std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                  LPCWSTR sw = stemp.c_str();
                  SetWindowTextW(labelResult, sw);
                  return DefWindowProc(hWnd, message, wParam, lParam);
                }
                else
                {
                  OPERATION = ' ';
                  OPERATION_SIDE = false;
                  SetWindowTextW(labelResult, L"0");
                  return DefWindowProc(hWnd, message, wParam, lParam);
                }
              }
              break;
            }
            case BUTTON_ONE_OVER_X_CLICKED: {
              try
              {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double RESULT = 1 / FIRST_NUMBER;
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              catch (const std::exception error)
              {
                std::string str = error.what();
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                MessageBox(hWnd, sw, L"ERROR!", MB_OK);
                SetWindowTextW(labelResult, L"0");
                FIRST_NUMBER_STRING = "";
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              break;
            }
            case BUTTON_X_POWER_TWO_CLICKED: {
              try
              {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double RESULT = pow(FIRST_NUMBER, 2);
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              catch (const std::exception error)
              {
                std::string str = error.what();
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                MessageBox(hWnd, sw, L"ERROR!", MB_OK);
                SetWindowTextW(labelResult, L"0");
                FIRST_NUMBER_STRING = "";
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              break;
            }
            case BUTTON_SQUARE_ROOT_CLICKED: {
              try
              {
                long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                long double RESULT = sqrt(FIRST_NUMBER);
                std::string str = std::to_string(RESULT);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
                FIRST_NUMBER_STRING = str;
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              catch (const std::exception error)
              {
                std::string str = error.what();
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                MessageBox(hWnd, sw, L"ERROR!", MB_OK);
                SetWindowTextW(labelResult, L"0");
                FIRST_NUMBER_STRING = "";
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
                ::SetFocus(hWnd);
              }
              break;
            }
            case BUTTON_SEVEN_CLICKED: {
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(7);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else 
              {
                SECOND_NUMBER_STRING += std::to_string(7);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            }
            case BUTTON_EIGHT_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(8);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(8);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_NINE_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(9);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(9);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_PLUS_CLICKED: {
              if (OPERATION_SIDE)
              {
                return DefWindowProc(hWnd, message, wParam, lParam);
              }
              OPERATION = '+';
              OPERATION_SIDE = true;
              SetWindowTextW(labelResult, L"+");
              ::SetFocus(hWnd);
              break;
            }
            case BUTTON_FOUR_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(4);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(4);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_FIVE_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(5);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(5);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_SIX_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(6);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(6);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_MINUS_CLICKED: {
              if (OPERATION_SIDE)
              {
                return DefWindowProc(hWnd, message, wParam, lParam);
              }
              OPERATION = '-';
              OPERATION_SIDE = true;
              SetWindowTextW(labelResult, L"-");
              ::SetFocus(hWnd);
              break;
            }
            case BUTTON_ONE_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(1);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(1);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_TWO_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(2);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(2);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_THREE_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(3);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(3);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_DIVIDE_CLICKED: {
              if (OPERATION_SIDE)
              {
                return DefWindowProc(hWnd, message, wParam, lParam);
              }
              OPERATION = '/';
              OPERATION_SIDE = true;
              SetWindowTextW(labelResult, L"/");
              ::SetFocus(hWnd);
              break;
            }
            case BUTTON_COMMA_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += '.';
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += '.';
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_ZERO_CLICKED:
              if (!OPERATION_SIDE) // first side
              {
                FIRST_NUMBER_STRING += std::to_string(0);
                std::wstring stemp = std::wstring(FIRST_NUMBER_STRING.begin(), FIRST_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              else
              {
                SECOND_NUMBER_STRING += std::to_string(0);
                std::wstring stemp = std::wstring(SECOND_NUMBER_STRING.begin(), SECOND_NUMBER_STRING.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowTextW(labelResult, sw);
              }
              ::SetFocus(hWnd);
              break;
            case BUTTON_EQUALS_CLICKED: {
              try
              {
                if (FIRST_NUMBER_STRING.length() != 0 && SECOND_NUMBER_STRING.length() != 0 && OPERATION != ' ')
                {
                  switch (OPERATION)
                  {
                  case '+': {
                    long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                    long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                    long double RESULT = FIRST_NUMBER + SECOND_NUMBER;
                    std::string str = std::to_string(RESULT);
                    std::wstring stemp = std::wstring(str.begin(), str.end());
                    LPCWSTR sw = stemp.c_str();
                    SetWindowTextW(labelResult, sw);
                    FIRST_NUMBER_STRING = str;
                    SECOND_NUMBER_STRING = "";
                    OPERATION = ' ';
                    OPERATION_SIDE = false; 
                    break;
                  }
                  case '-': {
                    long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                    long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                    long double RESULT = FIRST_NUMBER - SECOND_NUMBER;
                    std::string str = std::to_string(RESULT);
                    std::wstring stemp = std::wstring(str.begin(), str.end());
                    LPCWSTR sw = stemp.c_str();
                    SetWindowTextW(labelResult, sw);
                    FIRST_NUMBER_STRING = str;
                    SECOND_NUMBER_STRING = "";
                    OPERATION = ' ';
                    OPERATION_SIDE = false;
                    break;
                  }
                  case '/': {
                    long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                    long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                    long double RESULT = FIRST_NUMBER / SECOND_NUMBER;
                    std::string str = std::to_string(RESULT);
                    std::wstring stemp = std::wstring(str.begin(), str.end());
                    LPCWSTR sw = stemp.c_str();
                    SetWindowTextW(labelResult, sw);
                    FIRST_NUMBER_STRING = str;
                    SECOND_NUMBER_STRING = "";
                    OPERATION = ' ';
                    OPERATION_SIDE = false;
                    break;
                  }
                  case '*': {
                    long double FIRST_NUMBER = ::atof(FIRST_NUMBER_STRING.c_str());
                    long double SECOND_NUMBER = ::atof(SECOND_NUMBER_STRING.c_str());
                    long double RESULT = FIRST_NUMBER * SECOND_NUMBER;
                    std::string str = std::to_string(RESULT);
                    std::wstring stemp = std::wstring(str.begin(), str.end());
                    LPCWSTR sw = stemp.c_str();
                    SetWindowTextW(labelResult, sw);
                    FIRST_NUMBER_STRING = str;
                    SECOND_NUMBER_STRING = "";
                    OPERATION = ' ';
                    OPERATION_SIDE = false;
                    break;
                  }
                  default:
                    break;
                  }
                }
              }
              catch (const std::exception error)
              {
                std::string str = error.what();
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();
                MessageBox(hWnd, sw, L"ERROR!", MB_OK);
                SetWindowTextW(labelResult, L"");
                FIRST_NUMBER_STRING = "";
                SECOND_NUMBER_STRING = "";
                OPERATION = ' ';
                OPERATION_SIDE = false;
              }
              ::SetFocus(hWnd);
              break;
            }
            case BUTTON_MULTIPLY_CLICKED: {
              if (OPERATION_SIDE)
              {
                return DefWindowProc(hWnd, message, wParam, lParam);
              }
              OPERATION = '*';
              OPERATION_SIDE = true;
              SetWindowTextW(labelResult, L"*");
              ::SetFocus(hWnd);
              break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_GETMINMAXINFO:
    {
      MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;

      // Set minimum width and height
      pMinMaxInfo->ptMinTrackSize.x = WIDTH;
      pMinMaxInfo->ptMinTrackSize.y = HEIGHT;

      // Set maximum width and height
      pMinMaxInfo->ptMaxTrackSize.x = WIDTH;
      pMinMaxInfo->ptMaxTrackSize.y = HEIGHT;

      return 0;
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
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

INT_PTR CALLBACK Info(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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