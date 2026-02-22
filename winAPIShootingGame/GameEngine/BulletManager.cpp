#include "BulletManager.h"
#include "SBullet.h"
#include "CEngine.h"
#include "rudmacro.h"
#include "IBulletOwner.h"
#include <unordered_map>
#include <unordered_set>

BulletManager::BulletManager() {
    OutputDebugString(L"BulletManager Created\n");
    // 오브젝트 풀 초기화
    for (int i = 0; i < MAX_BULLETS; i++) {
        SBullet* bullet = new SBullet();
        bulletPool.push(bullet);
    }
}

BulletManager::~BulletManager() { //총알 큐 삭제
    OutputDebugString(L"BulletManager Destroyed\n");
    while (!activeBullets.empty()) { delete activeBullets.front(); activeBullets.pop(); }
    while (!bulletPool.empty()) { delete bulletPool.front();     bulletPool.pop(); }

   
    // 총알 행동 해제 (원본만 지움)
    for (auto& kv : bulletShots) {
        wchar_t buf[128];
        swprintf(buf, 128, L"Deleting bulletShot [%S] : %p\n", kv.first.c_str(), kv.second);
        OutputDebugString(buf);
        SAFE_DELETE(kv.second); // Manager가 소유자이므로 여기서 해제
    }
    bulletShots.clear();

    // activeBulletShots는 공유 포인터라 delete 하면 안됨
    activeBulletShots.clear();

}

void BulletManager::AddBulletShots(std::string tAddName, IBulletShot* tAddBehavior)
{
    tAddBehavior->BindBulletManager(this);
    wchar_t buf[128];
    swprintf(buf, 128, L"Register bulletShot [%S] : %p\n", tAddName.c_str(), tAddBehavior);
    OutputDebugString(buf);

    bulletShots.insert({ tAddName, tAddBehavior });
}

void BulletManager::AddActiveBulletShots(std::string ShotName) {
    auto it = bulletShots.find(ShotName);
    if (it != bulletShots.end()) {
        activeBulletShots.insert(*it); // unordered_map에 추가
    }
}

IBulletShot* BulletManager::findActiveBulletShots(std::string ShotName) {
    auto it = activeBulletShots.find(ShotName);
    if (it != activeBulletShots.end()) {
        return it->second;
    }
    return nullptr;
}

void BulletManager::Update(CEngine* tEngine, const IBulletOwner& tOwner) 
{
    if (bulletShots.empty()) 
        return;

    if(!activeBulletShots.empty()) {
        for (const std::pair<const std::string, IBulletShot*>& behavior : activeBulletShots) {
            behavior.second->FireBullet(tEngine, tOwner);
            behavior.second->Update_Bullets(tEngine, tOwner);
        }
    }
}

void BulletManager::Render(CEngine* tEngine, const IBulletOwner& tOwner)
{
    if (bulletShots.empty()) 
        return;

    if (!activeBulletShots.empty()) {
        // 모든 활성화된 총알행동 렌더링
        for (const std::pair<const std::string, IBulletShot*>& behavior : activeBulletShots) {
            behavior.second->RenderBullet(tEngine, tOwner);
        }
    }

}



void BulletManager::ReturnBulletToPool(SBullet* bullet)
{
    if (!bullet) return;

    // 총알 상태 초기화
    bullet->Reset();

    bulletPool.push(bullet);
}