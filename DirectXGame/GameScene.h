#pragma once
#include "AbilityInfo.h"
#include "Bullet.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "GameSceneUI.h"
#include "Goal.h"
#include "HitEffect.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Math.h"
#include "Player.h"
#include "Skydome.h"
#include "cheese.h"
#include "strongEnemy.h"
#include <vector>

class AbilityItem;

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

	void CreateEffect(const KamataEngine::Vector3& position);

public:
	// bgm
	uint32_t soundBGM;
	uint32_t voiceHandel;

	// カメラ
	KamataEngine::Camera camera_;
	// 追跡カメラ
	CameraController* CameraController_ = nullptr;
	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	// 3Dモデル
	uint32_t blockTextureHandele;
	uint32_t chageBlockTextureHandele;
	uint32_t bulletTextTextureHandle;
	uint32_t SpaceJumpTexttextureHandle;
	uint32_t ChargeTextTextureHandle;

	KamataEngine::Model* modelblock_ = nullptr;
	KamataEngine::Model* modelplayerHp_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelPlayerAttck_ = nullptr;
	KamataEngine::Model* modelBullet_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;
	KamataEngine::Model* modelDeathParticles_ = nullptr;
	KamataEngine::Model* modelHitEffect = nullptr;
	KamataEngine::Model* goalmodel_ = nullptr;
	KamataEngine::Model* modelCheese_ = nullptr;

	// 座標
	KamataEngine::WorldTransform tutorialWorldTransform_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// 背景
	Skydome* skydome_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// UI
	GameSceneUI* ui_ = nullptr;
	AbilityInfo* info_;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// 敵
	std::list<Enemy*> enemies_;
	std::list<ExplosionEnemy*> ExplosionEnemies_;

	// プレイヤーの球
	std::list<Bullet*> PlayerBullet_;

	// ですパーティクル
	DeathParticles* deathParticles_ = nullptr;
	std::list<HitEffect*> hitEffects_;

	// アビリティ
	std::list<AbilityItem*> AbilityItem_;

	// フェード
	enum class Phase {
		kFadeIn,
		kPlay,
		kDeath,
		kFadeOut,
		kInformation,
	};
	Phase phase_;
	Fade* fade_ = nullptr;

	Goal* goal = nullptr;

	// フラグ
	bool finished_ = false;
	bool isGoal_ = false;
};