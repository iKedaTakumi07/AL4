#pragma once
#include "KamataEngine.h"
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

	// カメラ
	KamataEngine::Camera camer_;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
};
