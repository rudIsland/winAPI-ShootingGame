//#pragma once
//#define _USE_MATH_DEFINES
//#include <math.h>
//#include "Vector2D.h"
//
//struct Transform2D {
//	Vector2D position;
//	float rotation = 0.0f; // degree
//
//	// 입력된 벡터를 회전값만큼 회전시킴
//	Vector2D RotateVector(const Vector2D& vec) const {
//		float rad = rotation * (float)(M_PI / 180.0f);
//		float cosA = cosf(rad);
//		float sinA = sinf(rad);
//
//		return Vector2D(
//			vec.mX * cosA - vec.mY * sinA,
//			vec.mX * sinA + vec.mY * cosA
//		);
//	}
//
//	// 회전 방향 벡터
//	Vector2D Forward() const {
//		float rad = rotation * (float)(M_PI / 180.0f);
//		return Vector2D(cosf(rad), sinf(rad));
//	}
//
//	Vector2D Right() const {
//		float rad = (rotation + 90.0f) * (float)(M_PI / 180.0f);
//		return Vector2D(cosf(rad), sinf(rad));
//	}
//};