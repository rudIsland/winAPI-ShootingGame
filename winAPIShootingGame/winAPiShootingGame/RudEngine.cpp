// 표준 라이브러리
#include <iostream>

// 엔진 및 기타, 매크로
#include "RudEngine.h"
#include "rudmacro.h"
#include "CInputMgr.h"

// 유닛, 총알 객체
#include "CPlayerUnit.h"
#include "CEnemyUnit.h"   // 적 유닛 클래스
#include "CBossScarab.h"

#include "BulletOneStraightShot.h"
#include "BulletTwoStraightShot.h"
#include "BulletRoundShot.h"
#include "BulletPlayerAimShot.h"

// 리소스
#include "CTexture.h"
using namespace std;

#pragma comment(lib, "GameEngine.lib")

RudEngine::RudEngine() {}

RudEngine::~RudEngine() {}

void RudEngine::OnCreate()
{
	srand(static_cast<unsigned>(time(nullptr))); // 시드 한 번만

	OutputDebugString(L"RudEngine::OnCreate()\n");

	AddKeyToInputMgr(); //Private Method <-- 이곳에서 키 추가 시키면 됨.

	LoadTextureResource(); //Private Method <-- 이곳에서 텍스처를 추가시키면 됨


	//===플레이어 프리팹 설정==//
	PFPlayerUnit = CreatePrefab<CPlayerUnit>(
		mPlayerTex,				//imageTexture
		mBulletTex,				//Bullet Texture
		Vector2D(0.5f, 0.5f),	//Anchor
		Vector2D(0, 0),			//DisPlay
		300.0f,					//speed
		500.0f,					//HP
		500.0f,					//MaxHP
		0.5f,					//Scale
		Team::Player			//Team
		);					

	mPlayerUnit = InstanceUnit<CPlayerUnit>(PFPlayerUnit);
	mPlayerUnit->SetForward(0.0f, -0.1f);
	
	mPlayerUnit->onGameOverCallback = [this]() {
		GameOver(); //RudEngine의 GameOver를 가져와야하므로 자기자신 this를 캡처절로
		OutputDebugString(L"********************* GAME OVER *********************\n");
		};

	PFBossScarabUnit = CreatePrefab<CBossScarab>(
		mBossScarabTex,			//imageTexture
		mEnemyBulletTex,		//Bullet Texture
		Vector2D(0.5f, 0.5f),	//Anchor
		Vector2D(0, 0),			//position
		100.0f,					//speed
		4000.0f,				//currentHP
		4000.0f,				//MaxHP
		0.2f,					//Scale
		Team::Enemy				//Team
		);



	mBossScarabUnit = InstanceUnit<CBossScarab>(PFBossScarabUnit); //프리팹을 복제하여 새로운 객체유닛 생성
	mBossScarabUnit->SetForward(0.0f, 0.1f);
	mBossScarabUnit->SetTargetProvider(this); //Provider 주입



	//===유닛들을 등록===//
	mUnits.push_back(mPlayerUnit);
	mUnits.push_back(mBossScarabUnit);

	//==유닛 스폰==//
	mPlayerUnit->SetPosition(Vector2D(ClientWidth * mPlayerUnit->getAnchor().mX, ClientHeight - 100));
	mPlayerUnit->Activate();

	mBossScarabUnit->SetPosition(Vector2D(ClientWidth * mBossScarabUnit->getAnchor().mX, 100));
	mBossScarabUnit->Activate();

}

void RudEngine::AddKeyToInputMgr()
{
	CInputMgr::GetInst()->AddKeyInfo("DoMoveLt", 'A');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveRt", 'D');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveFoward", 'W');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveBack", 'S');
}

void RudEngine::LoadTextureResource()
{
	//자원을 모두 구축한다.
	mPlayerTex = new CTexture();
	mPlayerTex->LoadTexture(hInst, mhdc, L"resource/bongbong_0.bmp");
	mTextures.push_back(mPlayerTex);

	mBulletTex = new CTexture();
	mBulletTex->LoadTexture(hInst, mhdc, L"resource/BulletNoBack2.bmp");
	mTextures.push_back(mBulletTex);

	mBossScarabTex = new CTexture();
	mBossScarabTex->LoadTexture(hInst, mhdc, L"resource/Boss4.bmp");
	mTextures.push_back(mBossScarabTex);

	mEnemyBulletTex = new CTexture();
	mEnemyBulletTex->LoadTexture(hInst, mhdc, L"resource/Boss4_Bullet_80x80.bmp");
	mTextures.push_back(mEnemyBulletTex);
}



void RudEngine::OnUpdate()
{
	//부모 클래스 update
	CEngine::OnUpdate();

	if (isGameOver) return;

	// 1) 유닛들 갱신
	for (auto unit : mUnits) {
		if (unit->getIsActive())
			unit->Update(this);
	}

	// 2) 충돌 검사
	CheckAllCollistions();


	//3) 지나간 잔상을 제거후(백 버퍼 지우기)
	this->Clear(0.1f, 0.1f, 0.8f);

	// 4) 유닛들 렌더
	for (auto unit : mUnits) {
		if(unit->getIsActive())
			unit->Render(this);
	}
	//완성된 버퍼를 프론트버퍼로 가져오기
	this->Present();

}


void RudEngine::CheckAllCollistions()
{
	for (auto attacker : mUnits) {
		if (!attacker->getIsActive()) continue;
		auto bullets = attacker->GetBulletManager()->GetActiveBullets();

		for (auto target : mUnits) {
			if (!target->getIsActive()) continue;
			if (attacker == target) continue; // 자기 자신 제외
			if (attacker->getTeam() == target->getTeam()) continue; // 같은 팀 제외

			const float bulletRadius = 5.0f;
			float targetRadius = target->getRadius();

			// bullets 큐를 복사 대신 참조(혹은 반복자 기반으로) 순회
			std::queue<SBullet*> tmp = bullets;
			while (!tmp.empty()) {
				SBullet* bullet = tmp.front();
				tmp.pop();

				if (!bullet->isActive) continue;

				if (isUnitCircleColliding(bullet->position, bulletRadius,
					target->getPosition(), targetRadius))
				{
					target->TakeDamage(bullet->mDamage);
					bullet->isActive = false;
				}
			}
		}
	}
}



bool RudEngine::isUnitCircleColliding(Vector2D pos1, float radius1, Vector2D pos2, float radius2)
{
	float dx = pos1.mX - pos2.mX;
	float dy = pos1.mY - pos2.mY;
	float distSq = dx * dx + dy * dy; //중심점 사이의 거리 반환

	//두 중심점 사이 거리가 반지름 합보다 크면 떨어져있는 것. (거리가 짧아야 충돌)
	float radiusSum = radius1 + radius2;
	return distSq < (radiusSum * radiusSum);
}

void RudEngine::OnDestroy()
{
	OutputDebugString(L"RudEngine::OnDestroy()\n");


	//유닛 해제
	for (auto t = mUnits.begin(); t != mUnits.end(); ++t) {
		SAFE_RELEASE((*t));
	}

	DestoryPrefab(PFPlayerUnit);
	DestoryPrefab(PFBossScarabUnit);

	mUnits.clear();

	//텍스쳐 제거
	for (auto t = mTextures.begin(); t != mTextures.end(); ++t) {
		(*t)->UnLoadTexture();
		SAFE_DELETE((*t));
	}


	CEngine::OnDestroy();
}

void RudEngine::RemoveDeadUnits()
{
	for (auto it = mUnits.begin(); it != mUnits.end(); ) {
		CUnit* unit = *it;

		// 플레이어는 죽여도 삭제하지 않음 (GameOver만 발생)
		if (unit != mPlayerUnit && unit->IsDead()) {
			SAFE_RELEASE(unit);
			it = mUnits.erase(it);
		}
		else {
			++it;
		}
	}
}