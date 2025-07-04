#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
using namespace KamataEngine;

enum class Scene {
	kunknown = 0,

	kTitle,
	kGame,
};

// ゲームスクリーン
Scene scene = Scene::kunknown;

void ChangeScene() {
	switch (scene) {

	case Scene::kTitle:
		if (titleScene->isFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			gameScene = new GameScene;
			gameScene->Initialize();
		}

		break;
	case Scene::kGame:
		if (gameScene->isFinished()) {
			scene = Scene::kTitle;
			delete gameScene;
			gameScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();

			break;
		}
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();

		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();

		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_03_イケダ_タクミ_AL3");

	// DirectXCommomnインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// imguiインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// ゲームシーンのインスタンス生成

	// ゲームシーンの初期化

	// タイトルシーン
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();
	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		imguiManager->Begin();

		// ゲームシーンの更新
		ChangeScene();

		UpdateScene();

		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// ここに更新処理を記述する
		DrawScene();

		// 軸上
		AxisIndicator::GetInstance()->Draw();

		// プリミティブのリセット
		PrimitiveDrawer::GetInstance()->Reset();

		// imgui
		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの解放
	delete gameScene;
	delete titleScene;

	// nullptrの代入
	gameScene = nullptr;
	// エンジンの終了
	KamataEngine::Finalize();
	return 0;
}
