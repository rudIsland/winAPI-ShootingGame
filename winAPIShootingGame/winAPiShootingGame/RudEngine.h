#pragma once

#include "CEngine.h"
#include <list>
#include "CPlayerUnit.h"
#include "CEnemyUnit.h"
#include "ITargetProvider.h"

class CUnit;
class CTexture;
class SBUllet;
class CBossScarab;

class RudEngine : public CEngine, public ITargetProvider
{
public:
    RudEngine();
    virtual ~RudEngine();

    RudEngine(const RudEngine& t) = delete; //복사 생성자 금지
    RudEngine& operator=(const RudEngine& t) = delete; //복사 대입 연산자 금지

    virtual void OnCreate() override;
    void LoadTextureResource();
    void AddKeyToInputMgr();
    virtual void OnDestroy() override;
    virtual void OnUpdate() override;

private:

    bool isGameOver = false;

    //플레이어
    CUnit* PFPlayerUnit = nullptr;
    CPlayerUnit* mPlayerUnit = nullptr;
    CTexture* mPlayerTex = nullptr;
    CTexture* mBulletTex = nullptr;

    //적
    //CUnit* PFEnemyUnit = nullptr;
    //기본 Enemy유닛 및 리소스
    //CEnemyUnit* mEnemyBossUnit = nullptr; //원래 적
    //CTexture* mEnemyBossTex = nullptr;
    //CTexture* mEnemyBulletTex = nullptr;
    
    //Scarab유닛 및 리소스
    CUnit* PFBossScarabUnit = nullptr;
    CBossScarab* mBossScarabUnit = nullptr; //새로 선언한 전갈보스
    CTexture* mBossScarabTex = nullptr;
    CTexture* mEnemyBulletTex = nullptr;
    
    std::list<CUnit*> mUnits;
    std::list<CTexture*> mTextures;

public:
    void CheckAllCollistions();

    /*void CheckPlayerBulletEnemyCollision(CUnit* attacker, CUnit* target);
    void CheckEnemyBulletPlayerCollistion(CUnit* attacker, CUnit* target);*/
    bool isUnitCircleColliding(Vector2D pos1, float radius1, Vector2D pos2, float radius2);

    virtual Vector2D GetTargetIsPlayerGetPosition() const override {
        return mPlayerUnit ? mPlayerUnit->GetPosition() : Vector2D{ 0,0 };
    }

    inline void GameOver() { isGameOver = true; } //플레이어의 onDead에서 한번만 수행

public:
    inline CTexture* GetBulletTexture() { return mBulletTex; }  // 총알 텍스처 getter

private:
    void RemoveDeadUnits();

};

