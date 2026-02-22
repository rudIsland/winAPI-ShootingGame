#pragma once
#include "Vector2D.h"
#include "CRef.h"
#include "Transform2D.h"
#include <queue>
#include "CTexture.h"
//Bullet관련
#include "BulletManager.h"
#include "SBullet.h"
#include "IBulletOwner.h"
#include "IBulletBehavior.h"


class CEngine;


enum Team{
	Player, Enemy
};


class CUnit : public CRef, public IBulletOwner
{
protected:
	/*=== 멤버변수 ===*/
	Vector2D mPosition; //화면상 위치
	Vector2D mVelocity; //속도와 방향
	Vector2D mForward;  // 각 유닛의 전진 방향

	Vector2D mDisplayPos; //화면에 그릴 좌표
	Vector2D mAnchorPos; //그림의 기준점

	float mScale = 1.0f; //유닛의 크기
	float mSpeed = 1.0f;

	float mWidth; //유닛의 가로길이
	float mHeight; //유닛의 세로길이

	float mHP = 0.0f; //유닛의 체력
	float mMaxHP = 0.0f;
	bool isActive = false;

	Team mTeam;

	//IBulletShot* bulletBehavior;

	//캐릭터의 외관을 표현하는 그래픽 요소 자원
	CTexture* mTexture = nullptr;
	CTexture* mBulletTexture = nullptr;

	BulletManager* mBulletManager;


public:
	//=== 멤버함수 ===//
	CUnit();
	virtual ~CUnit();

	CUnit(const CUnit& t);
	void operator= (const CUnit& t);

	virtual void Create(float x, float y, float tSpeed); //생성
	virtual void Destroy(); //파괴
	virtual void Update(CEngine* tEngine); //갱신 이동
	virtual void Render(CEngine* tEngine); //출력
	Vector2D CalcDisplayPosition() const;
	virtual void LimitPositionToWindow(CEngine* tEngine) {};
	virtual void InItBulletShots() {};

	virtual CUnit* clone() = 0;

	inline bool IsDead() { return mHP <= 0; }
	inline void TakeDamage(float damage) { 
		mHP -= damage;
		if (isActive && mHP <= 0) {
			onDead();
		}
	}

	virtual void onDead() {
		Deactivate();
	}

	Vector2D GetRight() const { return Vector2D(-mForward.mY, mForward.mX); }
	void Move();
	float getRadius();


	//===Provider==//
public:
	void SetTargetProvider(const ITargetProvider* p) { provider = p; }
	const ITargetProvider* GetTargetProvider() const override { return provider; }
private:
	const ITargetProvider* provider = nullptr; // 소유X


//==========Getter Setter============//
public:
	inline void Activate() { isActive = true; }
	inline void Deactivate() { isActive = false; }

	void SetTexture(CTexture* tTexture) { mTexture = tTexture; }
	inline void SetAnchorPoint(float tAnchorX, float tAnchorY) {mAnchorPos.mX = tAnchorX;mAnchorPos.mY = tAnchorY;}
	inline void SetVelocity(const Vector2D tVelocity) {mVelocity = tVelocity;}
	inline void SetDisplayPos(const Vector2D DisplayPos) { mDisplayPos.mX = DisplayPos.mX; mDisplayPos.mY = DisplayPos.mY; }
	inline void SetScale(float scale) { mScale = scale; }
	inline void SetPosition(Vector2D tPosition) { mPosition.mX = tPosition.mX, mPosition.mY = tPosition.mY; }
	inline void SetSpeed(float tSpeed) { mSpeed = tSpeed; }
	inline void SetHP(float tHP) { mHP = tHP; }
	inline void SetMaxHP(float tMaxHP) { mMaxHP = tMaxHP; }
	inline void SetBulletTexture(CTexture* tTexture) { mBulletTexture = tTexture; }
	inline void SetForward(float x, float y) { mForward.mX = x; mForward.mY = y; };
	inline void SetForward(Vector2D forward) { mForward = forward.GetNormalize(); };

	inline void SetWidth(float tWidth) { mWidth = tWidth; }
	inline void SetHeight(float tHeight) { mHeight = tHeight; }
	inline void SetTeam(Team tTeam) { mTeam = tTeam; }

	inline Vector2D getPosition() const { return mPosition; }
	inline Vector2D getVelocity()const {return mVelocity;}
	inline Vector2D getAnchor()const {return mAnchorPos;}
	inline float getScale() const { return mScale; }
	inline float getSpeed()const { return mSpeed; }
	inline Vector2D getForward()const { return mForward; }

	inline bool getIsActive()const { return isActive; }
	inline Team getTeam() const { return mTeam; }

	//===== IBulletOwner 인터페이스 구현 ========//
public:
	Vector2D GetPosition() const override { return mPosition; }
	Vector2D GetForward() const override { return mForward; }
	Vector2D GetAnchor() const override { return mAnchorPos; }
	float GetUnitWidth() const override { return mTexture->GetBitmapInfo().bmWidth * mScale; }
	float GetUnitHeight() const override { return mTexture->GetBitmapInfo().bmHeight * mScale; }
	float GetScale() const override { return mScale; }

	BulletManager* GetBulletManager() const { return mBulletManager; };
};
