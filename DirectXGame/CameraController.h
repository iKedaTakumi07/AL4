#pragma once
#include "Math.h"
#include <KamataEngine.h>

class Player;

class CameraController {
public:
	// 矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 外部からポインタをセット
	/// </summary>
	void SetTarget(Player* target) { target_ = target; }

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// カメラ移動範囲
	/// </summary>
	/// <param name="area"></param>
	void SetMovablearea(Rect area) { movableArea_ = area; };

private:
	// 追従しきかっめら
	KamataEngine::Camera* camera_ = nullptr;
	// 対象のポインタ
	Player* target_ = nullptr;

	// 追従対象とカメラの座標(オフセット)
	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};
	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	// カメラの目標座標
	KamataEngine::Vector3 controllePos;
	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f;

	// 速度掛け率
	static inline const float kVelocityBias = 30.0f;

	// 追従対象の移動範囲
	static inline const Rect targetMargi = {-9.0f, 9.0f, -5.0f, 5.0f};
};