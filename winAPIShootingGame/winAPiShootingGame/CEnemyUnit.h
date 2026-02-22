#pragma once
#include "CUnit.h"

class CEngine;

class CEnemyUnit : public CUnit
{
public:
	CEnemyUnit();
	virtual ~CEnemyUnit();

	CEnemyUnit(const CEnemyUnit& t);

	virtual void Update(CEngine* tEngine) override;
	virtual void Render(CEngine* tEngine) override;
	virtual void LimitPositionToWindow(CEngine* tEngine) {};

public:
	//ХЌЗа
	virtual CUnit* clone() override {
		return new CEnemyUnit(*this);
	}

};

