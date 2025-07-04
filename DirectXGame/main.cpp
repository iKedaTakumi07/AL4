#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_03_イケダ_タクミ_AL3");

	// DirectXCommomnインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// imguiインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// ゲームシーンのインスタンス生成
	GameScene* gameScene = new GameScene();
	// ゲームシーンの初期化
	gameScene->Initialize();

	// タイトルシーン
	TitleScene* titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		imguiManager->Begin();

		// ゲームシーンの更新
		gameScene->Update();

		// タイトルシーンの更新
		titleScene->Update();

		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// ここに更新処理を記述する

		// ゲームシーンの描画
		gameScene->Draw();

		// タイトルシーンの描画
		titleScene->Draw();

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
