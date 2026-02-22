#pragma once

#include "Vector2D.h"

struct ITargetProvider {
	virtual ~ITargetProvider() = default;
	virtual Vector2D GetTargetIsPlayerGetPosition() const = 0;
};