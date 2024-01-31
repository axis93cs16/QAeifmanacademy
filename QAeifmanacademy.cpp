// QAeifmanacademy.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "QAeifmanacademy.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#include <stdio.h>
#include "resource.h"
#include <ddraw.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <winuser.h>
#include <atlstr.h>
#include <locale.h>
#include <tchar.h>
#include <cctype>
#include <algorithm> 
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include "atlbase.h"
#include "comutil.h"
#include <array>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND m_hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QAEIFMANACADEMY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QAEIFMANACADEMY));

    MSG msg;

    // Цикл основного сообщения:
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


LPWSTR convertStr(LPCSTR pInStr)
{
    int length = strlen(pInStr);
    wchar_t* pwstr = new wchar_t[length];
    int result = MultiByteToWideChar(
        CP_ACP, MB_PRECOMPOSED, pInStr, length,
        pwstr, length
    );
    pwstr[length] = L'\0';
    return LPWSTR(pwstr);
}
std::string convertLPWSTRtoStr(LPWSTR pInStr) 
{
    // Convert TCHAR array to std::wstring
    std::wstring wideString1(pInStr);

    // Convert std::wstring to std::string
    std::string narrowString1(wideString1.begin(), wideString1.end());
    return narrowString1;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QAEIFMANACADEMY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QAEIFMANACADEMY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
  
    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
static COLORREF bkcolor = RGB(255, 255, 255);
static COLORREF butcolor = RGB(255, 255, 255);
static COLORREF textcolor = RGB(255, 255, 255);
static COLORREF labelcolor = RGB(255, 255, 255);


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED,
      CW_USEDEFAULT, 0, 300, 200, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}


// trim from start
inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
inline std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
inline std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}
//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
HWND label1;
HWND text1;
HWND buttonok;
std::string tmptxt;
std::string tmptxtt;
std::string tmptxt2;

std::string getserialnumber(HWND hWnd, LPSTR cmda) {
    HANDLE hCin[2];
    HANDLE hCout[2];

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    CreatePipe(&hCin[0], &hCin[1], &sa, 0);
    SetHandleInformation(hCin[1], HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&hCout[0], &hCout[1], &sa, 0);
    SetHandleInformation(hCout[0], HANDLE_FLAG_INHERIT, 0);

    PROCESS_INFORMATION pi;

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdError = hCout[1];
    si.hStdOutput = hCout[1];
    si.hStdInput = hCin[0];
    si.wShowWindow = SW_HIDE;
    LPWSTR procname1 = convertStr(cmda);
    BOOL bProc = CreateProcess(NULL, procname1, NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &si, &pi);
    const int BUFSIZE = 4096;
    DWORD dRead, dWritten;
    CHAR chBuf[BUFSIZE] = { 0 };
    //const char* chBuf2 = "bios get serialnumber";
    //const char* chBuf3 = "exit";
    // если bProc==false - процесс не запустился.
    CloseHandle(hCout[1]);
    CloseHandle(hCin[0]);

    //Здесь пишем то, что хотим отправить дочернему процессу WriteFile(hCin[1]...
    //WriteFile(hCin[1], chBuf2, strlen(chBuf2), &dWritten, NULL);
    //WriteFile(hCin[1], chBuf3, strlen(chBuf3), &dWritten, NULL);

    DWORD dwExit = STILL_ACTIVE;
    do
    {
        GetExitCodeProcess(pi.hProcess, &dwExit);
        Sleep(1);
    } while (dwExit == STILL_ACTIVE);
    //MessageBoxA(hWnd, "ok2", "ok2", MB_OK);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    //Здесь читаем из дочернего процесса ReadFilr(hCout[0]

    BOOL bSuccess = FALSE;
    //MessageBoxA(hWnd, "ok1", "ok1", MB_OK);
    bSuccess = ReadFile(hCout[0], chBuf, BUFSIZE, &dRead, NULL);
    if (!bSuccess)
    {
        //printf("error reading :%d \n", GetLastError());
        //MessageBoxA(hWnd, "ReadError", "ok", MB_OK);
        //break;
    }
    //MessageBoxA(hWnd, chBuf, "ok", MB_OK);
    CloseHandle(hCout[0]);
    CloseHandle(hCin[1]);


    TerminateProcess(hCout, 0);
    TerminateProcess(hCin, 1);
    return chBuf;
}


inline bool exists_test(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}
std::string wstrtostr(const std::wstring& wstr)
{
    // Convert a Unicode string to an ASCII string
    std::string strTo;
    char* szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

std::wstring strtowstr(const std::string& str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t* wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}
TCHAR compname1[256] = TEXT("");
ULONG sz1 = 256;
TCHAR username1[256] = TEXT("");
ULONG sz2 = 256;
std::string serialn2s = "";
std::string serialn1s = "";
WNDPROC oldEditProc;

void saveresultik(HWND hWnd)
{
    int cTxtLen = GetWindowTextLength(text1);
    LPSTR bufftext = (PSTR)VirtualAlloc((LPVOID)NULL, (DWORD)(cTxtLen + 1), MEM_COMMIT, PAGE_READWRITE);
    GetWindowTextA(text1, bufftext, cTxtLen + 1);

    //СОХРАНИТЬ в ФАЙЛ//
    CString filename1;
    filename1 += "//server-files/shares/users/!RECYCLER/serials/";
    filename1 += compname1;
    filename1 += ".txt";
    std::ofstream file1(filename1);
    file1 << tmptxt;
    file1 << "\nOffice: ";
    file1 << bufftext;
    file1.close();
    //СОХРАНИТЬ в ФАЙЛ//
    //СОХРАНИТЬ в Общую базу//
    std::string filename2 = "//server-files/shares/users/!RECYCLER/serials/MainBaseSN.csv";
    std::string dheadcsv = "computername,username,bios,baseboard,office";
    std::string drowcsv = wstrtostr(compname1) + "," + wstrtostr(username1) + "," + serialn1s + "," + serialn2s + "," + bufftext;

    if (exists_test(filename2)) {
        std::ifstream file2;
        file2.open(filename2);
        std::string rowcsvstr;
        int rownumi = 0;
        int sizeofarr = 0;
        std::string filecontarr2[9999];
        while (std::getline(file2, rowcsvstr)) //костыль читаем весь файл в массив
        {
            filecontarr2[rownumi] = rowcsvstr;
            rownumi++;
        };
        file2.close();
        sizeofarr=rownumi;
        std::string *filecontarr = new std::string[sizeofarr];
        filecontarr[0] = "";
        std::copy(std::begin(filecontarr2), std::begin(filecontarr2) + sizeofarr, filecontarr);
        //delete[] filecontarr2;

        file2.open(filename2);

        char delimiter = ',';
        std::string headcsv1, headcsv2, headcsv3, headcsv4, headcsv5;
        std::string headinput;
        std::getline(file2, rowcsvstr);
        std::stringstream streamcsvrow(rowcsvstr);
        std::getline(streamcsvrow, headcsv1, delimiter);
        std::getline(streamcsvrow, headcsv2, delimiter);
        std::getline(streamcsvrow, headcsv3, delimiter);
        std::getline(streamcsvrow, headcsv4, delimiter);
        std::getline(streamcsvrow, headcsv5, delimiter);
        std::string valcsv1, valcsv2, valcsv3, valcsv4, valcsv5;
        int compexists = 0;
        rownumi = 0;

        /*if (headcsv1 == "computername") { MessageBoxA(hWnd, headcsv1.c_str(), "Спасибо!", MB_OK); };
        if (headcsv2 == "username") { MessageBoxA(hWnd, headcsv2.c_str(), "Спасибо!", MB_OK); };
        if (headcsv3 == "bios") { MessageBoxA(hWnd, headcsv3.c_str(), "Спасибо!", MB_OK); };
        if (headcsv4 == "baseboard") { MessageBoxA(hWnd, headcsv4.c_str(), "Спасибо!", MB_OK); };
        if (headcsv5 == "office") { MessageBoxA(hWnd, headcsv5.c_str(), "Спасибо!", MB_OK); };*/
        if (headcsv1 == "computername" && headcsv2 == "username" && headcsv3 == "bios" && headcsv4 == "baseboard" && headcsv5 == "office") {

            while (std::getline(file2, rowcsvstr))
            {
                rownumi++;
                std::stringstream streamcsvrow(rowcsvstr);
                getline(streamcsvrow, valcsv1, delimiter);
                getline(streamcsvrow, valcsv2, delimiter);
                getline(streamcsvrow, valcsv3, delimiter);
                getline(streamcsvrow, valcsv4, delimiter);
                getline(streamcsvrow, valcsv5, delimiter);
                if (valcsv1 == wstrtostr(compname1)) {
                    //MessageBoxA(hWnd, "Данные уже есть", "Спасибо!", MB_OK);
                    compexists = 1;
                    rowcsvstr = drowcsv;
                    //MessageBoxA(hWnd, rowcsvstr.c_str(), "Спасибо!", MB_OK);
                    filecontarr[rownumi] = rowcsvstr;                            
                }
                
            };
            if (compexists == 0) {
                failtowrite:
                file2.close();
                std::ofstream file2;
                file2.open(filename2, std::ios::in);
                if (!file2) {
                    //MessageBoxA(hWnd, "Ошибка записи в базу", "Спасибо!", MB_OK);
                    ShowWindow(m_hWnd, SW_HIDE);
                    Sleep(1500);
                    goto failtowrite;
                }
                file2.close();
                file2.open(filename2, std::ios::app);
                file2 << "\n";
                file2 << drowcsv;
                //MessageBoxA(hWnd, "Данные добавлены в базу", "Спасибо!", MB_OK);
            }
            else {
                failtowrite2:
                file2.close();
                std::ofstream file2;
                file2.open(filename2, std::ios::in);
                if (!file2) {
                    //MessageBoxA(hWnd, "Ошибка записи в базу корректировки", "Спасибо!", MB_OK);
                    ShowWindow(m_hWnd, SW_HIDE);
                    Sleep(1500);
                    goto failtowrite2;
                }
                file2.close();
                file2.open(filename2);
                int newlineflag = 0;
                //for (std::string linearr : filecontarr)
                for (size_t inuma=0; inuma < sizeofarr; inuma++)
                {
                    //MessageBoxA(hWnd, linearr.c_str(), "Спасибо!", MB_OK);
                    file2 << filecontarr[inuma]; //сюда пишем в файл массив
                    if (inuma < sizeofarr - 1) {
                        file2 << "\n";
                    }
                    /*if (newlineflag >= 5) {
                        newlineflag = 0;
                        file2 << "\n";
                    }
                    else { newlineflag++; };*/
                }
            }
        }
        else {
            CString msgstr;
            msgstr = "База повреждена сообщите Администратору";
            MessageBox(hWnd, msgstr, L"Спасибо!", MB_OK);
        };
        file2.close();
    } else {
        std::ofstream file2(filename2);
        file2 << dheadcsv;
        file2 << "\n";
        file2 << drowcsv;
        file2.close();
    };
    //СОХРАНИТЬ в Общую базу//
    tmptxt2 = "Сохраненные даные:\n" + tmptxt + "\nOffice: " + bufftext;//Вывод для сообщения
}

LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            //Do your stuff
            saveresultik(m_hWnd);
            DestroyWindow(m_hWnd);
            break;  //or return 0; if you don't want to pass it further to def proc
            //If not your key, skip to default:
        }
    default:
        return CallWindowProc(oldEditProc, wnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
       {
         m_hWnd = hWnd;
         label1 = CreateWindowA("STATIC", "Введите номер вашего кабинета", WS_VISIBLE | WS_CHILD, 10, 10, 250, 32, hWnd, NULL, hInst, NULL);
         text1 = CreateWindowA("EDIT", "", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 52, 250, 32, hWnd, NULL, hInst, NULL);
         buttonok = CreateWindowA("BUTTON", "OK", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 85, 94, 100, 32, hWnd, NULL, hInst, NULL);
         SetFocus(text1);
         
         oldEditProc = (WNDPROC)SetWindowLongPtr(text1, GWLP_WNDPROC, (LONG_PTR)subEditProc);

        std::string serialn1;
        std::string serialn2;

        int firstupwthis=0;

        GetComputerNameEx(ComputerNameDnsFullyQualified, compname1, &sz1);
        GetUserName(username1, &sz2);
        serialn1 = getserialnumber(hWnd, (LPSTR)"wmic.exe bios get serialnumber");
        serialn2 = getserialnumber(hWnd, (LPSTR)"wmic.exe baseboard get serialnumber");
        for (size_t i = 0; i < serialn1.length(); i++)
        {
            if (serialn1[i] == '\n')
            if (firstupwthis == 0)
            {// whatever
                serialn1s = serialn1.substr(i, serialn1.length() - i);
                trim(serialn1s);
                firstupwthis = 1;
            };
        };
        firstupwthis = 0;
        for (size_t i = 0; i < serialn2.length(); i++)
        {
            if (serialn2[i] == '\n')
                if (firstupwthis == 0)
                {// whatever
                    serialn2s = serialn2.substr(i, serialn2.length() - i);
                    trim(serialn2s);
                    firstupwthis = 1;
                };
        };
        bkcolor = RGB(240, 220, 220);
        InvalidateRect(hWnd, NULL, TRUE); //перерисовать форму

        //compname1 username1
        tmptxt = "Computername:" + convertLPWSTRtoStr(compname1) + "\nUsername:" + convertLPWSTRtoStr(username1) + "\nBIOS: " + serialn1s + "\nBaseboard: " + serialn2s;
        //TCHAR msg1[512] = TEXT("");
        //mbstowcs_s(NULL, msg1, (tmptxt.length() + 1), tmptxt.c_str(), (tmptxt.length()+1));
        //MessageBox(hWnd, msg1, TEXT("Msg title"), MB_OK | MB_ICONQUESTION);
           }
       break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        if ((HWND)lParam == buttonok) {
            saveresultik(hWnd);
            //MessageBoxA(hWnd, tmptxt2.c_str(), "Спасибо!", MB_OK);
            DestroyWindow(hWnd);
            break;
        };
        // Разобрать выбор в меню:
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
        break;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT rc;
            HDC hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rc);
            SetDCBrushColor(hdc, bkcolor);
            FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
            return 0;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        static HBRUSH hBrush = CreateSolidBrush(RGB(230, 230, 230));
    case WM_CTLCOLOREDIT:
        {
        HDC dc = (HDC)wParam;
        SetBkColor(dc,RGB(220, 220, 220));
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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

void main() {

}


