#include "BulletSpreadShot.h"
#include "CEngine.h"
#include "CTexture.h"
#include "IBulletOwner.h"
#include "rudmacro.h"

BulletSpreadShot::BulletSpreadShot(CTexture* tBulletTexture, float tDamage, float tSpeed, float tFireInterval, float tAngle, int tBulletCount)
{
    bulletTexture = tBulletTexture;
    fireInterval = tFireInterval;
    mDamage = tDamage;
    mSpeed = tSpeed;
    mAngle = tAngle;
    mBulletCount = tBulletCount;


    // 오브젝트 풀 초기화
   /* for (int i = 0; i < MAX_BULLETS; i++) {
        SBullet* bullet = new SBullet();
        bullet->mDamage = tDamage;
        bullet->mSpeed = tSpeed;
        bulletPool.push(bullet);
    }*/
}

void BulletSpreadShot::BindBulletManager(BulletManager* tBindBulletManager) {
    mBindBulletManager = tBindBulletManager;
}

BulletSpreadShot::~BulletSpreadShot()
{
    mBindBulletManager = nullptr; // 소유권 없음
   /* while (!activeBullets.empty()) { delete activeBullets.front(); activeBullets.pop(); }
    while (!bulletPool.empty()) { delete bulletPool.front();     bulletPool.pop(); }*/
}


void BulletSpreadShot::RenderBullet(CEngine* tEngine, const IBulletOwner& tOwner) {
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
void BulletSpreadShot::Update_Bullets(CEngine* tEngine, const IBulletOwner& tOwner)
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
            mBindBulletManager->ReturnBulletToPool(bullet);
        }
        else {
            activeBullets.push(bullet);
        }
    }
}

//총알 생성
void BulletSpreadShot::FireBullet(CEngine* tEngine, const IBulletOwner& tOwner)
{
    fireTimer += CEngine::mDeltaTime;
    if (fireTimer < fireInterval) return;
    fireTimer = 0.0f;
    
    //매니저에서 총알들의 원본을 참조
    std::queue<SBullet*>& activeBullets = mBindBulletManager->GetActiveBulletsRef();
    std::queue<SBullet*>& bulletPool = mBindBulletManager->GetBulletPoolRef();

    //1) 바라보는 방향의 각도구하기
    Vector2D OwnerForwardAngle = tOwner.GetForward();

    //공교롭게도 회전은 아래로 하기때문에 화면좌표계의 y값을 뒤집어줄 필요가 없다.
    float forwardRadian = atan2(OwnerForwardAngle.mY, OwnerForwardAngle.mX); // atan2(y,x)
    float forwardDeg = forwardRadian * 180.0f / 3.14159265f;

    //// wchar_t 버퍼에 담아서 출력
    //wchar_t buf[128];
    //swprintf_s(buf, L"Degree = %.2f\n", forwardDeg);
    //OutputDebugString(buf);

    const float start = -mAngle * 0.5f + forwardDeg; //발사 시작 각도 ex) + 40도 -> -20도~20도
    const float angleStep = (mBulletCount > 1) ? (mAngle / (mBulletCount - 1)) : 0.0f; //N이 1이면 0도로 설정 아닐경우 각도대로

    float tDegree = 360 / mBulletCount; //간격
    for (int i = 0; i < mBulletCount; ++i) {
        if (bulletPool.empty()) return;

        SBullet* bullet = bulletPool.front();
        bulletPool.pop();

        Vector2D tVelocity;
        
        // i번째 탄의 각도 (degree)
        float tAngleDegree = (angleStep * i) + start;
        tVelocity.mX = cosf(tAngleDegree * (3.14159f / 180.0f));
        tVelocity.mY = sinf(tAngleDegree * (3.14159f / 180.0f));


        bullet->SetPosition(tOwner.GetPosition());
        bullet->direction = tOwner.GetForward().GetNormalize();;
        bullet->mDamage = mDamage; //런타임중 데미지 변경
        bullet->mSpeed = mSpeed;  //런타임중 속도 변경
        bullet->velocity = bullet->direction * bullet->mSpeed;   // ← 이후로는 이 velocity만 사용
        bullet->mScale = 0.3f;
        bullet->isActive = true;
        activeBullets.push(bullet);

    }
}
