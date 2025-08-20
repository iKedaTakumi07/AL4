#define NOMINMAX

#include "Enemy.h"
#include "GameScene.h"
#include "MapChipField.h"
#include "Math.h"
#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {

	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	velocity_ = {-kwalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を取得
	Vector3 WorldPos;
	WorldPos.x = worldTransform_.matWorld_.m[3][0];
	WorldPos.y = worldTransform_.matWorld_.m[3][1];
	WorldPos.z = worldTransform_.matWorld_.m[3][2];

	return WorldPos;
}

void Enemy::Update() {

	if (behaviorRequest_ != Behavior::kUnknown) {
		behavior_ = behaviorRequest_;

		switch (behavior_) {
		case Enemy::Behavior::kDefeated:
		default:
			counter_ = 0;
			break;
		}

		behaviorRequest_ = Behavior::kUnknown;
	}

	CollisionMapInfo collisionMapInfo;

	switch (behavior_) {
	case Enemy::Behavior::kWalk:

		// 地面
		if (onGround_) {
			// 移動
			worldTransform_.translation_ += velocity_;
		} else {

			// 空中
			// 移動
			worldTransform_.translation_ += velocity_;
			// 落下速度
			velocity_.y += -kGravityAcceleration / 60.0f;
			// 落下速度制限
			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		}

		collisionMapInfo.move = velocity_;
		collisionMapInfo.landing = false;
		collisionMapInfo.hitWall = false;

		CheckMapCollision(collisionMapInfo);

		UpdateOnWall(collisionMapInfo);

		UpDateOnGround(collisionMapInfo);

		// タイマーの更新
		walkTimer_ += 1.0f / 60.0f;

		// 回転
		// float param = std::sin()
		worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

		// 更新
		WorldtransformUpdate(worldTransform_);
		break;
	case Enemy::Behavior::kDefeated:

		collisionMapInfo.move = velocity_;
		collisionMapInfo.landing = false;
		collisionMapInfo.hitWall = false;

		CheckMapCollision(collisionMapInfo);

		UpdateOnWall(collisionMapInfo);

		UpDateOnGround(collisionMapInfo);

		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStaart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);

		WorldtransformUpdate(worldTransform_);
		if (counter_ >= kDefeatedTime) {
			isDead_ = true;
		}

		break;
	}

	// 旋回制限
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得する
		float destiationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定すル
		worldTransform_.rotation_.y = easeInOutQuint(destiationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

void Enemy::CheckMapCollision(CollisionMapInfo& info) {

	// CheckMapCollisionUP(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

// void Enemy::CheckMapCollisionUP(CollisionMapInfo& info) {
//
//	// 上昇あり?
//	if (info.move.y <= 0) {
//		return;
//	}
//
//	// 移動後の4つの角度の座標
//	std::array<Vector3, KNumCorner> positionsNew;
//
//	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
//		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
//	}
//
//	// 真上の当たり判定を行う
//	MapChipType mapChipType;
//	MapChipType mapChipTypeNext;
//
//	bool hit = false;
//	// 右上点の判定
//	MapChipField::IndexSet indexSet;
//	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
//	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
//	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex + 1);
//	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
//		hit = true;
//	}
//
//	// 右上点の判定
//	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
//	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
//	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex + 1);
//	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
//		hit = true;
//	}
//
//	// ブロックにヒット?
//	if (hit) {
//		// めり込みを排除
//		indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
//		// 現在座標が壁の祖とかの判定
//		MapChipField::IndexSet indexSetNow;
//		indexSetNow = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
//		if (indexSetNow.yindex != indexSet.yindex) {
//			// 　めり込み先のブロックの範囲矩形
//			MapChipField::Rect rect = mapChipFeild_->GetRectByindex(indexSet.xindex, indexSet.yindex);
//			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
//			info.ceiling = true;
//		}
//	}
// }

void Enemy::CheckMapCollisionDown(CollisionMapInfo& info) {

	if (info.move.y >= 0) {
		return;
	}

	std::array<Vector3, KNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックヒットー
	if (hit) {
		// めり込み排除
		indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		// 現在座標が壁の祖とかの判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		if (indexSetNow.yindex != indexSet.yindex) {
			// めり込み先のブロックの範囲矩形
			MapChipField::Rect rect = mapChipFeild_->GetRectByindex(indexSet.xindex, indexSet.yindex);
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			info.landing = true;
		}
	}
}

Vector3 Enemy::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[KNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Enemy::UpdateOnWall(const CollisionMapInfo& info) {

	if (info.hitWall) {
		velocity_.x *= -1.0f;
	}
}

void Enemy::CheckMapCollisionRight(CollisionMapInfo& info) {

	// 右移動あり?
	if (info.move.x <= 0) {
		return;
	}

	std::array<Vector3, KNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 真下の当たり班テオ
	bool hit = false;

	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex - 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex - 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
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

		// 向き
		if (lrDirection_ != LRDirection::kleft) {
			lrDirection_ = LRDirection::kleft;

			// 旋回時の向き/旋回タイマー
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = kTimeTurn;
		}
	}
}

void Enemy::CheckMapCollisionLeft(CollisionMapInfo& info) {
	// 左移動あり?
	if (info.move.x >= 0) {
		return;
	}

	std::array<Vector3, KNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 真下の当たり班テオ
	bool hit = false;

	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[KLeftTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex + 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex + 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
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

			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;

				// 旋回時の向き/旋回タイマー
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
		}
	}
}

void Enemy::Draw() {
	// 描画
	model_->Draw(worldTransform_, *camera_);
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
void Enemy::OnCollision(const Player* player) {

	// ふふふ
	if (behavior_ == Behavior::kDefeated) {
		return;
	}

	if (player->IsAttack()) {
		if (gameScene_) {

			Vector3 pos = player->GetWorldPosition();

			Vector3 effectPos;

			effectPos.x = (GetWorldPosition() + pos).x / 2.0f;
			effectPos.y = (GetWorldPosition() + pos).y / 2.0f;
			effectPos.z = (GetWorldPosition() + pos).z / 2.0f;
			gameScene_->CreateEffect(effectPos);
		}

		behaviorRequest_ = Behavior::kDefeated;

		isCollisionDisabled_ = true;
	}
}

void Enemy::UpDateOnGround(const CollisionMapInfo& info) {

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