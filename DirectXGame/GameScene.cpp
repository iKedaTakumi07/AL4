#include "GameScene.h"
#include "HitEffect.h"
#include "Math.h"
#include <cassert>

using namespace KamataEngine;

GameScene::~GameScene() {
	// ですトラ
	delete modelblock_;
	delete skydome_;
	delete modelPlayer_;
	delete player_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete modelEnemy_;

	delete modelDeathParticles_;
	delete deathParticles_;

	for (HitEffect* hitEffect : hitEffects_) {
		delete hitEffect;
	}

	delete fade_;
	delete goal;
	delete goalmodel_;
}

void GameScene::Initialize() { /*初期化を書く*/

	// ゲームプレイフェーズから開始
	phase_ = Phase::kFadeIn;

	// 初期化
	camera_.Initialize();

	// 3Dモデルを生成
	modelblock_ = Model::CreateFromOBJ("block", true);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 天球
	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_, &camera_);

	// マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");
	GenerateBlocks();

	// プレイヤー
	player_ = new Player();

	modelPlayer_ = Model::CreateFromOBJ("player");
	modelPlayerAttck_ = Model::CreateFromOBJ("attack_effect");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	player_->SetMapChipField(mapChipField_);
	player_->Initialize(modelPlayer_, modelPlayerAttck_, &camera_, playerPosition);

	// 追跡カメラ
	CameraController_ = new CameraController();
	CameraController_->Initialize(&camera_);
	CameraController_->SetTarget(player_);
	CameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CameraController_->SetMovableArea(cameraArea);

	// 敵
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	for (int32_t i = 0; i < 1; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(25, 2);
		newEnemy->SetMapChipField(mapChipField_);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		newEnemy->SetGameScene(this);
		enemies_.push_back(newEnemy);
	}

	// デスパーティクル
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle");

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	modelHitEffect = Model::CreateFromOBJ("particle");
	HitEffect::SetModel(modelHitEffect);
	HitEffect::SetCamera(&camera_);

	// ゴール
	goal = new Goal();

	goalmodel_ = Model::CreateFromOBJ("enemy");
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(4, 18);
	goal->Initialize(goalmodel_, &camera_, goalPosition);
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {

#pragma region
	{
		AABB aabb1, aabb2, aabb3;

		aabb1 = player_->GetAABB();

		for (Enemy* enemy : enemies_) {
			if (enemy->IsCollisionDisabled())
				continue;

			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突関数を呼び出す
				player_->OnCollision(enemy);

				// 敵キャラの衝突判定を呼び出す
				enemy->OnCollision(player_);
			}
		}

		aabb3 = goal->GetAABB();

		if (IsCollision(aabb1, aabb3)) {
			goal->OnCollision(player_);
		}
	}
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case GameScene::Phase::kPlay:

		if (player_->IsDead()) {
			phase_ = Phase::kDeath;

			const Vector3& dethParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelDeathParticles_, &camera_, dethParticlesPosition);
		}
		if (goal->isCleraed()) {
			phase_ = Phase::kFadeOut;
		}

		break;
	case GameScene::Phase::kDeath:
		break;
	}
}

void GameScene::CreateEffect(const Vector3& position) {
	HitEffect* newHitEffect = HitEffect::Create(position);

	hitEffects_.push_back(newHitEffect);
}

void GameScene::Update() { /* 更新勝利を書く */

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->isDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	ChangePhase();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kPlay;
		}

		skydome_->Update();
		CameraController_->Update();

		// 自キャラの更新
		player_->Update();

		goal->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// UpdateCamera();
#ifdef _DEBUG
		// if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//	// フラグをトグル
		//	isDebugCameraActive_ = !isDebugCameraActive_;
		// }
#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			/*debugCamera_->Update();*/
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				// アフィン変換～DirectXに転送
				WorldtransformUpdate(*worldTransformBlock);
			}
		}
		break;
	case Phase::kPlay:
		skydome_->Update();
		CameraController_->Update();

		// 自キャラの更新
		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

//		UpdateCamera();
#ifdef _DEBUG
		// if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//	// フラグをトグル
		//	isDebugCameraActive_ = !isDebugCameraActive_;
		// }
#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// UpdateBlocks();
		//  ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				// アフィン変換～DirectXに転送
				WorldtransformUpdate(*worldTransformBlock);
			}
		}

		CheckAllCollisions();
		break;
	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
		}

		skydome_->Update();
		CameraController_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished() && goal->isCleraed()) {
			isGoal_ = true;
		}
		if (fade_->IsFinished() && player_->IsDead()) {
			finished_ = true;
		}

		skydome_->Update();
		CameraController_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		break;
	}
}

void GameScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// プレイヤー
	if (!player_->IsDead()) {
		player_->Draw();
	}

	// 背景
	skydome_->Draw();

	goal->Draw();

	// 敵
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelblock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// パーティクル
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	for (HitEffect* hitEffect : hitEffects_) {
		hitEffect->Draw();
	}

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	fade_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}
