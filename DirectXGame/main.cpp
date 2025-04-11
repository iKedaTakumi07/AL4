#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_03_イケダ_タクミ_AL3");

	// DirectXCommomnインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// ゲームシーンのインスタンス生成
	GameScene* gameScene = new GameScene();
	// ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManger = ImGuiManager::GetInstance();

	// ゲームシーンの初期化
	gameScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}
		// ImGui受付開始
		imguiManger->Begin();

		// ゲームシーンの更新
		gameScene->Update();

		// Imgui受付終了
		imguiManger->End();

		// 描画開始
		dxCommon->PreDraw();
		// ゲームシーンの描画
		gameScene->Draw();
		// 軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		// ImGui描画
		imguiManger->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの解放
	delete gameScene;
	// nullptrの代入
	gameScene = nullptr;
	// エンジンの終了
	KamataEngine::Finalize();
	return 0;
}
