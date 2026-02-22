#pragma once
#include "Vector2D.h"

// 총알 오브젝트 구조체
struct SBullet
{
	Vector2D position;
	Vector2D velocity;
	Vector2D direction;
	float mDamage;
	float mSpeed;
	float lifeTime; // 총알 생존 시간
	float mScale;
	bool isActive = false;

	inline void SetVelocity(Vector2D tVelocity) { velocity = tVelocity; }
	inline void SetPosition(Vector2D tPosition) { position = tPosition; }
	inline void SetPosition(float x, float y) { position.mX = x, position.mY = y; }

	inline void Reset() {
		isActive = false;
		lifeTime = 0.0f;
		position = { 0, 0 };
		velocity = { 0, 0 };
		direction = { 0,0 };
	}
};