#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	/*初期化を書く*/

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// 初期化
	worldTransform_.Initialize();
	camea_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, &camea_);
}

GameScene::~GameScene() {
	// デストラ
	delete model_;
	delete player_;
}

void GameScene::Update() { /* 更新勝利を書く */

	// 自キャラの更新
	player_->Update();
}

void GameScene::Draw() {

	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 自キャラの描画
	player_->Draw();

	// 3Dモデル後処理
	Model::PostDraw();
}
