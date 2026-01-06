#pragma once
#include "KamataEngine.h"
#include "Math.h"

class GameSceneUI {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	void Update();

	void Draw();

	void SetPlayerHp(int Hp) { PlayerHp_ = Hp; };

private:
	// ワールド変換データ
	std::vector<KamataEngine::WorldTransform*> worldTransforms_;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Model* modelHp_ = nullptr;

	int PlayerHp_;
	int MaxPlayerHp_ = 5;
};
