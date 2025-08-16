#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class Goal {
public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition() const;

	bool isCleraed() const { return isGoal_; };

private:
	// トランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// フラグ
	bool isGoal_ = false;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};
