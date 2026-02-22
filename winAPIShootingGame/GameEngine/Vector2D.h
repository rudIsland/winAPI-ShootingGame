#pragma once
//연속적인 2D공간을 다루는 벡터 구조체 정의
#define _USE_MATH_DEFINES
#include <math.h>

struct Vector2D
{
	float mX = 0.0f;
	float mY = 0.0f;


	//기본 생성자(매개 변수 없는 생성자)
	Vector2D() : mX(0.0f), mY(0.0f) {}
	//매개변수 있는 생성자
	Vector2D(float tX, float tY) : mX(tX), mY(tY) {}

	//복사 생성자
	Vector2D(const Vector2D& t) {
		mX = t.mX;
		mY = t.mY;
	}

	//복사 대입 연산자. 연쇄 대입은 고려하지 않음.
	inline void operator=(const Vector2D& t) {
		mX = t.mX;
		mY = t.mY;
	}

	//벡터 끼리의 덧셈
	inline Vector2D operator+ (const Vector2D& t) {
		return Vector2D(mX + t.mX, mY + t.mY);
	}

	//벡터 끼리의 뺄셈
	inline Vector2D operator- (const Vector2D& t) {
		return Vector2D(mX - t.mX, mY - t.mY);
	}

	//벡터의 스칼라 곱셈
	inline Vector2D operator* (float t) {
		return Vector2D(mX * t, mY * t);
	}

	float Dot(const Vector2D& t) const {
		return mX * t.mX + mY * t.mY;
	}

	float Length() const {
		return sqrtf(mX * mX + mY * mY);
	}

	Vector2D GetNormalize() {
		float len = Length();
		if (len > 0.0f) {
			return Vector2D(mX / len, mY / len); // 복사본 반환, 자기 자신은 그대로
		}
		else 
			return Vector2D(0, 0);
	}

	

	inline Vector2D Forward() { return Vector2D(0.0f, 1.0f); }
};

