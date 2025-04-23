#pragma once
#include "KamataEngine.h"
#include "Player.h"

// ゲームシーン
class GameScene {
public:
	// 初期化
	void Initialize();

	~GameScene();

	// 更新
	void Update();

	// 描画
	void Draw();

public:
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// ワールドトランスフォームも初期化
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camea_;

	// 自キャラ
	Player* player_ = nullptr;

};
