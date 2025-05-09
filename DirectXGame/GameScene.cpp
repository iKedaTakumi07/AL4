#include "GameScene.h"
#include "WorldTransForm.h"

using namespace KamataEngine;

void GameScene::Initialize() { /*初期化を書く*/
	// 3Dモデルを生成
	model_ = Model::Create();
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 初期化
	camera_.farZ = 0.0f;
	camera_.Initialize();

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	player_ = new Player();
	player_->Initialize(modelPlayer_, &camera_);

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if (i % 2 == 1) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			} else if (i == 1 && j == 1) {

			} else {
				/*worldTransformBlocks_[i][j] = new WorldTransform();*/

				worldTransformBlocks_[i][j] = nullptr;
			}
		}
	}

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
}

GameScene::~GameScene() {
	// ですトラ
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete modelPlayer_;
	delete player_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Update() { /* 更新勝利を書く */

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

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			// アフィン変換
			WolrdtransformUpdate(*worldTransformBlock);
		}
	}

	// 背景
	skydome_->Update();

	// プレイヤー
	player_->Update();
}

void GameScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// プレイヤー
	player_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			model_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 背景
	skydome_->Draw();

	Model::PostDraw();
}
