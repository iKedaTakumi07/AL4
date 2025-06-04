#pragma once
#include <KamataEngine.h>

class Player;

using namespace KamataEngine;

class CameraController {
public:
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	void Initialize(Camera* camera);

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	void SetMovableArea(Rect area) { movableArea_ = area; }

private:
	// 追跡カメラ
	Camera* camera_ = nullptr;

	// 対象者
	Player* target_ = nullptr;

	// オフセット
	Vector3 targetOffset_ = {0, 0, -15.0f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	// 目標座標
	Vector3 destination_;

	// 座標補間
	static inline const float KinterpolationRate = 0.1f;
};
