#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include "calculation.h"
#include <vector>

// ゲームシーン
class GameScene {
public:
	// 初期化
	void Initialize();

	// デストラ
	~GameScene();

	// 更新
	void Update();

	// 描画
	void Draw();

public:
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// 背景
	Skydome* skydome_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;
};
