#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "Goal.h"
#include "HitEffect.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Math.h"
#include "Player.h"
#include "Skydome.h"
#include "cheese.h"
#include <vector>

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:
	// デストラ
	~GameScene();

	// 初期化
	void Initialize();

	/*void IntStage(int stageId);*/

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

	bool isFinished() const { return finished_; };

	bool isCleraed() const { return isGoal_; };

	void CreateEffect(const Vector3& position);

	void SetStage(int stageid) { stageid_ = stageid; };

public:
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// 3Dモデル
	Model* modelblock_ = nullptr;
	Model* modelPlayerAttck_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticles_ = nullptr;
	Model* modelHitEffect = nullptr;
	Model* goalmodel_ = nullptr;
	Model* modelcheese_ = nullptr;

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

	// スターコイン(チーズ)
	cheese* Cheese_;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 敵
	std::list<Enemy*> enemies_;

	// ですパーティクル
	DeathParticles* deathParticles_ = nullptr;

	enum class Phase {
		kFadeIn,
		kPlay,
		kDeath,
		kFadeOut,
	};

	Phase phase_;

	Fade* fade_ = nullptr;

	bool finished_ = false;

	Goal* goal = nullptr;

	bool isGoal_ = false;

	std::list<HitEffect*> hitEffects_;

	int stageid_;

	std::vector<Vector3> stage1Enemies = {
	    {25.0f, 2.0f, 0.0f},
        {30.0f, 5.0f, 0.0f},
        {40.0f, 7.0f, 0.0f},
        {50.0f, 6.0f, 0.0f}
    };

	std::vector<Vector3> stage2Enemies = {
	    {10.0f, 2.0f,  0.0f},
        {20.0f, 5.0f,  0.0f},
        {45.0f, 7.0f,  0.0f},
        {15.0f, 10.0f, 0.0f},
        {60.0f, 3.0f,  0.0f},
        {80.0f, 4.0f,  0.0f}
    };

	std::vector<Vector3> stage3Enemies = {
	    {18.0f, 2.0f,  0.0f},
        {22.0f, 5.0f,  0.0f},
        {45.0f, 7.0f,  0.0f},
        {60.0f, 10.0f, 0.0f},
        {80.0f, 2.0f,  0.0f}
    };

	struct StageGoal {
		int goalX;
		int goalY;
	};

	StageGoal StageGoals_[3] = {
	    {62, 15},
        {97, 8 },
        {97, 18}
    };
};