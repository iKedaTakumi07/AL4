#include "GameScene.h"
#include "imgui.h"

using namespace KamataEngine;

void GameScene::Initialize() { /*初期化を書く*/
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// ファイル名を指定してテクスチャを読み込む
	modeltextureHandle_ = TextureManager::Load("sample.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ワールドトランスフォーむの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();

	// サウンドデータの読み込み
	soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");

	// 音声再生
	/* Audio::GetInstance()->PlayWave(soundDataHandle_);*/
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);

	// ライン描画
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

GameScene::~GameScene() {
	// デストラクタ
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Update() { /* 更新勝利を書く */

	// スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();
	// 座標を移動
	position.x += 2.0f;
	position.y += 1.0f;
	// 移動した座標をスプライトンに反映
	sprite_->SetPosition(position);

	// スペースキーを押した瞬間
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声停止
		Audio::GetInstance()->StopWave(voiceHandle_);
	}

	// デバックテキストの表示
#ifdef _DEBUG
	ImGui::Begin("Debug1");
	ImGui::Text("kamata Tarou %d.%d.%d", 2050, 12, 31);
	ImGui::End();
	ImGui::Begin("Debug2");
	// float3入力ボックス
	ImGui::InputFloat3("inputfloat3", inputFloat3);
	// float3スライダー
	ImGui::SliderFloat3("sliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

	// デモウィンドウの表示を有効可
	ImGui::ShowDemoWindow();
#endif

	// デバックカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() { /* 描画処理 */
	// DirectXCommomnインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 3Dモデル描画
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), modeltextureHandle_);

	// ライン描画

	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d({float(i), 0, 0}, {float(i), 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
			PrimitiveDrawer::GetInstance()->DrawLine3d({10, float(j), 0}, {0, float(j), 0}, {0.0f, 0.0f, 1.0f, 1.0f});
		}
	}

	// 3Dモデル描画後処理
	Model::PostDraw();
}
