#pragma once
#include <KamataEngine.h>

class Player;

using namespace KamataEngine;

class CameraControlloer {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 瞬間合わせ
	/// </summary>
	void Reset();

	void SetTarget(Player* target) { target_ = target; };

private:
	// 追跡カメラ
	Camera* camera_ = nullptr;

	// 追跡対象
	Player* target_ = nullptr;

	// オフセット
	Vector3 targetOffset_ = {0, 0, -15.0f};
};