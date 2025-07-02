#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Math.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>

using namespace KamataEngine;

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

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	// フェーズの切り替え
	void ChangePhase();

public:
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// 3Dモデル
	Model* modelblock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticles_ = nullptr;

	// 背景
	Skydome* skydome_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// 追跡カメラ
	CameraController* CameraController_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// カメラ
	Camera camera_;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 敵
	std::list<Enemy*> enemies_;

	// ですパーティクル
	DeathParticles* deathParticles_ = nullptr;

	enum class Phase {
		kPlay,
		kDeath,
	};

	Phase phase_;
};
