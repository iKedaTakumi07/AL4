#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	// ですトラ
	delete modelblock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete modelPlayer_;
	delete player_;
	delete mapChipField_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() { /*初期化を書く*/

	// 初期化
	camera_.Initialize();
	camera_.farZ = 0.0f;

	player_ = new Player();

	modelPlayer_ = Model::CreateFromOBJ("player");

	// 座標を指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 19);

	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	// 3Dモデルを生成
	modelblock_ = Model::CreateFromOBJ("block", true);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField;

	// 読み込み
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	GenerateBlocks();

	// 追跡カメラ
	CameraController_ = new CameraController();
	CameraController_->Initialize(&camera_);
	CameraController_->SetTarget(player_);
	CameraController_->Reset();
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

void GameScene::Update() { /* 更新勝利を書く */

	// プレイヤー
	player_->Update();

	// 背景
	skydome_->Update();

	// 追跡カメラ
	CameraController_->Update();

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG

	if (isDebugCameraActive_) {
		// デバックカメラ更新
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;

		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			// アフィン変換
			WolrdtransformUpdate(*worldTransformBlock);
		}
	}

	// デバックカメラ更新
	debugCamera_->Update();
}

void GameScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// プレイヤー
	player_->Draw();

	// 背景
	skydome_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelblock_->Draw(*worldTransformBlock, camera_);
		}
	}

	Model::PostDraw();
}
