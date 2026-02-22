#pragma once
#include "CEnemyUnit.h"


class CBossScarab : public CEnemyUnit
{
private:
	float mMoveSpeed=80.0f; //좌우 움직이는 속도
	float mMoveTime=0.0f; //현재까지 움직인 시간
	float mTpInterval = 2.0f; //순간이동 시간 간격

	int mCurrentPhase = -1;
	int mMoveType = 0;

	int GetPhaseByHP();


private:
	inline float step(float edge, float x) {
		return x < edge ? 0.0f : 1.0f;
	}

	
public:
	CBossScarab();
	virtual ~CBossScarab();
	CBossScarab(const CBossScarab& t);

	virtual void Update(CEngine* tEngine) override;
	void ApplyPhaseSettings(int phase);
	void MoveLR(CEngine* tEngine);
	void RandomMoveLR(CEngine* tEngine);
	virtual void Render(CEngine* tEngine) override;
	virtual void LimitPositionToWindow(CEngine* tEngine) {};
	virtual void InItBulletShots() override;

	BulletManager* GetBulletManager() const { return mBulletManager; };

public:
	//클론
	virtual CUnit* clone() override {
		return new CBossScarab(*this);
	}
};

