#pragma once
#include "Vector2D.h"
#include "queue"
#include "IBulletOwner.h"

class BulletManager;
class CEngine;
struct SBullet;


//총알의 행동(탄 종류)를 인터페이스화
class IBulletShot
{
public:
    virtual ~IBulletShot() = default;

    virtual void RenderBullet(CEngine* tEngine, const IBulletOwner& tOwner) = 0;
    virtual void Update_Bullets(CEngine* tEngine, const IBulletOwner& tOwner) = 0;
    virtual void FireBullet(CEngine* tEngine, const IBulletOwner& tOwner) =0;

    virtual void BindBulletManager(BulletManager* tBindBulletManager)=0;
};

