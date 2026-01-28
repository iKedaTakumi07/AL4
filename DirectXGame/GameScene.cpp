#include "GameScene.h"
#include "AbilityItem.h"
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

	for (Bullet* bullet : PlayerBullet_) {
		delete bullet;
	}

	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (ExplosionEnemy* exEnemy : ExplosionEnemies_) {
		delete exEnemy;
	}

	delete modelEnemy_;

	delete modelDeathParticles_;
	delete deathParticles_;

	delete modelplayerHp_;

	delete ui_;
	for (HitEffect* hitEffect : hitEffects_) {
		delete hitEffect;
	}

	for (AbilityItem* abilityItem : AbilityItem_) {
		delete abilityItem;
	}

	delete fade_;
	delete goal;
	delete goalmodel_;

	delete modelCheese_;

	delete info_;
	delete GameUI_;
}

void GameScene::Initialize() { /*初期化を書く*/

	soundBGM = Audio::GetInstance()->LoadWave("stage.wav");

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
	mapChipField_->LoadMapChipCsv("Resources/NewMap.csv");
	blockTextureHandele = TextureManager::Load("block/BreakableBlock.png");
	chageBlockTextureHandele = TextureManager::Load("block/chargeBreakableBlock.png");

	modelEnemy_ = Model::CreateFromOBJ("newEnemy");

	modelCheese_ = Model::CreateFromOBJ("cheese");
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelPlayerAttck_ = Model::CreateFromOBJ("attack_effect");
	modelBullet_ = Model::CreateFromOBJ("bullet");

	// ゴール

	goalmodel_ = Model::CreateFromOBJ("goal");

	GenerateBlocks();

	// ui
	modelplayerHp_ = Model::CreateFromOBJ("playerHp");
	int playerHp = player_->GetPlayerHp();
	ui_ = new GameSceneUI();
	ui_->Initialize(modelplayerHp_, &camera_);
	ui_->SetPlayerHp(playerHp);

	// 追跡カメラ
	CameraController_ = new CameraController();
	CameraController_->Initialize(&camera_);
	CameraController_->SetTarget(player_);
	CameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 33.0f, 6.0f};
	CameraController_->SetMovableArea(cameraArea);

	// デスパーティクル
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle");

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	modelHitEffect = Model::CreateFromOBJ("particle");
	HitEffect::SetModel(modelHitEffect);
	HitEffect::SetCamera(&camera_);

	bulletTextTextureHandle = TextureManager::Load("AbilityInfo/bulletText.png");
	SpaceJumpTexttextureHandle = TextureManager::Load("AbilityInfo/SpaceJumpText.png");
	ChargeTextTextureHandle = TextureManager::Load("AbilityInfo/chargeText.png");

	info_ = new AbilityInfo();
	info_->Initialize(bulletTextTextureHandle, Vector2(140.0f, 200.0f));

	GameUIMoveTextureHandle = TextureManager::Load("GameUI/MoveText.png");
	GameUIShotTextureHandle = TextureManager::Load("GameUI/ShotText.png");
	GameUISpaceTextureHandle = TextureManager::Load("GameUI/SpaceJumpText.png");
	GameUIChargeTextureHandle = TextureManager::Load("GameUI/ChargeText.png");

	GameUI_ = new GameUI();
	GameUI_->Initialize(GameUIMoveTextureHandle, Vector2(0.0f, 720.0f - 64.0f));
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

			MapChipType mapChipType;
			uint8_t SubID;
			Enemy* newEnemy;
			ExplosionEnemy* NeweEnemy;
			AbilityItem* abilityItem;
			Vector3 spawnPos;
			mapChipType = mapChipField_->GetMapChipTypeByIndex(j, i);

			WorldTransform* worldTransform = nullptr;
			switch (mapChipType) {
			case MapChipType::kBlock:
				worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

				SubID = mapChipField_->GetMapChipSubIDByIndex(j, i);
				switch (SubID) {
				case 0:

					break;
				case 1:
					mapChipField_->SetMapChipType(j, i, MapChipType::kBreakableBlock);

					break;
				case 2:
					mapChipField_->SetMapChipType(j, i, MapChipType::kChageBreakeBlock);
					break;
				default:
					break;
				}
				break;
			case MapChipType::kPlayer:
				assert(player_ == nullptr && "自キャラを2重に配置しようとしています");
				// プレイヤー
				player_ = new Player();
				spawnPos = mapChipField_->GetMapChipPositionByIndex(2, 14);

				player_->SetMapChipField(mapChipField_);
				player_->Initialize(modelPlayer_, modelPlayerAttck_, &camera_, spawnPos);
				break;
			case MapChipType::kEnemy:

				SubID = mapChipField_->GetMapChipSubIDByIndex(j, i);
				switch (SubID) {
				case 0:
					newEnemy = new Enemy();
					spawnPos = mapChipField_->GetMapChipPositionByIndex(j, i);

					newEnemy->SetMapChipField(mapChipField_);
					newEnemy->Initialize(modelEnemy_, &camera_, spawnPos);
					newEnemy->SetGameScene(this);
					enemies_.push_back(newEnemy);

					break;
				case 1:
					NeweEnemy = new ExplosionEnemy();
					spawnPos = mapChipField_->GetMapChipPositionByIndex(j, i);
					NeweEnemy->SetMapChipField(mapChipField_);
					NeweEnemy->Initialize(modelEnemy_, &camera_, spawnPos);
					NeweEnemy->SetGameScene(this);
					ExplosionEnemies_.push_back(NeweEnemy);

					break;
				case 2:
					break;
				default:
					break;
				}
				break;
			case MapChipType::kAbility:
				SubID = mapChipField_->GetMapChipSubIDByIndex(j, i);

				switch (SubID) {
				case 0:
					abilityItem = new AbilityItem();
					spawnPos = mapChipField_->GetMapChipPositionByIndex(j, i);
					abilityItem->SetNunber(uint32_t(0));
					abilityItem->Initialize(modelCheese_, &camera_, spawnPos);
					AbilityItem_.push_back(abilityItem);
					SubID = mapChipField_->GetMapChipSubIDByIndex(j, i);

					break;
				case 1:
					abilityItem = new AbilityItem();
					spawnPos = mapChipField_->GetMapChipPositionByIndex(j, i);
					abilityItem->SetNunber(uint32_t(1));
					abilityItem->Initialize(modelCheese_, &camera_, spawnPos);
					AbilityItem_.push_back(abilityItem);
					SubID = mapChipField_->GetMapChipSubIDByIndex(j, i);

					break;
				case 2:
					abilityItem = new AbilityItem();
					spawnPos = mapChipField_->GetMapChipPositionByIndex(j, i);
					abilityItem->SetNunber(uint32_t(2));
					abilityItem->Initialize(modelCheese_, &camera_, spawnPos);
					AbilityItem_.push_back(abilityItem);
					SubID = mapChipField_->GetMapChipSubIDByIndex(j, i);

					break;
				default:
					break;
				}

				break;
			case MapChipType::kGoal:
				goal = new Goal();
				spawnPos = mapChipField_->GetMapChipPositionByIndex(j, i);
				goal->Initialize(goalmodel_, &camera_, spawnPos);
				break;
			default:
				break;
			}
		}
	}
}

void GameScene::CheckAllCollisions() {

#pragma region
	{
		AABB aabbPlayer, aabbEnemy, aabbGoal, aabbPBullet, aabbExpEnemy, aabbAbility;

		aabbPlayer = player_->GetAABB();

		for (AbilityItem* ability : AbilityItem_) {
			if (ability->IsCollisionDisabled())
				continue;

			aabbAbility = ability->GetAABB();

			if (IsCollision(aabbPlayer, aabbAbility)) {
				uint32_t num;

				num = ability->GetNumber();
				player_->OnCollision(num);
				if (num == 0) {
					info_->SetTextureHandle(bulletTextTextureHandle);
					GameUI_->SetTextureHandle(GameUIShotTextureHandle);
				} else if (num == 1) {
					info_->SetTextureHandle(SpaceJumpTexttextureHandle);
					GameUI_->SetTextureHandle(GameUISpaceTextureHandle);
				} else if (num == 2) {
					info_->SetTextureHandle(ChargeTextTextureHandle);
					GameUI_->SetTextureHandle(GameUIChargeTextureHandle);
				}
				ability->OnCollision();

				phase_ = Phase::kInformation;
			}
		}

		for (Enemy* enemy : enemies_) {
			if (enemy->IsCollisionDisabled())
				continue;

			aabbEnemy = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabbPlayer, aabbEnemy)) {
				// 自キャラの衝突関数を呼び出す
				player_->OnCollision(enemy, aabbEnemy, aabbPlayer);

				int playerhp;
				playerhp = player_->GetPlayerHp();
				ui_->SetPlayerHp(playerhp);

				// 敵キャラの衝突判定を呼び出す
				enemy->OnCollision(player_);
			}

			for (Bullet* bullet : PlayerBullet_) {
				if (bullet->IsDead())
					continue;

				aabbPBullet = bullet->GetAABB();

				// 弾と敵の当たり判定
				if (IsCollision(aabbEnemy, aabbPBullet)) {
					enemy->OnCollision(bullet->isGetLevel());

					bullet->OnCollision();
				}
			}
		}

		for (ExplosionEnemy* enemy : ExplosionEnemies_) {
			if (enemy->IsCollisionDisabled())
				continue;
			aabbExpEnemy = enemy->GetAABBExplosion();

			if (enemy->isDead()) {
				// AABB同士の交差判定
				if (IsCollision(aabbPlayer, aabbExpEnemy)) {
					// 自キャラの衝突関数を呼び出す
					player_->OnCollision(enemy, aabbExpEnemy, aabbPlayer);

					int playerhp;
					playerhp = player_->GetPlayerHp();
					ui_->SetPlayerHp(playerhp);
				}
			}

			aabbExpEnemy = enemy->GetAABB();
			// AABB同士の交差判定
			if (IsCollision(aabbPlayer, aabbExpEnemy)) {
				// 自キャラの衝突関数を呼び出す
				player_->OnCollision(enemy, aabbExpEnemy, aabbPlayer);

				int playerhp;
				playerhp = player_->GetPlayerHp();
				ui_->SetPlayerHp(playerhp);

				// 敵キャラの衝突判定を呼び出す
				enemy->OnCollision(player_);
			}

			for (Bullet* bullet : PlayerBullet_) {
				if (bullet->IsDead())
					continue;

				aabbPBullet = bullet->GetAABB();

				// 弾と敵の当たり判定
				if (IsCollision(aabbExpEnemy, aabbPBullet)) {
					enemy->OnCollision(bullet->isGetLevel());

					bullet->OnCollision();
				}
			}
		}

		aabbGoal = goal->GetAABB();

		if (IsCollision(aabbPlayer, aabbGoal)) {
			goal->OnCollision(player_);
		}

		// ブロックが破壊されたとき存在しないようにする
		uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
		uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

		for (uint32_t i = 0; i < numBlockVirtical; i++) {
			for (uint32_t j = 0; j < numBlockHorizontal; j++) {
				if (mapChipField_->GetMapChipTypeByIndex(j, i) != MapChipType::kBlank)
					continue;

				if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlank) {
					if (worldTransformBlocks_[i][j] == nullptr)
						continue;
					delete worldTransformBlocks_[i][j];
					worldTransformBlocks_[i][j] = nullptr;
				}
			}
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

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		// フラグをトグル
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
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
		CameraController_->Update();
	}

	ui_->Update();

	// bgm
	if (!Audio::GetInstance()->IsPlaying(voiceHandel)) {
		// voiceHandel = Audio::GetInstance()->PlayWave(soundBGM, true, 0.5f);
	}

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	ExplosionEnemies_.remove_if([](ExplosionEnemy* enemy) {
		if (enemy->isDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	PlayerBullet_.remove_if([](Bullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	AbilityItem_.remove_if([](AbilityItem* Ability) {
		if (Ability->isDead()) {
			delete Ability;
			return true;
		}
		return false;
	});

	skydome_->Update();

	// ブロックの更新

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			// アフィン変換～DirectXに転送
			WorldtransformUpdate(*worldTransformBlock);
		}
	}

	ChangePhase();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kPlay;
		}

		// 自キャラの更新
		player_->Update();

		goal->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (ExplosionEnemy* enemy : ExplosionEnemies_) {
			enemy->Update();
		}

		for (AbilityItem* Ability : AbilityItem_) {
			Ability->Update();
		}

		break;
	case Phase::kPlay:

		// 自キャラの更新
		player_->Update();

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

		goal->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (ExplosionEnemy* enemy : ExplosionEnemies_) {
			enemy->Update();
		}

		for (AbilityItem* Ability : AbilityItem_) {
			Ability->Update();
		}

		// チャージショット

		if (player_->isGetShot()) {

			// プレイヤー位置を基準に弾をスポーン
			Vector3 spawnPos = player_->GetWorldPosition();
			Vector3 direction = player_->Getdirection();
			float charge = player_->isGetCharge();
			// プレイヤーの向きがわかればそれに合わせる

			Bullet* newBullet = new Bullet();
			newBullet->Initialize(modelBullet_, &camera_, spawnPos, charge);
			newBullet->Setdirection(direction);
			newBullet->SetMapChipField(mapChipField_);

			PlayerBullet_.push_back(newBullet);
		}

		for (Bullet* bullet : PlayerBullet_) {
			bullet->Update();
		}

		CheckAllCollisions();
		break;
	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished() && goal->isCleraed()) {
			isGoal_ = true;
			Audio::GetInstance()->StopWave(voiceHandel);
		}
		if (fade_->IsFinished() && player_->IsDead()) {
			finished_ = true;
			Audio::GetInstance()->StopWave(voiceHandel);
		}

		goal->Update();

		break;
	case Phase::kInformation:

		info_->Update();

		if (Input::GetInstance()->PushKey(DIK_RETURN)) {
			phase_ = Phase::kPlay;
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

	for (ExplosionEnemy* enemy : ExplosionEnemies_) {
		enemy->Draw();
	}

	for (Bullet* bullet : PlayerBullet_) {
		bullet->Draw();
	}

	for (AbilityItem* Ability : AbilityItem_) {
		Ability->Draw();
	}

	// ブロックが破壊されたとき存在しないようにする
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) != MapChipType::kBlock && mapChipField_->GetMapChipTypeByIndex(j, i) != MapChipType::kBreakableBlock &&
			    mapChipField_->GetMapChipTypeByIndex(j, i) != MapChipType::kChageBreakeBlock)
				continue;

			if (mapChipField_->GetMapChipSubIDByIndex(j, i) == 1) {
				modelblock_->Draw(*worldTransformBlocks_[i][j], camera_, blockTextureHandele);
			} else if (mapChipField_->GetMapChipSubIDByIndex(j, i) == 2) {
				modelblock_->Draw(*worldTransformBlocks_[i][j], camera_, chageBlockTextureHandele);
			} else {
				modelblock_->Draw(*worldTransformBlocks_[i][j], camera_);
			}
		}
	}

	ui_->Draw();

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

	GameUI_->Draw();

	if (phase_ == Phase::kInformation) {
		info_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();
}
