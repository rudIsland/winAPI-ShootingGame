#include "BulletRoundShot.h"
#include "CEngine.h"
#include "CTexture.h"
#include "IBulletOwner.h"
#include "rudmacro.h"

BulletRoundShot::BulletRoundShot(CTexture* tBulletTexture, float tDamage, float tSpeed, float tFireInterval, int tBulletCount, float tRotateAngle)
{
    bulletTexture = tBulletTexture; //총알 텍스처
    fireInterval = tFireInterval; //발사 간격
    mBulletCount = tBulletCount; //한번에 발사할 총알갯수
    mDamage = tDamage;
    mSpeed = tSpeed;
    mAngle = tRotateAngle;  //발사마다 회전할 각도
    curRotationAngle = 0.0f;

    //// 오브젝트 풀 초기화
    //for (int i = 0; i < MAX_BULLETS; i++) {
    //    SBullet* bullet = new SBullet();
    //    bullet->mDamage = mDamage;
    //    bullet->mSpeed = mSpeed;
    //    bulletPool.push(bullet);
    //}
}

void BulletRoundShot::BindBulletManager(BulletManager* tBindBulletManager)
{
    mBindBulletManager = tBindBulletManager;
}

BulletRoundShot::~BulletRoundShot()
{
    mBindBulletManager = nullptr; // 소유권 없음
  /*  while (!activeBullets.empty()) { delete activeBullets.front(); activeBullets.pop(); }
    while (!bulletPool.empty()) { delete bulletPool.front();     bulletPool.pop(); }*/
}


void BulletRoundShot::RenderBullet(CEngine* tEngine, const IBulletOwner& tOwner) {
    std::queue<SBullet*> drawQueue = mBindBulletManager->GetActiveBullets(); //복사만.
    while (!drawQueue.empty()) {
        SBullet* bullet = drawQueue.front();
        drawQueue.pop();

        // 총알도 중앙 기준으로 그리기 위해 앵커 적용
        float bulletWidth = bulletTexture->GetBitmapInfo().bmWidth * bullet->mScale;
        float bulletHeight = bulletTexture->GetBitmapInfo().bmHeight * bullet->mScale;

        // 중앙 기준으로 렌더링 위치 계산
        float renderX = bullet->position.mX - bulletWidth * 0.5f;   // 중앙에서 너비의 절반만큼 빼기
        float renderY = bullet->position.mY - bulletHeight * 0.5f;  // 중앙에서 높이의 절반만큼 빼기

        tEngine->DrawtextureScaled(renderX, renderY, bulletTexture, bullet->mScale);
    }
}

//총알 업데이트
void BulletRoundShot::Update_Bullets(CEngine* tEngine, const IBulletOwner& tOwner)
{
    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();

    int count = (int)activeBullets.size();
    for (int i = 0; i < count; ++i) {
        SBullet* bullet = activeBullets.front(); activeBullets.pop();

        bullet->SetPosition(bullet->position + bullet->velocity * CEngine::mDeltaTime); //중앙에서 생성

        bool removeBullet =
            !bullet->isActive ||
            bullet->position.mY < 0 || bullet->position.mY > tEngine->ClientHeight ||
            bullet->position.mX < 0 || bullet->position.mX > tEngine->ClientWidth;

        if (removeBullet) {
            //bullet->Reset();
            //bulletPool.push(bullet);
            mBindBulletManager->ReturnBulletToPool(bullet);
        }
        else {
            activeBullets.push(bullet);
        }
    }
}

//총알 생성
void BulletRoundShot::FireBullet(CEngine* tEngine, const IBulletOwner& tOwner)
{
    fireTimer += CEngine::mDeltaTime;
    if (fireTimer < fireInterval) return;
    fireTimer = 0.0f;

    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();

    curRotationAngle += mAngle; //회전 각도를 누적시킨다.
    if (curRotationAngle >= 90.0f) { //90도 이상 회전되면 의미 없으므로 초기화
        curRotationAngle = 0.0f;
    }

    float tAngleStep = 360.0f / mBulletCount; //회전할 각도

    for (int i = 0; i < mBulletCount; ++i) {
        if (bulletPool.empty()) return;

        SBullet* bullet = bulletPool.front();
        bulletPool.pop();

        Vector2D tVelocity;
        // i번째 탄의 각도 (degree)
        float tAngleDegree = (tAngleStep * i) + curRotationAngle;

        // degree -> radian
        float rad = tAngleDegree * (3.14159f / 180.0f);
   
        tVelocity.mX = cosf(tAngleDegree * (3.14159f / 180.0f));
        tVelocity.mY = sinf(tAngleDegree * (3.14159f / 180.0f));



        bullet->SetPosition(tOwner.GetPosition());
        bullet->direction = tVelocity;
        bullet->mDamage = mDamage; //런타임중 데미지 변경
        bullet->mSpeed = mSpeed;  //런타임중 속도 변경
        bullet->velocity = bullet->direction * bullet->mSpeed; //여기서 속도를 곱해줘야함.
        bullet->mScale = 0.3f;
        bullet->isActive = true;
        activeBullets.push(bullet);
    }
}
