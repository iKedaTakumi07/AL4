#pragma once
#include "CameraController.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "Enemy.h"
#include <vector>

// ゲームシーン
class GameScene {
public:
	// デストラ
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 表示ブロックの生成
	void GenerateBlocks();

public:
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// 3Dモデル
	KamataEngine::Model* modelblock_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	// 背景
	Skydome* skydome_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// 追跡カメラ
	CameraController* CameraController_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// カメラ
	KamataEngine::Camera camera_;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// 敵
	Enemy* enemy_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;
};
