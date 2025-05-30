#pragma once
#include <KamataEngine.h>

class Player;

using namespace KamataEngine;

class CameraController {
public:
	void Initialize(Camera* camera);

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

private:
	// 追跡カメラ
	Camera* camera_ = nullptr;

	// 対象者
	Player* target_ = nullptr;

	// オフセット
	Vector3 targetOffset_ = {0, 0, -15.0f};
};
