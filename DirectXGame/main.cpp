#include "ClearScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

enum class Scene {
	kunknown = 0,

	kTitle,
	kSelect,
	kGame,
	kClera,
	kOver,
};

// ゲームスクリーン
TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
// SelectScene* selectScene = nullptr;
ClearScene* cleraScene = nullptr;

Scene scene = Scene::kunknown;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->isFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			/*selectScene = new SelectScene();
			selectScene->Initialize();*/
			gameScene = new GameScene();
			gameScene->Initialize();
		}

		break;
	case Scene::kSelect:
		/*if (selectScene->isFinished()) {
		    scene = Scene::kGame;
		    delete selectScene;
		    selectScene = nullptr;
		    gameScene = new GameScene();
		    gameScene->Initialize();
		}*/
		break;

	case Scene::kGame:
		if (gameScene->isFinished()) {
			scene = Scene::kGame;
			delete gameScene;
			gameScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize();
			break;
		}
		if (gameScene->isCleraed()) {
			scene = Scene::kClera;
			delete gameScene;
			gameScene = nullptr;
			cleraScene = new ClearScene();
			cleraScene->Initialize();
			break;
		}

		break;
	case Scene::kClera:
		if (cleraScene->isFinished()) {
			scene = Scene::kTitle;
			delete cleraScene;
			cleraScene = nullptr;
			/*selectScene = new SelectScene;
			selectScene->Initialize();*/
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kOver:

		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();

		break;
	case Scene::kSelect:
		/*selectScene->Update();*/
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClera:
		cleraScene->Update();
		break;
	case Scene::kOver:

		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kSelect:
		/*selectScene->Draw();*/
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClera:
		cleraScene->Draw();
		break;
	case Scene::kOver:

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
	delete titleScene;
	/*delete selectScene;*/
	delete gameScene;
	delete cleraScene;

	// nullptrの代入
	gameScene = nullptr;

	// エンジンの終了
	KamataEngine::Finalize();
	return 0;
}
