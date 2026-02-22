#pragma once
#include "CEngine.h"

class SoftRenderer : public CEngine
{
	SoftRenderer() {};
	virtual ~SoftRenderer() {};
	virtual void OnCreate()override {
		CEngine::OnCreate();
	};
	virtual void OnDestroy()override {
		CEngine::OnDestroy();
	}
};

