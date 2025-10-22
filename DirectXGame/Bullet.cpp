#define NOMINMAX

#include "Bullet.h"
#include "MapChipField.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;

void Bullet::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Bullet::Update() {

	// 左右加速
	Vector3 acceleration = {};
	if (kBulletdirection.y <= 2.5f) {
		acceleration.x += kbulletSpeed;
	} else {
		acceleration.x += kbulletSpeed * -1.0f;
	}

	// 加速/減速
	velocity_.x += acceleration.x;

	// 最大速度制限
	velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;

	// 衝突チェック
	CheckMapCollision(collisionMapInfo);

	isHit(collisionMapInfo);

	worldTransform_.translation_ += collisionMapInfo.move;

	// 行列を定数バッファに転送
	WorldtransformUpdate(worldTransform_);
}

void Bullet::Draw() { model_->Draw(worldTransform_, *camera_); }

void Bullet::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUP(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Bullet::CheckMapCollisionUP(CollisionMapInfo& info) {

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
	MapChipType mapChipTypeNext;

	bool hit = false;
	// 右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		// めり込みを排除
		indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		// 現在座標が壁の祖とかの判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipFeild_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yindex != indexSet.yindex) {
			// 　めり込み先のブロックの範囲矩形
			MapChipField::Rect rect = mapChipFeild_->GetRectByindex(indexSet.xindex, indexSet.yindex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			info.ceiling = true;
		}
	}
}

void Bullet::CheckMapCollisionDown(CollisionMapInfo& info) {

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

void Bullet::CheckMapCollisionRight(CollisionMapInfo& info) {

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
	}
}

void Bullet::OnCollision() { isDead_ = true; }

void Bullet::CheckMapCollisionLeft(CollisionMapInfo& info) {
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
		}
	}
}

void Bullet::isHit(const CollisionMapInfo& info) {
	if (info.hitWall || info.landing || info.ceiling) {
		isDead_ = true;
	}
}

Vector3 Bullet::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

AABB Bullet::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Bullet::GetWorldPosition() const {
	// ワールド座標を取得
	Vector3 WorldPos;
	WorldPos.x = worldTransform_.translation_.x;
	WorldPos.y = worldTransform_.translation_.y;
	WorldPos.z = worldTransform_.translation_.z;

	return WorldPos;
}
