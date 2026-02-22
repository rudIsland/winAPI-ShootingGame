#include "CUnit.h"
#include "CEngine.h"
#include "CTexture.h"
#include "BulletManager.h"
#include "rudmacro.h"

CUnit::CUnit()
{
	
}

CUnit::~CUnit()
{
	/*if (!bulletShots.empty()) {
		for (auto behavior : bulletShots) {
			delete behavior;
			behavior = nullptr;
		}
	}*/
	SAFE_DELETE(mBulletManager);
}

CUnit::CUnit(const CUnit& t) //clone으로 복사대입연산자가 호출됨. (프리팹은 실행X)
{ //포인터 변수끼리 대입하므로 얉은 복사

	mPosition = t.mPosition;
	mVelocity = t.mVelocity;
	mForward = t.mForward;
	mDisplayPos = t.mDisplayPos;
	mAnchorPos = t.mAnchorPos;
	mScale = t.mScale;
	mSpeed = t.mSpeed;
	mHP = t.mHP;
	isActive = t.isActive;
	mTeam = t.mTeam;

	// 텍스처는 포인터 복사 (공유)
	mTexture = t.mTexture;
	mBulletTexture = t.mBulletTexture;

	// BulletManager는 새로 생성
	mBulletManager = new BulletManager();

	// Provider 복사
	provider = t.provider;
}

void CUnit::operator=(const CUnit& t)
{
}

void CUnit::Create(float x, float y, float tSpeed)
{
	mPosition.mX = x;
	mPosition.mY = y;
	mSpeed = tSpeed;
}

void CUnit::Destroy()
{

}

void CUnit::Update(CEngine* tEngine)
{
}


void CUnit::Render(CEngine* tEngine)
{
	// 1. Texture가 없으면 바로 return (방어코드)
	if (!mTexture || !tEngine) return;
	

	mDisplayPos = CalcDisplayPosition();


	// 3. 실제 그리기
	tEngine->DrawtextureScaled(mDisplayPos.mX, mDisplayPos.mY, mTexture, mScale);
}

void CUnit::Move()
{
	mPosition = mPosition + mVelocity * CEngine::mDeltaTime;
}

// Anchor와 Position을 고려한 실제 그릴 위치 계산 함수 분리
Vector2D CUnit::CalcDisplayPosition() const
{
	if (!mTexture) return mPosition;

	float tWidth = mTexture->GetBitmapInfo().bmWidth * mScale;
	float tHeight = mTexture->GetBitmapInfo().bmHeight * mScale;

	// 중심 좌표에서 Anchor만큼 빼서 좌상단 좌표 반환
	return Vector2D(
		mPosition.mX - tWidth * mAnchorPos.mX,
		mPosition.mY - tHeight * mAnchorPos.mY
	);
}

float CUnit::getRadius() {
	if (!mTexture) return 0.0f;

	return (mTexture->GetBitmapInfo().bmWidth * mScale) * 0.5f;
}
