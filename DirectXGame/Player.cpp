#include "Player.h"
#include <cassert>

using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
	// nullポインタチェック
	assert(model);

	// 引数の内容をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	camea_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camea_, textureHandle_);
}
