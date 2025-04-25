#include "Skydome.h"
#include <cassert>

using namespace KamataEngine;

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {

	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
}

void Skydome::Update() {
	// 更新処理

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}
