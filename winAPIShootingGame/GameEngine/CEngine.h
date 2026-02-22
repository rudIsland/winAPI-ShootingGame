#pragma once

#include "framework.h"
#include "rudmacro.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CUnit.h"
#include "Vector2D.h"
//윈도우 api를 이용하는데 필요한 헤더를 포함했다.
//가장 중요한 것은 #include <windows.h>

//라이브러리 파일을 링크시키도록 컴파일 옵션을 지시
#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

class CTexture;

class CEngine
{
public:
	CEngine();
	virtual ~CEngine(); //가상 소멸자 적용

	//2) 새롭게 추가된 예약어를 이용한 방식
	CEngine(const CEngine& t) = delete; //복사 생성자
	CEngine& operator=(const CEngine& t) = delete; //복사 대입 연산자 금지

	BOOL Create(HINSTANCE hInstance, int nCmdShow, LPCWSTR tWindowClass, LPCWSTR tTitle);
	MSG Run();

	//다음은 게임 프로그램 구조의 이벤트 함수에 해당한다.
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate();

	// 픽셀Pixel: 색상 데이터
	// 여기서 매개변수는 색상정보의 각각의 구성 성분의 수치를 [0,1]로
	// '정규화'하여 다루겠다는 의도로 설계
	void Clear(float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	void Present();


	//화면 screen은 픽셀의 2차원 집합이므로 불연속적인 공간이다.
	//그러나 좌표나 반지름은 int로 최종 취급된다.
	//여기서는 연속적인 게임 세계 공간으로 취급하기 위해서 float실수로 다루겠다.
	void DrawCircle(float x, float y, float radius) const;
	void Drawtexture(float x, float y, CTexture* tTexture) const;
	

	void DrawtextureScaled(float tx, float ty, CTexture* tTexture, float scale) const;
	
	template<typename T>
	T* CreatePrefab(CTexture* tTexture, CTexture* tBulletTexture, Vector2D tAnchor, Vector2D tDisplayPos, float tSpeed, float tHP, float tMaxHP, float tScale, Team tTeam) {
		T* tPrefab = new T(); 
		
		//프리팹 생성할땐 공유자원만 할당해줄 것.
		tPrefab->SetTexture(tTexture);
		tPrefab->SetBulletTexture(tBulletTexture);
		
		tPrefab->SetDisplayPos(tDisplayPos);
		tPrefab->SetAnchorPoint(tAnchor.mX, tAnchor.mY);
		tPrefab->SetSpeed(tSpeed);
		tPrefab->SetHP(tHP);
		tPrefab->SetMaxHP(tMaxHP);
		tPrefab->SetScale(tScale);
		tPrefab->SetWidth(tTexture->GetBitmapInfo().bmWidth * tScale);
		tPrefab->SetHeight(tTexture->GetBitmapInfo().bmHeight * tScale);
		tPrefab->SetTeam(tTeam);

		return tPrefab;
	}

	void DestoryPrefab(CUnit*& tPrefab) {
		SAFE_DELETE(tPrefab);
	}

	template<typename T>
	T* InstanceUnit(CUnit* tPrefab) {
		T* tUnit = nullptr;
		//여기서는 clone으로 복사생성자를 호출하고 각자의 것은 따로 실행
		tUnit = static_cast<T*>(tPrefab->clone());
		tUnit->InItBulletShots(); //복사대입 연산자에 넣으니까 자기것만 호출해서 넣질않음 이것도 확실한 방법임
		tUnit->AddRef();

		return tUnit;
	}

	template<typename T>
	void DestroyUnit(T*& tUnit) {
		SAFE_RELEASE(tUnit);
	}


	
public:
	//static float deltaTime;
	//클라이언트 창 크기
	static const int ClientWidth = 1024;
	static const int ClientHeight = 900;

public:
	// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

	//WndProc, About 는 현재 멤버함수이다.
	//그런데 window api에서 포인터가 원하는 형식은 전역함수의 참조이다.
	//그러므로 그에 맞추기 위해
	//static(정적)함수로 선언하였다.


protected:
	static HINSTANCE hInst;              //전역공간에 초기화해야 한다.
	WCHAR szTitle[MAX_LOADSTRING];
	WCHAR szWindowClass[MAX_LOADSTRING];

	//WCHAR szTitle[MAX_LOADSTRING] = {0};                  // 제목 표시줄 텍스트입니다.
	//WCHAR szWindowClass[MAX_LOADSTRING] = {0};            // 기본 창 클래스 이름입니다.

protected:
	//dc handle
	HDC mhdc = NULL;
	//window handle
	HWND mhWnd = NULL;

	//더블 버퍼 시스템의 back buffer객체의 참조
	CTexture* mBackBuffer = nullptr;

private:
	//틱 타이머를 위한 변수
	LARGE_INTEGER mSecond; //초당 틱 개수
	LARGE_INTEGER mTime; //일정 시점의 틱번호
public:
	 static float mDeltaTime;
};

