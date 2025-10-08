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

public:
	// bgm
	uint32_t soundBGM;
	uint32_t voiceHAndel;

	// カメラ
	Camera camera_;
	// 追跡カメラ
	CameraController* CameraController_ = nullptr;
	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

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

	// 座標
	WorldTransform tutorialWorldTransform_;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// 背景
	Skydome* skydome_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// スターコイン(チーズ)→パワーアップアイテムに変更予定
	cheese* Cheese_;

	// 敵
	std::list<Enemy*> enemies_;

	// ですパーティクル
	DeathParticles* deathParticles_ = nullptr;
	std::list<HitEffect*> hitEffects_;

	// フェード
	enum class Phase {
		kFadeIn,
		kPlay,
		kDeath,
		kFadeOut,
	};
	Phase phase_;
	Fade* fade_ = nullptr;

	Goal* goal = nullptr;

	// フラグ
	bool finished_ = false;
	bool isGoal_ = false;

	// 敵配置→josnとかにしたい
	std::vector<Vector3> stage0Enemies = {
	    {38.0f, 7.0f, 0.0f},
	};

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

	StageGoal StageGoals_[4] = {
	    {45, 16},
        {62, 15},
        {97, 8 },
        {97, 18}
    };

	StageGoal Stagecheese_[4] = {
	    {16, 15},
        {17, 13},
        {5,  10},
        {29, 8 }
    };
};