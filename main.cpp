#include "main.h"
#include "FPS.h"
#include "DXInput.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ウィンドウ生成
	WinApp* win = nullptr;
	win = WinApp::GetInstance();
	win->CreateWindow_(L"DirectX");

	Masage* masage;	//メッセージ
	masage = Masage::GetInstance();

	//DirectX初期化処理
	DirectXCommon* dx = nullptr;
	dx = DirectXCommon::GetInstance();
	dx->Initialize(win);

	//キーボード
	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(win);

	//ゲームシーン
	GameScene* gameScene = nullptr;
	gameScene = new GameScene();
	gameScene->Initialize(dx, input);

	//FPSを固定
	FPS* fps = nullptr;
	fps = new FPS;
	fps->SetFrameRate(60.0f);
	fps->FpsControlBegin();;

#pragma endregion

		//ゲームループ
	while (true)
	{
		//Xボタンで終了メッセ時が来たらゲームループを抜ける 
		if (masage->Update() == 1)break;

#pragma region DirectX毎フレーム処理

		//キーボード更新
		input->Update();

		gameScene->Update();

		dx->PreDraw();
		// 4. 描画コマンド
		gameScene->Draw();

		dx->PostDraw();

#pragma endregion
	}
	
	//FPS固定を解除
	fps->FpsControlEnd();
	dx->EndImgui();

	delete fps;
	delete gameScene;
	/*delete dx;*/

	masage->ExitGameloop();

	//ウィンドウクラスを登録解除
	win->deleteWindow();

	return 0;
}