#include "BulletOneStraightShot.h"
#include "CEngine.h"
#include "CTexture.h"
#include "IBulletOwner.h"
#include "rudmacro.h"

BulletOneStraightShot::BulletOneStraightShot(CTexture* tBulletTexture, float tDamage, float tSpeed, float tFireInterval)
{
    bulletTexture = tBulletTexture;
    fireInterval = tFireInterval;
    mDamage = tDamage;
    mSpeed = tSpeed;

}

void BulletOneStraightShot::BindBulletManager(BulletManager* tBindBulletManager)
{
    mBindBulletManager = tBindBulletManager;
}

BulletOneStraightShot::~BulletOneStraightShot()
{
    //mBindBulletManager = nullptr; // 소유권 없음
}


void BulletOneStraightShot::RenderBullet(CEngine* tEngine, const IBulletOwner& tOwner) {
    std::queue<SBullet*> drawQueue = mBindBulletManager->GetActiveBullets();
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
void BulletOneStraightShot::Update_Bullets(CEngine* tEngine, const IBulletOwner& tOwner)
{

    //실제 큐에 참조된다.
    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();

    if (bulletPool.empty()) return; // 방어코드

    int count = (int)mBindBulletManager->GetActiveBullets().size();
    for (int i = 0; i < count; ++i) {
        SBullet* bullet = activeBullets.front(); activeBullets.pop();

        bullet->SetPosition(bullet->position + bullet->velocity * CEngine::mDeltaTime);

        //클라이언트 벽 충돌감지 or 적과 충돌로 인한 isActive비활성화 감지
        bool removeBullet =
            !bullet->isActive ||
            bullet->position.mY < 0 || bullet->position.mY > tEngine->ClientHeight ||
            bullet->position.mX < 0 || bullet->position.mX > tEngine->ClientWidth;

        if (removeBullet) {
            bullet->Reset();             // ← 기존 ReturnBulletToPool 동등 동작
            bulletPool.push(bullet);
        }
        else {
            activeBullets.push(bullet);
        }
    }
}

//총알 생성
void BulletOneStraightShot::FireBullet(CEngine* tEngine, const IBulletOwner& tOwner)
{
    fireTimer += CEngine::mDeltaTime;
    if (fireTimer < fireInterval) return;
    fireTimer = 0.0f;

    //실제 큐에 참조된다.
    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();

    if (bulletPool.empty()) return; // 방어코드

    SBullet* bullet = bulletPool.front();
    bulletPool.pop();

    // 중심 좌표
    Vector2D ownerPos = tOwner.GetPosition();

    // Anchor 고려한 절반 높이
    float halfHeight = tOwner.GetUnitHeight() * 0.5f;

    // 총알 생성 위치 (중앙 위쪽 약간)
    Vector2D spawnPos(
        ownerPos.mX,                            
        ownerPos.mY - halfHeight * -0.3f //중심좌표 - 높이*-3(아래로)
    );


    bullet->direction = tOwner.GetForward().GetNormalize();
    bullet->SetPosition(spawnPos);
    bullet->mDamage = mDamage; //런타임중 데미지 변경
    bullet->mSpeed = mSpeed;  //런타임중 속도 변경
    bullet->velocity = bullet->direction * bullet->mSpeed; //발사 주체의 앞방향으로 발사
    bullet->mScale = 0.3f;
    bullet->isActive = true;
    activeBullets.push(bullet);
}
