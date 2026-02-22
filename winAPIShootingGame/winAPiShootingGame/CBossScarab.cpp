#include "CBossScarab.h"
#include "CEngine.h"
#include "CUnit.h"
#include "ITargetProvider.h"
#include "rudmacro.h"

//총알
#include "BulletOneStraightShot.h"
#include "BulletTwoStraightShot.h"
#include "BulletRoundShot.h"
#include "BulletPlayerAimShot.h"
#include "BulletSpreadShot.h"

CBossScarab::CBossScarab(){

}
CBossScarab::~CBossScarab(){

}
CBossScarab::CBossScarab(const CBossScarab& t) : CEnemyUnit(t) {

    // 멤버 변수들 복사(공통된 변수면 CUnit에 선언으로 옮겨서 복사생성자에 넣어도됨.)
    mMoveSpeed = t.mMoveSpeed;
    mMoveTime = t.mMoveTime;
    mTpInterval = t.mTpInterval;
    mCurrentPhase = t.mCurrentPhase;
    mMoveType = t.mMoveType;
}

void CBossScarab::InItBulletShots() {
    wchar_t buf[128];
    swprintf(buf, 128, L"[BossScarab %p] InItBulletShots called\n", this);
    OutputDebugString(buf);
    // 총알 등록
    mBulletManager->AddBulletShots("OneStraightShot", new BulletOneStraightShot(mBulletTexture, 100.0f, 1000.0f, 2.0f));
    mBulletManager->AddBulletShots("TwoStraightShot", new BulletTwoStraightShot(mBulletTexture, 60.0f, 200.0f, 2.3f));
    mBulletManager->AddBulletShots("RoundShot", new BulletRoundShot(mBulletTexture, 100.0f, 200.0f, 2.0f, 8, 10.0f));
    mBulletManager->AddBulletShots("AimShot", new BulletPlayerAimShot(mBulletTexture, 500.0f, 1000.0f, 2.0f));

    //mBulletManager->AddActiveBulletShots("OneStraightShot");
}

void CBossScarab::Update(CEngine* tEngine)
{

    // 현재 HP에 맞는 페이즈 계산
    int phase = GetPhaseByHP();

    // 페이즈가 바뀌면 바꾼다.
    if (phase != mCurrentPhase) {
        mCurrentPhase = phase;
        ApplyPhaseSettings(phase);
    }

    // 이동 실행
    if (mMoveType == 1)
        MoveLR(tEngine);
    else if (mMoveType == 2)
        RandomMoveLR(tEngine);

    //총알행동 업데이트
    mBulletManager->Update(tEngine, *this);
}

void CBossScarab::MoveLR(CEngine* tEngine)
{
    mMoveTime += CEngine::mDeltaTime * mMoveSpeed * 0.1f;
    if (mMoveTime > 360.0f)  // 한 바퀴 돌면 0으로 초기화
        mMoveTime -= 360.0f;

    // 이동 범위 (픽셀 단위)
    float amplitude = 200.0f; // 좌우 최대 이동량

    // 사인파로 좌우 이동
    float moveDistance = amplitude * sinf(mMoveTime * (M_PI / 180.0f) * 45.0f);
    Vector2D basePos = Vector2D(tEngine->ClientWidth * 0.5f, 100.0f);
    SetPosition(Vector2D(basePos.mX + moveDistance, basePos.mY));
}

void CBossScarab::RandomMoveLR(CEngine* tEngine)
{
    //2초마다 순간이동
    mMoveTime += CEngine::mDeltaTime;
    if (mMoveTime < 2.0f) return;
    mMoveTime = 0.0f;

    float randPosX = (rand() % 401) - 200.0f;

    // 현재 위치 기준으로 이동
    Vector2D basePos = Vector2D(tEngine->ClientWidth * 0.5f, 100.0f); // 프리팹 스폰 위치 기준
    SetPosition(Vector2D(basePos.mX + randPosX, basePos.mY));
}


void CBossScarab::Render(CEngine* tEngine)
{
    // 방어코드 강화
    if (!mTexture) {
        // 로그나 디버그 출력 추가
        OutputDebugStringA("CBossScarab::Render - mTexture is nullptr!\n");
        return;
    }
    if (!tEngine) {
        OutputDebugStringA("CBossScarab::Render - tEngine is nullptr!\n");
        return;
    }

    // display 위치 계산
    mDisplayPos = CalcDisplayPosition();

    // 3. 실제 그리기
    tEngine->DrawtextureScaled(mDisplayPos.mX,
        mDisplayPos.mY,
        mTexture,
        mScale);

    mBulletManager->Render(tEngine, *this);
}

void CBossScarab::ApplyPhaseSettings(int phase)
{
    // 먼저 현재 활성화된 모든 총알행동 제거
    mBulletManager->ClearActiveBulletShots();

    switch (phase) {
    case 1:
        mMoveType = 1; // 좌우 이동
        mMoveSpeed = 35.0f;
        mBulletManager->AddActiveBulletShots("OneStraightShot");
        break;
    case 2:
        {
        mMoveType = 1;
        mMoveSpeed = 36.5f;
        // OneStraightShot 파라미터 수정된 버전 등록
        mBulletManager->AddBulletShots("OneStraightShot",
            new BulletOneStraightShot(mBulletTexture, 100.0f, 1000.0f, 1.0f));
        mBulletManager->AddActiveBulletShots("OneStraightShot");
        break;
        }
    case 3:
        mMoveType = 1; // 좌우 이동
        mMoveSpeed = 38.0f;
        mBulletManager->AddActiveBulletShots("TwoStraightShot");
        break;
    case 4:
        {
        mMoveType = 1;
        mMoveSpeed = 39.5f;
        // TwoStraightShot 파라미터 수정된 버전 등록
        mBulletManager->AddBulletShots("TwoStraightShot",
            new BulletTwoStraightShot(mBulletTexture, 60.0f, 280.0f, 1.5f));
        mBulletManager->AddActiveBulletShots("TwoStraightShot");
        break;
        }
    case 5:
        mMoveType = 1; // 좌우 이동
        mMoveSpeed = 41.0f;
        mBulletManager->AddActiveBulletShots("RoundShot");
        break;
    case 6:
        {
        mMoveType = 1;
        mMoveSpeed = 43.0f;
        // RoundShot 파라미터 수정된 버전 등록
        mBulletManager->AddBulletShots("RoundShot",
            new BulletRoundShot(mBulletTexture, 100.0f, 250.0f, 2.0f, 14, 10.0f));
        mBulletManager->AddActiveBulletShots("RoundShot");
        break;
        }
    case 7:
        mMoveType = 1;
        mMoveSpeed = 43.0f;
        mBulletManager->AddActiveBulletShots("AimShot");
        break;
    case 8:
        {
        mMoveType = 2; // 랜덤 이동
        mTpInterval = 1.5f;
        // AimShot 파라미터 수정된 버전 등록
        mBulletManager->AddBulletShots("AimShot",
            new BulletPlayerAimShot(mBulletTexture, 100.0f, 1200.0f, 1.5f));
        mBulletManager->AddActiveBulletShots("AimShot");
        break;
        }
    case 9:
        {
        mMoveType = 2; // 랜덤 이동
        mTpInterval = 1.2f;
        // AimShot 파라미터 수정된 버전 등록
        mBulletManager->AddBulletShots("AimShot",
            new BulletPlayerAimShot(mBulletTexture, 100.0f, 1200.0f, 1.2f));
        mBulletManager->AddActiveBulletShots("AimShot");
        break;
        }
    case 10:
        {
        mMoveType = 2; // 랜덤 이동
        mTpInterval = 0.5f;
        // AimShot 파라미터 수정된 버전 등록
        mBulletManager->AddBulletShots("AimShot",
            new BulletPlayerAimShot(mBulletTexture, 100.0f, 1200.0f, 0.5f));
        mBulletManager->AddActiveBulletShots("AimShot");
        break;
        }
    }
}

int CBossScarab::GetPhaseByHP()
{
    if (mHP >= 3600.0f) return 1;
    if (mHP >= 3200.0f) return 2;
    if (mHP >= 2800.0f) return 3;
    if (mHP >= 2400.0f) return 4;
    if (mHP >= 2000.0f) return 5;
    if (mHP >= 1600.0f) return 6;
    if (mHP >= 1200.0f) return 7;
    if (mHP >= 800.0f) return 8;
    if (mHP >= 400.0f) return 9;
    if (mHP >= 0.0f) return 10;
    return 4;
}