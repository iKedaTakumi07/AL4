#include "GameScene.h"

using namespace KamataEngine;

// アフィン変換行列を高速に生成する
Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);

void GameScene::Initialize() { /*初期化を書く*/
	// 3Dモデルを生成
	model_ = Model::Create();
	// 初期化
	camer_.translation_ = Vector3(0.0f, 0.0f, -5.0f);
	camer_.Initialize();

	// 要素数
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {

		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}

GameScene::~GameScene() {
	// ですトラ
	delete model_;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}

void GameScene::Update() { /* 更新勝利を書く */
	// ブロックの更新
	for (WorldTransform* worldTransBlock : worldTransformBlocks_) {

		// アフィン変換
		worldTransBlock->matWorld_ = MakeAffineMatrix(worldTransBlock->scale_, worldTransBlock->rotation_, worldTransBlock->translation_);

		// 定数バッファ
		worldTransBlock->TransferMatrix();
	}
}

void GameScene::Draw() {


	Model::PreDraw
	

	// ブロックの描画
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		model_->Draw(*worldTransformBlock, camer_);
	}
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