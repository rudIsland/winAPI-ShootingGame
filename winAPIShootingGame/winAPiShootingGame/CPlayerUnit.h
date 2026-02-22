#pragma once
#include "CUnit.h"
#include <functional>
class CEngine;

class CPlayerUnit : public CUnit
{
private:
	//const int MAX_BULLETS = 30;
	//float fireTimer = 0.0f;          // 발사 타이머
	//const float fireInterval = 1.0f; // 0.3초 간격

public:
	CPlayerUnit();
	virtual ~CPlayerUnit();

	CPlayerUnit(const CPlayerUnit& t);

	
	std::function<void()> onGameOverCallback; //콜백함수

	virtual void Update(CEngine* tEngine) override;
	void Update_PlayerMove(CEngine* tEngine);
	virtual void Render(CEngine* tEngine) override;
	virtual void LimitPositionToWindow(CEngine* tEngine);
	virtual void onDead() override{
		Deactivate();
		TriggerGameOver();
	}
	virtual void InItBulletShots() override;


	void TriggerGameOver() {
		onGameOverCallback();
	}
	

	//클론
	virtual CUnit* clone() override {
		return new CPlayerUnit(*this);
	}
};

