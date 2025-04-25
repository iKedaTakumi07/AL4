#include "GameScene.h"

using namespace KamataEngine;

// アフィン変換行列を高速に生成する
Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);

void GameScene::Initialize() { /*初期化を書く*/
	// 3Dモデルを生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);
	// 初期化
	camera_.Initialize();

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

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
			} else {
				worldTransformBlocks_[i][j] = new WorldTransform();

				worldTransformBlocks_[i][j] = nullptr;
			}
		}
	}
}

GameScene::~GameScene() {
	// ですトラ
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;

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
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファ
			worldTransformBlock->TransferMatrix();
		}
	}

	// 背景
	skydome_->Update();
}

void GameScene::Draw() {

	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

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

// アフィン変換行列を高速に生成する
Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
	Matrix4x4 num;
	num.m[0][0] = scale.x * 1.0f * cosf(rotate.y) * cosf(rotate.x) * 1.0f;
	num.m[0][1] = 0.0f;
	num.m[0][2] = 0.0f;
	num.m[0][3] = 0.0f;
	num.m[1][0] = 0.0f;
	num.m[1][1] = scale.y * cosf(rotate.x) * 1.0f * cosf(rotate.z) * 1.0f;
	num.m[1][2] = 0.0f;
	num.m[1][3] = 0.0f;
	num.m[2][0] = 0.0f;
	num.m[2][1] = 0.0f;
	num.m[2][2] = scale.z * cosf(rotate.x) * cosf(rotate.y) * 1.0f * 1.0f;
	num.m[2][3] = 0.0f;
	num.m[3][0] = translate.x;
	num.m[3][1] = translate.y;
	num.m[3][2] = translate.z;
	num.m[3][3] = 1.0f;
	return num;
}