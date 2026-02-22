#include "CTexture.h"
#include "CEngine.h"

CTexture::CTexture()
{
    memset(&mBitmapInfo, 0, sizeof(BITMAP));
}
CTexture::~CTexture()
{

}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, LPCWSTR tFileName)
{
    mhDCMem = CreateCompatibleDC(hDC);

    // LoadImage 호출 전 파일 확인
    //OutputDebugString(L"Loading: ");
    //OutputDebugString(tFileName);
    //OutputDebugString(L"\n");

    //파일로부터 비트맵을 로드하여 비트맵 핸들을 얻는다. disk ---> memory
    mhBitmap = (HBITMAP)LoadImage(hInst, tFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // 실패 체크
        if (!mhBitmap) {
            DWORD error = GetLastError();
            WCHAR buffer[256];
            swprintf_s(buffer, L"LoadImage failed!: %d\n", error);
            OutputDebugString(buffer);
            return false; // 여기서 리턴
        }


    //비트맵의 정보를 얻어와 tBitmapInfo변수에 값을 써넣는다.
    GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);


    //사용하려는 새로운 비트맵을 설정한다. 
    //예전 기본 세팅됐던 비트맵을 리턴받는다.
    mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);


    return true;
}

void CTexture::UnLoadTexture()
{
    //원래 것으로 다시 설정해둔다.
    SelectObject(mhDCMem, mhOldBitmap);

    //사용했던 비트맵 을 삭제한다.
    DeleteObject(mhBitmap);

    DeleteDC(mhDCMem);
}

bool CTexture::CreateBackBuffer(HINSTANCE hInst, HDC hDC, int tClinetWidth, int tClinetHeight)
{
    mhDCMem = CreateCompatibleDC(hDC);

    //클라이언트 창 크기의 비트맵 데이터를 DDB로 만든다.
    mhBitmap = CreateCompatibleBitmap(hDC, tClinetWidth, tClinetHeight);

    //비트맵의 정보를 얻어와 tBitmapInfo변수에 값을 써넣는다.
    GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);


    //사용하려는 새로운 비트맵을 설정한다. 
    //예전 기본 세팅됐던 비트맵을 리턴받는다.
    mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);

    return true;
}
