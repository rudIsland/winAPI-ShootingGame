#pragma once
#include <queue>
#include "IBulletBehavior.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <list>

struct SBullet;
class CEngine;
class IBulletOwner;

class BulletManager
{
public:
    BulletManager();
    ~BulletManager();

    BulletManager(const BulletManager&) = delete;
    BulletManager& operator=(const BulletManager&) = delete;

    void Update(CEngine* tEngine, const IBulletOwner& tOwner);
    void Render(CEngine* tEngine, const IBulletOwner& tOwner);

    // 복사본만 반환
    std::queue<SBullet*> GetActiveBullets() const { return activeBullets; }
    std::queue<SBullet*> GetBulletPools() const { return bulletPool; }

    // 원본에 참조시키기 위해 반환
    std::queue<SBullet*>& GetActiveBulletsRef() { return activeBullets; }
    std::queue<SBullet*>& GetBulletPoolRef() { return bulletPool; }


public:
    // 총알행동 등록
    void AddBulletShots(std::string tAddName, IBulletShot* tAddBehavior);

    // 현재 활성화중인 총알행동 추가, 검색, 삭제, 리스트클리어
    void AddActiveBulletShots(std::string ShotName);
    IBulletShot* findActiveBulletShots(std::string ShotName);

    inline void RemoveActiveBulletShots(const std::string& name)
    {
        activeBulletShots.erase(name);
    }

    inline void ClearActiveBulletShots() {
        activeBulletShots.clear();
    }

    void ReturnBulletToPool(SBullet* bullet);
private:

	int MAX_BULLETS = 100;

	std::queue<SBullet*> bulletPool;  // 총알 큐
	std::queue<SBullet*> activeBullets; //활성화된 총알 큐

	std::unordered_map<std::string, IBulletShot*> bulletShots; //사용가능한 총알행동
	std::unordered_map<std::string, IBulletShot*> activeBulletShots; //현재 발사되고 있는 총알행동


	//=====Getter Setter=====//
public:
    inline std::unordered_map<std::string, IBulletShot*> GetActiveBulletShots() { return activeBulletShots; }


};

