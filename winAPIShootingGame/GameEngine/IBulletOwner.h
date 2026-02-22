#pragma once
#include "Vector2D.h"

struct ITargetProvider;

class IBulletOwner
{
public:
    virtual ~IBulletOwner() = default;

    // 발사 주체의 중심 좌표
    virtual Vector2D GetPosition() const = 0;

    // 발사 방향(정규화)
    virtual Vector2D GetForward() const = 0;

    // 텍스처 기준점 (Anchor)
    virtual Vector2D GetAnchor() const = 0;

    // 유닛 크기 정보
    virtual float GetUnitWidth() const = 0;
    virtual float GetUnitHeight() const = 0;

    // 스케일
    virtual float GetScale() const = 0;

    //Provider
    virtual const ITargetProvider* GetTargetProvider() const = 0;
};