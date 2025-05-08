#include "GameScene.h"
#include "MakeAffine.h"

using namespace KamataEngine;

//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

void GameScene::Initialize() { /*初期化を書く*/
	// 3Dモデルを生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skymode", true);
	modelPlayer_ = Model::CreateFromOBJ("skymode");

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 初期化
	camera_.farZ = 0.0f;
	camera_.Initialize();

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	player_ = new Player();
	player_->Initialize(modelPlayer_, &camera_);

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if (i % 2 == 1) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			} else if (i == 1 && j == 1) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;

			} else {
				worldTransformBlocks_[i][j] = new WorldTransform();

				worldTransformBlocks_[i][j] = nullptr;
			}
		}
	}

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
}

GameScene::~GameScene() {
	// ですトラ
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete modelPlayer_;
	delete player_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Update() { /* 更新勝利を書く */

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}

	if (isDebugCameraActive_) {
		// デバックカメラ更新
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;

		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

#endif // _DEBUG

	//// ブロックの更新
	//for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	//	for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
	//		if (!worldTransformBlock)
	//			continue;

	//		// アフィン変換
	//		worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

	//		// 定数バッファ
	//		worldTransformBlock->TransferMatrix();
	//	}
	//}

	// 背景
	skydome_->Update();

	// プレイヤー
	player_->Update();
}

void GameScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			model_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 背景
	skydome_->Draw();

	// プレイヤー
	player_->Draw();

	Model::PostDraw();
}
//
//Matrix4x4 MakeRotateXMatrix(float radian) {
//	Matrix4x4 num;
//	num = {1, 0, 0, 0, 0, std::cos(radian), std::sin(radian), 0, 0, std::sin(-radian), std::cos(radian), 0, 0, 0, 0, 1};
//	return num;
//}
//
//Matrix4x4 MakeRotateYMatrix(float radian) {
//	Matrix4x4 num;
//	num = {std::cos(radian), 0, std::sin(-radian), 0, 0, 1, 0, 0, std::sin(radian), 0, std::cos(radian), 0, 0, 0, 0, 1};
//	return num;
//}
//
//Matrix4x4 MakeRotateZMatrix(float radian) {
//	Matrix4x4 num;
//	num = {std::cos(radian), std::sin(radian), 0, 0, std::sin(-radian), std::cos(radian), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
//	return num;
//}
//
//Matrix4x4 Mulyiply(const Matrix4x4& m1, const Matrix4x4& m2) {
//	Matrix4x4 num;
//	num.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
//	num.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
//	num.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
//	num.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
//
//	num.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
//	num.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
//	num.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
//	num.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
//
//	num.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
//	num.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
//	num.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
//	num.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
//
//	num.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
//	num.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
//	num.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
//	num.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
//
//	return num;
//}
//
//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
//	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
//	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
//	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
//	Matrix4x4 rotateXYZ = Mulyiply(rotateX, Mulyiply(rotateY, rotateZ));
//
//	Matrix4x4 num;
//	num.m[0][0] = scale.x * rotateXYZ.m[0][0];
//	num.m[0][1] = scale.x * rotateXYZ.m[0][1];
//	num.m[0][2] = scale.x * rotateXYZ.m[0][2];
//	num.m[0][3] = 0.0f * 0.0f * 0.0f * 0.0f;
//	num.m[1][0] = scale.y * rotateXYZ.m[1][0];
//	num.m[1][1] = scale.y * rotateXYZ.m[1][1];
//	num.m[1][2] = scale.y * rotateXYZ.m[1][2];
//	num.m[1][3] = 0.0f * 0.0f * 0.0f * 0.0f;
//	num.m[2][0] = scale.z * rotateXYZ.m[2][0];
//	num.m[2][1] = scale.z * rotateXYZ.m[2][1];
//	num.m[2][2] = scale.z * rotateXYZ.m[2][2];
//	num.m[2][3] = 0.0f * 0.0f * 0.0f * 0.0f;
//	num.m[3][0] = translate.x;
//	num.m[3][1] = translate.y;
//	num.m[3][2] = translate.z;
//	num.m[3][3] = 1.0f;
//	return num;
//}
