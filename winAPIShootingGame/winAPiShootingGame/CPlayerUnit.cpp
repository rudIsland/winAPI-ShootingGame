#include "CPlayerUnit.h"
#include "CInputMgr.h"
#include "CEngine.h"
#include "CTexture.h"
#include "CEnemyUnit.h"
#include "rudmacro.h"
#include <string>

#include "BulletManager.h"
#include "BulletTwoStraightShot.h"

CPlayerUnit::CPlayerUnit() { }

CPlayerUnit::~CPlayerUnit(){ 
   
}

CPlayerUnit::CPlayerUnit(const CPlayerUnit& t) : CUnit(t){
}

void CPlayerUnit::InItBulletShots() {
    //총알 행동 등록
    mBulletManager->AddBulletShots("TwoStraightShot", new BulletTwoStraightShot(mBulletTexture, 5.0f, 1000.0f, 0.25f));

    mBulletManager->AddActiveBulletShots("TwoStraightShot");

}

void CPlayerUnit::Update(CEngine* tEngine)
{
    Update_PlayerMove(tEngine);

    mBulletManager->Update(tEngine, *this);
}


void CPlayerUnit::Update_PlayerMove(CEngine* tEngine)
{
    //플레이어 유닛을 움직이게 한다.
    mVelocity = Vector2D(0, 0);

    // 입력만 받고 Move()는 호출하지 않음
    if (CInputMgr::GetInst()->KeyPress("DoMoveLt")) { mVelocity.mX += -1.0f; }
    if (CInputMgr::GetInst()->KeyPress("DoMoveRt")) { mVelocity.mX += +1.0f; }
    if (CInputMgr::GetInst()->KeyPress("DoMoveFoward")) { mVelocity.mY += -1.0f; }
    if (CInputMgr::GetInst()->KeyPress("DoMoveBack")) { mVelocity.mY += +1.0f; }

    // 입력이 있을 때만 한 번만 Move() 호출
    if (mVelocity.mX != 0 || mVelocity.mY != 0) {
        SetVelocity(Vector2D(mVelocity * mSpeed));
        Move();
    }

    LimitPositionToWindow(tEngine);  // 윈도우 창 크기만큼 이동 제한
}





void CPlayerUnit::LimitPositionToWindow(CEngine* tEngine)
{
    if (!mTexture || !tEngine) return;  // 방어코드 추가

    float tRadius = mTexture->GetBitmapInfo().bmWidth * mScale * 0.5f;

    // X축 경계 검사
    if (mPosition.mX - tRadius < 0)
        mPosition.mX = tRadius;
    if (mPosition.mX + tRadius > tEngine->ClientWidth)
        mPosition.mX = tEngine->ClientWidth - tRadius;

    // Y축 경계 검사
    if (mPosition.mY - tRadius < 0)
        mPosition.mY = tRadius;
    if (mPosition.mY + tRadius > tEngine->ClientHeight)
        mPosition.mY = tEngine->ClientHeight - tRadius;
}


void CPlayerUnit::Render(CEngine* tEngine)
{
    // 방어코드
    if (!mTexture || !tEngine) return;

    // display 위치 계산
    mDisplayPos = CalcDisplayPosition();

    // 3. 실제 그리기
    tEngine->DrawtextureScaled(mDisplayPos.mX,
        mDisplayPos.mY,
        mTexture, 
        mScale);

    mBulletManager->Render(tEngine, *this);
}

