#define NOMINMAX

#include "Player.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "Math.h"
#include <algorithm>
#include <cassert>
#include <numbers>

using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model, Model* modelAttack, KamataEngine::Camera* camera, const Vector3& position) {

	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	modelAttack_ = modelAttack;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	worldTransformAttack_.Initialize();
	worldTransformAttack_.translation_ = worldTransform_.translation_;
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;

	objectColor.Initialize();

	objectColor.SetColor({1.0f, 1.0f, 1.0f, 1.0f});
}

void Player::Update() {

	if (behaviorRequest_ != Behavior::kUnknown) {
		behavior_ = behaviorRequest_;

		switch (behavior_) {
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}

		// ふるまいをリセット
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdata();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdata();
		break;
	}

	if (isInvincible) {
		invincibilityTimer -= 1.0f / 60.0f;
		count++;
		if (invincibilityTimer <= 0) {
			isInvincible = false;
		}
	}

	// 行列を定数バッファに転送
	WorldtransformUpdate(worldTransform_);
	WorldtransformUpdate(worldTransformAttack_);
}

void Player::Draw() {

	// 3Dモデルを描画
	if (isInvincible) {
		if (count % 2 == 0) {
			model_->Draw(worldTransform_, *camera_, &objectColor);
		}
	} else {
		model_->Draw(worldTransform_, *camera_, &objectColor);
	}

	if (behavior_ == Behavior::kAttack) {

		switch (attackPhase_) {

		case Player::AttackPhase::kAnticipation:
			break;
		case Player::AttackPhase::kAction:
			break;
		case Player::AttackPhase::kRecovery:
			modelAttack_->Draw(worldTransformAttack_, *camera_);

			break;
		}
	}
}

Vector3 Player::GetWorldPosition() const {
	// ワールド座標を取得
	Vector3 WorldPos;
	WorldPos.x = worldTransform_.translation_.x;
	WorldPos.y = worldTransform_.translation_.y;
	WorldPos.z = worldTransform_.translation_.z;

	return WorldPos;
}

Vector3 Player::GetWorldRotation() const {
	Vector3 WorldRotation;
	WorldRotation.x = worldTransform_.rotation_.x;
	WorldRotation.y = worldTransform_.rotation_.y;
	WorldRotation.z = worldTransform_.rotation_.z;

	return WorldRotation;
}

AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy, AABB pos, AABB pos2) {
	if (IsAttack() || isInvincible) {
		return;
	}

	// 敵のいる方向と逆方向に飛ばす。&2弾ジャンプロック
	if (pos.max.x < pos2.max.x) {
		velocity_ = Vector3(0.0f, 0.0f, 0.0f);
		knockbackVelocity_ = Vector3(knockback, kJumpNockBack / 60.0f, 0.0f);
		isSpaceJump = true;
		onGround_ = false;

	} else {
		velocity_ = Vector3(0.0f, 0.0f, 0.0f);
		knockbackVelocity_ = Vector3(-knockback, kJumpNockBack / 60.0f, 0.0f);
		isSpaceJump = true;
		onGround_ = false;
	}

	// 体力関連
	health--;
	isInvincible = true;
	invincibilityTimer = invincibilityTime;
	count = 0;
	if (health <= 0) {
		isDead_ = true;
	}

	(void)enemy;
}

void Player::OnCollision(const ExplosionEnemy* enemy, AABB pos, AABB pos2) {
	if (IsAttack() || isInvincible) {
		return;
	}

	// 敵のいる方向と逆方向に飛ばす。&2弾ジャンプロック
	if (pos.max.x < pos2.max.x) {
		velocity_ = Vector3(0.0f, 0.0f, 0.0f);
		knockbackVelocity_ = Vector3(knockback, kJumpNockBack / 60.0f, 0.0f);
		isSpaceJump = true;

	} else {
		velocity_ = Vector3(0.0f, 0.0f, 0.0f);
		knockbackVelocity_ = Vector3(-knockback, kJumpNockBack / 60.0f, 0.0f);
		isSpaceJump = true;
	}

	// 体力関連
	health--;
	isInvincible = true;
	invincibilityTimer = invincibilityTime;
	count = 0;
	if (health <= 0) {
		isDead_ = true;
	}

	(void)enemy;
}

void Player::OnCollision(uint32_t i) {

	if (i == uint32_t(0)) {
		isBullet_ = true;
	}
	if (i == uint32_t(2)) {
		isGetCarge_ = true;
	}
	if (i == uint32_t(1)) {
		isGetSpaceJump_ = true;
	}
}
void Player::BehaviorRootInitialize() {}

void Player::BehaviorRootUpdata() {

	InputMove();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_ + knockbackVelocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;

	// 衝突チェック
	CheckMapCollision(collisionMapInfo);

	worldTransform_.translation_ += collisionMapInfo.move;

	if (collisionMapInfo.ceiling) {
		velocity_.y = 0;
		knockbackVelocity_.y = 0.0f;
	}

	UpdateOnWall(collisionMapInfo);

	UpDateOnGround(collisionMapInfo);

	// 旋回制限
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得する
		float destiationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定すル
		worldTransform_.rotation_.y = easeInOutQuint(destiationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	if (Input::GetInstance()->TriggerKey(DIK_E)) {
		// behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::BehaviorAttackInitialize() {

	// 初期化
	attackParameter_ = 0;

	velocity_ = {};

	attackPhase_ = AttackPhase::kAnticipation;
}

void Player::BehaviorAttackUpdata() {

	const Vector3 attackVelocity = {0.8f, 0.0f, 0.0f};

	Vector3 velocity{};

	attackParameter_++;

	switch (attackPhase_) {
	case Player::AttackPhase::kAnticipation:
	default: {
		velocity = {};
		float t = static_cast<float>(attackParameter_) / KanticipationTime;
		worldTransform_.scale_.z = EaseOut(1.0f, 0.3f, t);
		worldTransform_.scale_.y = EaseOut(1.0f, 1.6f, t);

		if (attackParameter_ >= KanticipationTime) {
			attackPhase_ = AttackPhase::kAction;
			attackParameter_ = 0;
		}
		break;
	}
	case Player::AttackPhase::kAction: {
		if (lrDirection_ == LRDirection::kRight) {
			velocity = +attackVelocity;
		} else {
			velocity = -attackVelocity;
		}

		float t = static_cast<float>(attackParameter_) / kActionTime;
		worldTransform_.scale_.z = EaseOut(0.3f, 1.3f, t);
		worldTransform_.scale_.y = EaseIn(1.6f, 0.7f, t);

		// 余韻動作へ移行
		if (attackParameter_ >= kActionTime) {
			attackPhase_ = AttackPhase::kRecovery;
			attackParameter_ = 0; // パラメータをリセット
		}

		break;
	}
	case Player::AttackPhase::kRecovery: {
		velocity = {};
		float t = static_cast<float>(attackParameter_) / kRecoveryTime;
		worldTransform_.scale_.z = EaseOut(1.3f, 1.0f, t);
		worldTransform_.scale_.y = EaseOut(0.7f, 1.0f, t);

		// 通常行動に戻る
		if (attackParameter_ >= kRecoveryTime) {
			behaviorRequest_ = Behavior::kRoot;
		}
		break;
	}
	}

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;

	// 衝突チェック
	CheckMapCollision(collisionMapInfo);

	worldTransform_.translation_ += collisionMapInfo.move;

	// 旋回制限
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得する
		float destiationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定すル
		worldTransform_.rotation_.y = easeInOutQuint(destiationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	worldTransformAttack_.translation_ = worldTransform_.translation_;
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;
}

bool Player::isGetShot() {
	bool num;

	num = isShot_;
	isShot_ = false;

	return num;
}

float Player::isGetCharge() {
	float num;

	num = chargePower;
	objectColor.SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	chargePower = 0.0f;

	return num;
}

void Player::InputMove() {

	// 移動入力
	// 左右移動操作
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {

			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_D)) {
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

			} else if (Input::GetInstance()->PushKey(DIK_A)) {
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

		if (Input::GetInstance()->TriggerKey(DIK_W)) {

			// ジャンプ初速
			velocity_.y += kJumpAcceleration / 60.0f;
		}

	} else {

		if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {

			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_D)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// ブレーキ
					velocity_.x *= (1.0f - kAcceleration / 2.0f);
				}
				acceleration.x += kAcceleration / 2.0f;
				// 向き
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					// 旋回時の向き/旋回タイマー
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// ブレーキ
					velocity_.x *= (1.0f - kAcceleration / 2.0f);
				}
				acceleration.x -= kAcceleration / 2.0f;
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
			velocity_.x *= (1.0f - kAtteleration / 2.0f);
		}

		// スペースジャンプ(2弾ジャンプ)
		if (isGetSpaceJump_) {
			if (Input::GetInstance()->TriggerKey(DIK_W)) {
				if (!isSpaceJump) {

					isSpaceJump = true;

					// 落下速度をリセットする必要あり
					velocity_.y = 0.0f;
					velocity_.y += kSpaceJumpAcceleration / 60.0f;
				}
			}
		}

		// 落下速度
		velocity_.y += -kGravityAcceleration / 60.0f;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 射撃可能か
	if (isBullet_) {
		if (coolTimer >= 0.0f) {
			coolTimer -= 1.0f / 60.0f;
		}

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			// チャージできるか
			if (isGetCarge_) {
				objectColor.SetColor({1.0f, 1.0f - chargePower, 1.0f - chargePower, 1.0f});

				if (coolTimer <= 0.0f) {
					isCharge_ = true;

					if (chargePower < chargeMaxPower) {
						chargePower += 1.0f / 60.0f;
					}
				}
			} else {
				if (coolTimer <= 0.0f) {
					isShot_ = true;
					coolTimer = 0.25f;
				}
			}

		} else {
			if (isCharge_) {
				isCharge_ = false;
				isShot_ = true;
			}
		}
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
	MapChipType mapChipTypeNext;

	bool hit = false;
	// 右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
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

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {

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
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
		hit = true;
		knockbackVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
	}

	// 右下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
		hit = true;
		knockbackVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
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
	MapChipType mapChipTypeNext;

	// 真下の当たり班テオ
	bool hit = false;

	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex - 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
		hit = true;
	}
	// 右下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex - 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
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
	MapChipType mapChipTypeNext;

	// 真下の当たり班テオ
	bool hit = false;

	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[KLeftTop]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex + 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
		hit = true;
	}
	// 左下点の判定

	indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
	mapChipTypeNext = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex + 1, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || mapChipTypeNext != MapChipType::kBreakableBlock && mapChipType == MapChipType::kBreakableBlock ||
	    mapChipType == MapChipType::kChageBreakeBlock && mapChipTypeNext != MapChipType::kChageBreakeBlock) {
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
			if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kBreakableBlock || mapChipType == MapChipType::kChageBreakeBlock) {
				hit = true;
			}
			// 左下点の判定

			indexSet = mapChipFeild_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipFeild_->GetMapChipTypeByIndex(indexSet.xindex, indexSet.yindex);
			if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kBreakableBlock || mapChipType == MapChipType::kChageBreakeBlock) {
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
			isSpaceJump = false;

			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
	}
}
