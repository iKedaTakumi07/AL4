#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Enemy {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

private:
	// トランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// 歩きの速さ
	static inline const float kwalkSpeed = 0.02f;
	// 速度
	Vector3 velocity_ = {};
};
