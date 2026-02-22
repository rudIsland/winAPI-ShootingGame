#pragma once

#include <windows.h>

class CTexture
{
private:
	HDC mhDCMem = NULL;
	HBITMAP mhBitmap = NULL;
	BITMAP mBitmapInfo;
	HBITMAP mhOldBitmap = NULL;


public:
	CTexture();
	~CTexture();

	// 복사/대입 금지
	CTexture(const CTexture&) = delete;
	CTexture& operator=(const CTexture&) = delete;

	bool LoadTexture(HINSTANCE hInst, HDC hdc, LPCWSTR tFileName);
	void UnLoadTexture();
	bool CreateBackBuffer(HINSTANCE hInst, HDC hDC, int tClinetWidth, int tClinetHeight);


//Getter, Setter
public:
	HDC GetDC() const { return mhDCMem; }
	HBITMAP GetBitmap() const { return mhBitmap; }
	const BITMAP& GetBitmapInfo() const { return mBitmapInfo; }
	bool IsValid() const { return mhBitmap && mhDCMem; }

};

