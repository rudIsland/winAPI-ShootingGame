#include "BulletTwoStraightShot.h"
#include "CEngine.h"
#include "CTexture.h"
#include "IBulletOwner.h"
#include "rudmacro.h"


BulletTwoStraightShot::BulletTwoStraightShot(CTexture* tBulletTexture, float tDamage, float tSpeed, float tFireInterval)
{
    bulletTexture = tBulletTexture;
    fireInterval = tFireInterval;
    mDamage = tDamage;
    mSpeed = tSpeed;
}

void BulletTwoStraightShot::BindBulletManager(BulletManager* tBindBulletManager)
{
    mBindBulletManager = tBindBulletManager;
}

BulletTwoStraightShot::~BulletTwoStraightShot()
{
    mBindBulletManager = nullptr; // 소유권 없음
}


void BulletTwoStraightShot::RenderBullet(CEngine* tEngine, const IBulletOwner& tOwner){
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
void BulletTwoStraightShot::Update_Bullets(CEngine* tEngine, const IBulletOwner& tOwner)
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
            bullet->Reset();        
            bulletPool.push(bullet);
        }
        else {
            activeBullets.push(bullet);
        }
    }
}

//총알 생성
void BulletTwoStraightShot::FireBullet(CEngine* tEngine, const IBulletOwner& tOwner)
{
    fireTimer += CEngine::mDeltaTime;
    if (fireTimer < fireInterval) return;
    fireTimer = 0.0f;

    //실제 큐에 참조된다.
    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();

    float offsetX = tOwner.GetUnitWidth() * 0.3333f;   // 좌우 거리
    float offsetY = tOwner.GetUnitHeight() * 0.5f + 5; // 위쪽 거리

    /*if (auto player = dynamic_cast<const CEnemyUnit*>(&tOwner)) {
        OutputDebugString(L"CEnemyUnit에서 호출됨\n");
    }*/

    for (int i = 0; i < 2; ++i) {
        if (bulletPool.empty()) return;

        SBullet* bullet = bulletPool.front();
        bulletPool.pop();

        Vector2D spawnPos = (i == 0)
            ? Vector2D(tOwner.GetPosition().mX - offsetX, tOwner.GetPosition().mY - offsetY) //Left Bullet
            : Vector2D(tOwner.GetPosition().mX + offsetX, tOwner.GetPosition().mY - offsetY); //Right Bullet


        bullet->direction = tOwner.GetForward().GetNormalize();
        bullet->SetPosition(spawnPos);
        bullet->mDamage = mDamage; //런타임중 데미지 변경
        bullet->mSpeed = mSpeed;  //런타임중 속도 변경
        bullet->velocity = bullet->direction * bullet->mSpeed; //발사 주체의 앞방향으로 발사
        bullet->mScale = 0.2f;
        bullet->isActive = true;
        activeBullets.push(bullet);
    }
}
