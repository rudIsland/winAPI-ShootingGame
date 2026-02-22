#include "BulletPlayerAimShot.h"
#include "CEngine.h"
#include "CTexture.h"
#include "IBulletOwner.h"
#include "rudmacro.h"

BulletPlayerAimShot::BulletPlayerAimShot(CTexture* tBulletTexture, float tDamage, float tSpeed, float tFireInterval)
{
    bulletTexture = tBulletTexture;
    fireInterval = tFireInterval;
    mDamage = tDamage;
    mSpeed = tSpeed;

    // 오브젝트 풀 초기화
    /*for (int i = 0; i < MAX_BULLETS; i++) {
        SBullet* bullet = new SBullet();
        bullet->mDamage = mDamage;
        bullet->mSpeed = mSpeed;
        bulletPool.push(bullet);
    }*/
}

void BulletPlayerAimShot::BindBulletManager(BulletManager* tBindBulletManager) {
    mBindBulletManager = tBindBulletManager;
}

BulletPlayerAimShot::~BulletPlayerAimShot()
{
    mBindBulletManager = nullptr; // 소유권 없음
   /* while (!activeBullets.empty()) { delete activeBullets.front(); activeBullets.pop(); }
    while (!bulletPool.empty()) { delete bulletPool.front();     bulletPool.pop(); }*/
}


void BulletPlayerAimShot::RenderBullet(CEngine* tEngine, const IBulletOwner& tOwner) {
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
void BulletPlayerAimShot::Update_Bullets(CEngine* tEngine, const IBulletOwner& tOwner)
{
    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();

    int count = (int)activeBullets.size();
    for (int i = 0; i < count; ++i) {
        SBullet* bullet = activeBullets.front(); activeBullets.pop();

        bullet->SetPosition(bullet->position + bullet->velocity * CEngine::mDeltaTime);

        bool removeBullet =
            !bullet->isActive ||
            bullet->position.mY < 0 || bullet->position.mY > tEngine->ClientHeight ||
            bullet->position.mX < 0 || bullet->position.mX > tEngine->ClientWidth;

        if (removeBullet) {
            mBindBulletManager->ReturnBulletToPool(bullet);
        }
        else {
            activeBullets.push(bullet);
        }
    }
}

//총알 생성
void BulletPlayerAimShot::FireBullet(CEngine* tEngine, const IBulletOwner& tOwner)
{
    fireTimer += CEngine::mDeltaTime;
    if (fireTimer < fireInterval) return;
    fireTimer = 0.0f;

    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();


    if (bulletPool.empty()) return;

    SBullet* bullet = bulletPool.front();
    bulletPool.pop();

    // 총알 생성 위치 (중앙 아래쪽 약간)
    Vector2D spawnPos( tOwner.GetPosition().mX, tOwner.GetPosition().mY * 1.1f);

    Vector2D velocity;
    Vector2D targetPos = tOwner.GetTargetProvider()->GetTargetIsPlayerGetPosition();

    //현재유닛 위치 -> 타겟 위치 Vector를 정규화하여 방향을 구하고 속도를 곱한다.
    velocity = targetPos - tOwner.GetPosition();
    velocity = velocity.GetNormalize();

    bullet->SetPosition(spawnPos);
    bullet->direction = velocity;
    bullet->mDamage = mDamage; //런타임중 데미지 변경
    bullet->mSpeed = mSpeed;  //런타임중 속도 변경
    bullet->velocity = bullet->direction * bullet->mSpeed;   // ← 이후로는 이 velocity만 사용
    bullet->mScale = 0.3f;
    bullet->isActive = true;
    activeBullets.push(bullet);
}
