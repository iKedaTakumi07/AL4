#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;

void GameScene::Initialize() { /*初期化を書く*/
	// 3Dモデルを生成
	modelblock_ = Model::CreateFromOBJ("block", true);
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 読み込み
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");
	GenerateBlocks();

	// 初期化
	camera_.farZ = 0.0f;
	camera_.Initialize();

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	player_ = new Player();
	// 座標を指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 19);
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	// 追従カメラ
	CameraController_ = new CameraController(); // 生成
	CameraController_->Initialize(&camera_);    // 初期化
	CameraController_->SetTarget(player_);      // 追従対象セット
	CameraController_->Reset();                 // リセット

	// 初期位置
	//CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	//CameraController_->SetMovablearea(cameraArea);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
}

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

void GameScene::Update() { /* 更新勝利を書く */

	// プレイヤー
	player_->Update();
	// 背景
	skydome_->Update();

	CameraController_->Update();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			// アフィン変換
			WolrdtransformUpdate(*worldTransformBlock);
		}
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}

	if (isDebugCameraActive_) {
		// デバックカメラ更新
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;

		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

#endif // _DEBUG
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
