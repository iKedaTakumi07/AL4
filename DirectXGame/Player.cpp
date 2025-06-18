#define NOMINMAX

#include "Player.h"
#include "MapChipField.h"
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

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	/*worldTransform_.translation_ = {1.0f, 1.0f, 0.0f};*/
}

void Player::Update() {

	InputMove();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	/*collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;*/

	// 衝突チェック
	CheckMapCollision(collisionMapInfo);

	worldTransform_.translation_ += collisionMapInfo.move;

	if (collisionMapInfo.ceiling) {
		velocity_.y = 0;
	}

	UpdateOnWall(collisionMapInfo);

	UpDateOnGround(collisionMapInfo);

	// 移動
	/*worldTransform_.translation_ += velocity_;*/

	// 着地フラグ
	/*bool landing = false;*/

	// 地面との当たり班{
	/*if (velocity_.y < 0) {
	    if (worldTransform_.translation_.y <= 1.0f) {
	        landing = true;
	    }
}*/

	// 設置判定
	// if (onGround_) {
	//	// ジャンプ開始
	//	if (velocity_.y > 0.0f) {
	//		onGround_ = false;
	//	}

	//} else {

	//	// 着地
	//	if (landing) {
	//		// めり込み排除
	//		worldTransform_.translation_.y = 1.0f;
	//		// 摩擦で横方向速度が減衰
	//		velocity_.x *= (1.0f - kAtteleration);
	//		// 下方方向速度をリセット
	//		velocity_.y = 0.0f;
	//		onGround_ = true;
	//	}
	//}

	// 旋回制限
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得する
		float destiationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定すル
		worldTransform_.rotation_.y = easeInOutQuint(destiationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	// 行列を定数バッファに転送
	WolrdtransformUpdate(worldTransform_);
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}

void Player::InputMove() {

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
			velocity_.y += kJumpAcceleration / 60.0f;
		}

	} else {

		// 落下速度
		velocity_.y += -kGravityAcceleration / 60.0f;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUP(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUP(CollisionMapInfo& info) {

	// 上昇あり?
	if (info.move.y <= 0) {
		return;
	}

	// 移動後の4つの角度の座標
	std::array<Vector3, KNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 真上の当たり判定を行う
	MapChipType mapChipType;

	bool hit = false;
	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		// めり込みを排除
		indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		// 　めり込み先のブロックの範囲矩形
		MapChipField::Rect rect = mapChipFeild_->GetRectByindex(indexSet.xindex, indexSet.yindex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		info.ceiling = true;
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {

	if (info.move.y >= 0) {
		return;
	}

	std::array<Vector3, KNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 幹下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックヒットー
	if (hit) {
		// めり込み排除
		indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		// めり込み先のブロックの範囲矩形
		MapChipField::Rect rect = mapChipFeild_->GetRectByindex(indexSet.xindex, indexSet.yindex);
		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		info.landing = true;
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {

	// 右移動あり?
	if (info.move.x <= 0) {
		return;
	}

	std::array<Vector3, KNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり班テオ
	bool hit = false;

	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));
		if (indexSetNow.xindex != indexSet.xindex) {

			// めり込み排除
			indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));
			MapChipField::Rect rect = mapChipFeild_->GetRectByindex(indexSet.xindex, indexSet.yindex);
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			// 壁に当たったことを判定結果に記録
			info.hitWall = true;
		}
	}
}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
	// 左移動あり?
	if (info.move.x >= 0) {
		return;
	}

	std::array<Vector3, KNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり班テオ
	bool hit = false;

	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[KLeftTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));

		if (indexSetNow.xindex != indexSet.xindex) {
			// めり込み排除
			indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
			MapChipField::Rect rect = mapChipFeild_->GetRectByindex(indexSet.xindex, indexSet.yindex);
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			// 壁に当たったことを判定結果に記録
			info.hitWall = true;
		}
	}
}

void Player::UpdateOnWall(const CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::UpDateOnGround(const CollisionMapInfo& info) {

	info;

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {

			std::array<Vector3, KNumCorner> positionNew;

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;

			// 真下の当たり班テオ
			bool hit = false;

			// 右下の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 左下点の判定

			indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下判定
			if (!hit) {
				onGround_ = false;
			}
		}

	} else {
		if (info.landing) {
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
	}
}
