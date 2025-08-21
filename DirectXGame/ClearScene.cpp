#include "ClearScene.h"
#include "Math.h"
#include <numbers>

using namespace KamataEngine;

void ClearScene::Initialize() {

	modelClearFont_ = Model::CreateFromOBJ("ClearFont");

	camera_.Initialize();

	worldTransformClearFont_.Initialize();

	const float kClearScale = 2.0f;

	worldTransformClearFont_.scale_ = {kClearScale, kClearScale, kClearScale};
	worldTransformClearFont_.translation_.y = 0.95f * std::numbers::pi_v<float>;

	fade_ = new Fade;
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void ClearScene::Update() {

	switch (phase_) {
	case ClearScene::Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case ClearScene::Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case ClearScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	}

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeClearFontMove);

	float angle = counter_ / kTimeClearFontMove * 2.0f * std::numbers::pi_v<float>;
	worldTransformClearFont_.translation_.y = std::sin(angle) + 10.0f;

	WorldtransformUpdate(worldTransformClearFont_);

	camera_.TransferMatrix();
}

void ClearScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 描画
	modelClearFont_->Draw(worldTransformClearFont_, camera_);

	fade_->Draw();

	Model::PostDraw();
}

ClearScene::~ClearScene() {
	//
	delete fade_;
	delete modelClearFont_;
}