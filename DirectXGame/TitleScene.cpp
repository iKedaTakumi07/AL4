#include "TitleScene.h"
#include "KamataEngine.h"
using namespace KamataEngine;

void TitleScene::Initialize() {
	// モデル
	modelTitle_ = Model::CreateFromOBJ("titleFont", true);
	modelPlayer_ = Model::CreateFromOBJ("player");

	camera_.Initialize();

	worldTransformTitle_.Initialize();

	const float kTitleScale = 2.0f;

	worldTransformTitle_.scale_ = {kTitleScale, kTitleScale, kTitleScale};

	const float kplayerScale = 10.0f;

	worldTransformplayer_.scale_ = {kplayerScale, kplayerScale, kplayerScale};
}

void TitleScene::Update() {}

void TitleScene::Draw() {
	// 描画
	modelPlayer_->Draw(worldTransformplayer_, camera_);
	modelTitle_->Draw(worldTransformTitle_, camera_);
}

TitleScene::~TitleScene() {

	delete modelPlayer_;
	delete modelTitle_;
}
