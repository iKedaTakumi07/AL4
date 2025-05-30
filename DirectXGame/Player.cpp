#define NOMINMAX

#include "Player.h"
#include "Math.h"
#include <algorithm>
#include <cassert>
#include <numbers>

using namespace KamataEngine;



void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {

	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = {1.0f, 1.0f, 0.0f};
}

void Player::Update() {

	// 着地フラグ
	bool landing = false;

	// 移動入力
	// 左右移動操作
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}
				acceleration.x += kAcceleration;
				// 向き
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					// 旋回時の向き/旋回タイマー
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}
				acceleration.x -= kAcceleration;
				// 向き
				if (lrDirection_ != LRDirection::kleft) {
					lrDirection_ = LRDirection::kleft;

					// 旋回時の向き/旋回タイマー
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速/減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			velocity_.x *= (1.0f - kAtteleration);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {

			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
		}

		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}

	} else {

		// 地面との当たり班
		if (velocity_.y < 0) {
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		}

		// 落下速度
		velocity_.y += -kGravityAcceleration;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		// 着地
		if (landing) {
			// めり込み排除
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰
			velocity_.x *= (1.0f - kAtteleration);
			// 下方方向速度をリセット
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}

	// 旋回制限

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得する
		float destiationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定すル
		worldTransform_.rotation_.y = easeInOutQuint(destiationRotationY, turnFirstRotationY_, turnTimer_);
	}

	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 行列を定数バッファに転送
	WolrdtransformUpdate(worldTransform_);
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}
