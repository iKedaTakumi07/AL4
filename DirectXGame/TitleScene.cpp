#include "TitleScene.h"
#include "KamataEngine.h"
#include "Math.h"
#include <numbers>

using namespace KamataEngine;

void TitleScene::Initialize() {
	// モデル
	modelTitle_ = Model::CreateFromOBJ("titleFont", true);
	modelPlayer_ = Model::CreateFromOBJ("player");

	camera_.Initialize();

	worldTransformTitle_.Initialize();

	const float kTitleScale = 2.0f;

	worldTransformTitle_.scale_ = {kTitleScale, kTitleScale, kTitleScale};
	worldTransformTitle_.translation_.y = 0.95f * std::numbers::pi_v<float>;

	worldTransformplayer_.Initialize();

	const float kplayerScale = 10.0f;

	worldTransformplayer_.scale_ = {kplayerScale, kplayerScale, kplayerScale};
	worldTransformplayer_.translation_.x = -2.0f;
	worldTransformplayer_.translation_.y = -10.0f;
}

void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);

	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;
	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	camera_.TransferMatrix();

	WolrdtransformUpdate(worldTransformplayer_);

	WolrdtransformUpdate(worldTransformTitle_);
}

void TitleScene::Draw() {

	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 描画
	modelPlayer_->Draw(worldTransformplayer_, camera_);
	modelTitle_->Draw(worldTransformTitle_, camera_);

	Model::PostDraw();
}

TitleScene::~TitleScene() {

	delete modelPlayer_;
	delete modelTitle_;
}
