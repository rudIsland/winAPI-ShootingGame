#pragma once

//전처리 구문은 한줄로만 처리되어야 한다.
//#define A B
//텍스트 형식의 소스코드 상에서 A를 만나면 B로 텍스트 치환한다.
//디버깅해도 여기에 들어오지 않는다.
#define SAFE_DELETE(p) if(nullptr != p) {delete p; p = nullptr;}

// \0 널종료 문자
// \t 탭 문자

// \ 엔터개행문자
// <-- \과 엔터 사이에 임의의 문자가 있으면 안된다.

//개행 문자

#define SAFE_RELEASE(p) \
if (nullptr != p)\
	{ p->Release();\
	p = nullptr;}


#define SINGLETON_DECLARE(p)\
private:\
static p* mInst;\
private:\
	p();\
	~p();\
public:\
	static p* GetInst();\
	static void ReleaseInst();

#define SINGLETON_DEFINITION(p)\
p* p::mInst = nullptr;\
p* p::GetInst()\
{\
	if (nullptr == mInst) {\
		mInst = new p();\
	}\
	return mInst;\
}\
void p::ReleaseInst()\
{\
	if (nullptr != mInst)\
	{\
		delete mInst;\
		mInst = nullptr;\
	}\
}