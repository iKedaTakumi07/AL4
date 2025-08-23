#include "SelectScene.h"
#include "Math.h"
#include <numbers>
using namespace KamataEngine;
void SelectScene::Initialize() {

	modelnumber1_ = Model::CreateFromOBJ("number1");
	modelnumber2_ = Model::CreateFromOBJ("number1");

	const float kScale = 2.0f;

	worldTransformmodelnumber1_.Initialize();
	worldTransformmodelnumber1_.scale_ = {kScale, kScale, kScale};
	worldTransformmodelnumber1_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformmodelnumber1_.translation_.y = -6.0f;
	worldTransformmodelnumber1_.translation_.x = 10.0f;

	worldTransformmodelnumber2_.Initialize();
	worldTransformmodelnumber2_.scale_ = {kScale, kScale, kScale};
	worldTransformmodelnumber2_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformmodelnumber2_.translation_.y = -6.0f;
	worldTransformmodelnumber1_.translation_.x = 20.0f;

	camera_.Initialize();

	fade_ = new Fade;
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void SelectScene::Update() {

	switch (phase_) {
	case SelectScene::Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case SelectScene::Phase::kMain:

		switch (Stage_) {
		case SelectScene::ksousa:
			if (Input::GetInstance()->PushKey(DIK_D)) {
				Stage_ = SelectScene::k1_1;
			}
			break;
		case SelectScene::k1_1:
			if (Input::GetInstance()->PushKey(DIK_A)) {
				Stage_ = SelectScene::ksousa;
			}

			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 0;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			if (Input::GetInstance()->PushKey(DIK_D)) {
				Stage_ = SelectScene::k1_2;
			}

			break;
		case SelectScene::k1_2:

			if (Input::GetInstance()->PushKey(DIK_A)) {
				Stage_ = SelectScene::k1_1;
			}
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 1;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			break;
		}

		break;
	case SelectScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	}

	WorldtransformUpdate(worldTransformmodelnumber1_);
	WorldtransformUpdate(worldTransformmodelnumber2_);

	camera_.TransferMatrix();
}

void SelectScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 描画

	fade_->Draw();
	modelnumber1_->Draw(worldTransformmodelnumber1_, camera_);
	modelnumber2_->Draw(worldTransformmodelnumber2_, camera_);

	Model::PostDraw();
}

SelectScene::~SelectScene() {
	//
	delete modelnumber1_;
	delete modelnumber2_;

	delete fade_;
}
