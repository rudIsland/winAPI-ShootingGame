#pragma once
#include "CTexture.h"
#include "IBulletBehavior.h"
#include "Vector2D.h"
#include "IBulletOwner.h"
#include <queue>

class CEngine;
struct SBullet;
class BulletManager;

class BulletSpreadShot : public IBulletShot
{
public:
	BulletSpreadShot(CTexture* tBulletTexture, float tDamage, float tSpeed, float tFireInterval, float tAngle ,int tBulletCount);
	~BulletSpreadShot() override;

	void RenderBullet(CEngine* tEngine, const IBulletOwner& tOwner) override;
	void Update_Bullets(CEngine* tEngine, const IBulletOwner& tOwner) override;
	void FireBullet(CEngine* tEngine, const IBulletOwner& tOwner) override;
	//std::queue<SBullet*> GetActiveBullets() const override { return activeBullets; }

	inline void SetDamage(const float tDamage) { mDamage = tDamage; }
	inline void SetSpeed(const float tSpeed) { mSpeed = tSpeed; }
	inline void SetFireInterval(const float tFirInterval) { fireInterval = tFirInterval; }
	inline void SetAngle(const float tAngle) { mAngle = tAngle; }
	inline void SetBulletCount(const float tBulletCount) { mBulletCount = tBulletCount; }

	void BindBulletManager(BulletManager* tBindBulletManager) override;

//private:
//	void ReturnBulletToPool(SBullet* bullet);

private:
	float mAngle;
	float mBulletCount;

	float mDamage = 0.0f;
	float mSpeed = 0.0f;

	//int MAX_BULLETS = 50;
	float fireTimer = 0.0f;          // 발사 타이머
	float fireInterval = 3.0f; // 1초 간격


	CTexture* bulletTexture = nullptr; //해당 공격방식이 가질 텍스처

	BulletManager* mBindBulletManager = nullptr; //총알 매니저 바인딩

	//std::queue<SBullet*> bulletPool;  // 총알 큐
	//std::queue<SBullet*> activeBullets; //활성화된 총알 큐
};

