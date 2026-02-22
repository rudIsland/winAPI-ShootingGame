#include "CInputMgr.h"
#include "rudmacro.h"
#include <Windows.h>
#include "BulletManager.h"


SINGLETON_DEFINITION(CInputMgr)


CInputMgr::CInputMgr()
{
	//OutputDebugString(L"CInputMgr::CInputMgr()\n");

	mMapKeyInfos.clear();
}

CInputMgr::~CInputMgr()
{
	//OutputDebugString(L"CInputMgr::~CInputMgr()\n");

	for (auto t = mMapKeyInfos.begin(); t != mMapKeyInfos.end(); t++)
	{
		SAFE_DELETE(t->second);
	}

	mMapKeyInfos.clear();
}

void CInputMgr::Update()
{
	for (auto t = mMapKeyInfos.begin(); t != mMapKeyInfos.end(); t++) {
		int tPushCount = 0;

		//눌림을 감지
		if (GetAsyncKeyState(t->second->mKeyInput) & 0x8000) {
			++tPushCount;
		}

		//처음 눌렸을때
		if (tPushCount == 1) {
			if (!t->second->mIsDown && !t->second->mIsPress) {
				t->second->mIsDown = true;
			}
			//눌리고 있는 중 시작
			else if (t->second->mIsDown && !t->second->mIsPress) {
				t->second->mIsDown = false;
				t->second->mIsPress = true;
			}
		} 
		else { //눌림 없음
			if (t->second->mIsDown || t->second->mIsPress) {
				t->second->mIsDown = false;
				t->second->mIsPress = false;
				t->second->mIsUp = true;
			}
			else if(t->second->mIsUp){
				t->second->mIsUp = false;
			}
		}

	} //for end
}

bool CInputMgr::KeyDown(const string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (nullptr == tInfo) {
		return false;
	}

	return tInfo->mIsDown;
}

bool CInputMgr::KeyPress(const string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey); //O(1)

	if (nullptr == tInfo) {
		return false;
	}

	return tInfo->mIsPress;
}

bool CInputMgr::KeyUp(const string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey); //O(1)

	if (nullptr == tInfo) {
		return false;
	}

	return tInfo->mIsUp;
}


