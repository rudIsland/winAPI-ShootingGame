#include "CEngine.h"
#include "CTexture.h"
#include "Resource.h"

#include "CInputMgr.h"

#include <iostream>

//Static변수라서 전역공간에 초기화
HINSTANCE CEngine::hInst = nullptr;
//float CEngine::deltaTime = 0.0f;
float CEngine::mDeltaTime = 0.f;

CEngine::CEngine()
{
    //메모리 초기화 함수를 이용하여 C스타일 문자열 데이터 초기화
    memset(szTitle, 0, MAX_LOADSTRING * sizeof(WCHAR));
    memset(szWindowClass, 0, MAX_LOADSTRING * sizeof(WCHAR));
}

CEngine::~CEngine()
{

}

BOOL CEngine::Create(HINSTANCE hInstance, int nCmdShow, LPCWSTR tWindowClass, LPCWSTR tTitle)
{

    // 전역 문자열을 초기화합니다.
    /*LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPPGAME, szWindowClass, MAX_LOADSTRING);*/
    lstrcpyW(szWindowClass, tWindowClass);
    lstrcpyW(szTitle, tTitle);

    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}

MSG CEngine::Run()
{
    /* 그래픽 처리 시스템 생성 */
    //DC획득
    mhdc = GetDC(mhWnd);
    //GetDC함수는 어디에서라도 호출 가능하다. (윈도우 핸들이 필요)

    //후면 버퍼를 생성하고 설정한다.
    mBackBuffer = new CTexture();
    mBackBuffer->CreateBackBuffer(hInst, mhdc, ClientWidth, ClientHeight);

    /* 입력 처리 시스템 생성 */
    CInputMgr::GetInst();



    OnCreate();

    MSG msg = { 0 };


    QueryPerformanceFrequency(&mSecond);
    //<--초당 틱 갯수(빈도)구하기: 해당 cpu의 초당 틱 발생 갯수 구하기

    QueryPerformanceCounter(&mTime); //이전(시작할때)프레임에 틱
    //이 함수 호출 시점에 현재 틱 번호 알아내기

    while (WM_QUIT != msg.message)
    {
        LARGE_INTEGER tTime;
        QueryPerformanceCounter(&tTime); //현재 프레임에 틱 시간
        //이 함수 호출 시점에 현재 틱 번호 알아내기
        mDeltaTime = (float)(tTime.QuadPart - mTime.QuadPart) / (float)mSecond.QuadPart;

        mTime = tTime; //그 다음 프레임에도 시간 측정을 하기위해 현재 틱을 이전 틱으로 기억시켜 둔다.
 
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            CInputMgr::GetInst()->Update();
            OnUpdate();
        }
    }


    //게임세계 해지
    OnDestroy();

    //입력처리 시스템 해지
    CInputMgr::ReleaseInst();

    //후면버퍼를 정리하고 삭제한다.
    if (nullptr != mBackBuffer) {

        mBackBuffer->UnLoadTexture();

        delete mBackBuffer;
        mBackBuffer = nullptr;
    }


    //DC 반환
    ReleaseDC(mhWnd, mhdc);
    //ReleaseDC함수는 어디에서라도 호출 가능하다.


    return msg;
}

void CEngine::OnCreate()
{
    //로그를 출력
    OutputDebugString(L"CEngine::OnCreate\n");
}

void CEngine::OnDestroy()
{
    OutputDebugString(L"CEngine::OnDestroy\n");
}

void CEngine::OnUpdate()
{
    //OutputDebugString(L"CAPIEngine::OnUpdate\n");


}

void CEngine::Clear(float tR, float tG, float tB)
{
    //지역변수에 참조 핸들을 잠깐 담아두겠다.
    HDC thDC = mBackBuffer->GetDC();

    //자원 획득
    HPEN thOldPen = NULL; //여기서 사용할 펜 핸들
    HBRUSH thOldBrush = NULL; //여기서 사용할 브러쉬 핸들

    //자원 획득
    HPEN thPen = NULL; //여기서 사용할 펜 핸들
    HBRUSH thBrush = NULL; //여기서 사용할 브러쉬 핸들


    thPen = CreatePen(PS_SOLID, 2, RGB(tR * 255, tG * 255, tB * 255));
    thBrush = CreateSolidBrush(RGB(tR * 255, tG * 255, tB * 255));

    thOldPen = (HPEN)SelectObject(thDC, thPen);
    thOldBrush = (HBRUSH)SelectObject(thDC, thBrush);

    Rectangle(mBackBuffer->GetDC(), 0, 0, ClientWidth, ClientHeight);



    //자원반환
    //사용이 끝났으면 원래 설정으로 되돌린다.
    SelectObject(thDC, thOldPen);
    SelectObject(thDC, thOldBrush);


    DeleteObject(thBrush);
    DeleteObject(thPen);
}

void CEngine::Present()
{
    BitBlt(
        mhdc, //frontbuffer 현재화면DC
        0, 0,
        ClientWidth,
        ClientHeight,
        mBackBuffer->GetDC(), //backbuffer, 별도의 memoryDC
        0, 0,
        SRCCOPY);
}

void CEngine::DrawCircle(float x, float y, float radius) const
{
    //왼쪽위에서 아래로 내려오는 스크린의 중점을 기준으로 radius만큼의 원을 생각해서 대입
    Ellipse(mBackBuffer->GetDC(), x - radius, y - radius, x + radius, y + radius);
}

void CEngine::Drawtexture(float tx, float ty, CTexture* tTexture) const
{
    //전송 함수
     //메모리 복사 기능
    BitBlt(
        mBackBuffer->GetDC(), tx, ty,
        tTexture->GetBitmapInfo().bmWidth,
        tTexture->GetBitmapInfo().bmHeight,
        tTexture->GetDC(),
        0, 0,
        SRCCOPY);
}


void CEngine::DrawtextureScaled(float tX, float tY, CTexture* tTexture, float scale) const
{
    int srcW = tTexture->GetBitmapInfo().bmWidth;
    int srcH = tTexture->GetBitmapInfo().bmHeight;
    int dstW = (int)(srcW * scale);
    int dstH = (int)(srcH * scale);

    TransparentBlt(mBackBuffer->GetDC(),
        tX, tY,
        dstW, dstH,
        tTexture->GetDC(),
        0, 0,
        srcW, srcH,
        RGB(255, 255, 255)
    );
   /* StretchBlt(
        mBackBuffer->GetDC(), 
        tX, tY,
        dstW, dstH,
        tTexture->GetDC(),
        0, 0, 
        srcW, srcH,
        SRCCOPY);*/
}


/********************* Window API Code ***************************/

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
ATOM CEngine::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CPPGAME));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_CPPGAME);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL CEngine::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    // 윈도우 생성 (임시)
    mhWnd = CreateWindowW(szWindowClass, szTitle, 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // 크기조절 제거,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!mhWnd)
        return FALSE;

    // 클라이언트 기준 전체 윈도우 크기 계산
    RECT tRect = { 0, 0, ClientWidth, ClientHeight };
    AdjustWindowRect(&tRect, WS_OVERLAPPEDWINDOW, FALSE); //메뉴 없게 FALSE로 설정
    int windowWidth = tRect.right - tRect.left;
    int windowHeight = tRect.bottom - tRect.top;

    // 전체 스크린 사이즈 가져와서 정확히 중앙 계산
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - windowWidth) / 2 + 100;
    int posY = (screenHeight - windowHeight) / 2 + 100;

    // 위치와 크기 설정
    SetWindowPos(mhWnd, HWND_TOPMOST, posX, posY, windowWidth, windowHeight, 0);

    ShowWindow(mhWnd, nCmdShow);
    UpdateWindow(mhWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
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
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);


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

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK CEngine::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
