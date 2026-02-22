#include "CEnemyUnit.h"
#include "CEngine.h"
#include "CTexture.h"
#include "CInputMgr.h"
#include "CUnit.h"

CEnemyUnit::CEnemyUnit() { }

CEnemyUnit::~CEnemyUnit() {
    /*delete bulletBehavior;
    bulletBehavior = nullptr;*/
}

CEnemyUnit::CEnemyUnit(const CEnemyUnit& t) : CUnit(t){
}

void CEnemyUnit::Update(CEngine* tEngine)
{
   /* if (bulletBehavior) {
        bulletBehavior->Update_Bullets(tEngine, *this);
        bulletBehavior->FireBullet(tEngine, *this);
    }*/
}



void CEnemyUnit::Render(CEngine* tEngine)
{
    // 방어코드
    if (!mTexture || !tEngine) return;

    // display 위치 계산
    mDisplayPos = CalcDisplayPosition();

    // 3. 실제 그리기
    tEngine->DrawtextureScaled(mDisplayPos.mX,
        mDisplayPos.mY,
        mTexture,
        mScale);

    // 총알들 렌더링
    /*if (bulletBehavior) {
        bulletBehavior->RenderBullet(tEngine, *this);
    }*/
}
