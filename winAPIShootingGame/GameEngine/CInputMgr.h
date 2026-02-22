#pragma once
#include <Windows.h>
#include <string>
#include <unordered_map> //해쉬자료구조 컨테이너

#include "rudmacro.h"

using namespace std;


struct SKeyInfo {
	string mStrName = "";	//추상화된 이름
	DWORD mKeyInput = 0;	//실제 키 입력값 <--char, int를 모두 처리하기 위한 의미로 DWORD(4byte 정수)로 타입을 결정

	//해당 '키 입력 정보'에 대한 (눌림 상태)
	bool mIsDown = false;
	bool mIsPress = false;
	bool mIsUp = false;
};



class CInputMgr
{

	SINGLETON_DECLARE(CInputMgr);

private:

	unordered_map<string, SKeyInfo*> mMapKeyInfos;

public:
	void Update();

	//임의의 키입력 정보 검색함수
	inline SKeyInfo* FindKeyInfo(const string& tStrKey) {
		auto t = mMapKeyInfos.find(tStrKey);
		if (mMapKeyInfos.end() == t) { 
			return nullptr;
		}
		return t->second; 
	}

	bool KeyDown(const string& tStrKey);
	bool KeyPress(const string& tStrKey);
	bool KeyUp(const string& tStrKey);

	//키 입력을 추가하기 위한 템플릿 함수
	template<typename T>
	bool AddKeyInfo(const string& tStrKey, const T& tData)
	{
		SKeyInfo* tKeyInfo = nullptr;

		tKeyInfo = new SKeyInfo();

		//실행중에 타입에 대한 정보를 조회할 수 있는 기능
		const char* tTypeName = typeid(T).name();

		if (strcmp(tTypeName, "char") == 0 || strcmp(tTypeName, "int") == 0) {
			tKeyInfo->mKeyInput = (DWORD)tData;
		}
		else {
			return false;
		}

		tKeyInfo->mStrName = tStrKey; //검색용 문자열 키

		this->mMapKeyInfos.insert({ tKeyInfo->mStrName, tKeyInfo });

		tKeyInfo = nullptr;

		return true;
	}
};

