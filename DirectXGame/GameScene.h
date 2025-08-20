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
	/*Vector3 playerPosition;*/

	// struct EnemyInfo {
	//	std::string type;
	//	Vector3 position;
	// };

	// struct StageInfo {
	//	Vector3 PlayerSpawnPos;
	//	std::vector<EnemyInfo> enemies;
	// };

	// std::vector<StageInfo> stages = {
	//   {{2.0f,0.0f,18.0f}, {{"EnemyA",{25.0f,0.0f,2.0f,}},{"EnemyA",{40.0f,0.0f,2.0f}}}},/*1-1*/
	//   {{2.0f,0.0f,18.0f}, {{"EnemyA",{25.0f,0.0f,2.0f,}},{"EnemyA",{40.0f,0.0f,2.0f}}}},
	//   };
	std::vector<Vector3> stage1Enemies = {
	    {25.0f, 2.0f, 0.0f},
        {30.0f, 5.0f, 0.0f},
        {40.0f, 7.0f, 0.0f},
    };

	std::vector<Vector3> stage2Enemies = {
	    {8.0f, 16.0f, 0.0f},
        {9.0f, 16.0f, 0.0f},
        {10.0f, 16.0f, 0.0f},
        {11.0f, 16.0f, 0.0f},
    };
};